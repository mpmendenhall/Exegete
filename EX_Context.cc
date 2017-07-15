/// \file EX_Context.cc

#include "EX_Context.hh"
using namespace EX;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//--------------------------

void Note::makeNote(const string& s, int l) {
    auto& S = Context::TheContext().currentScope();
    auto& N = S.getNote(l);
    if(!N) N = new Note(s);
    Context::TheContext().addNote(l);
}

//--------------------------

ScopeGuard::ScopeGuard(Scope::ID i): S(Context::TheContext().enterScope(i)) { }

ScopeGuard::~ScopeGuard() {
    Context::TheContext().exitScope(S.id);
}

//--------------------------

ScopeRequest::ScopeRequest(Scope::ID i): S(Context::TheContext().requestScope(i)) { }

ScopeRequest::~ScopeRequest() {
    if(S) Context::TheContext().exitScope(S->id);
}

//--------------------------

Scope& Context::getScope(Scope::ID id) {
    auto it = scopes.find(id);
    if(it != scopes.end()) return *it->second;
    return *(scopes[id] = new Scope(id));
}

Scope& Context::enterScope(Scope::ID id) {
    auto& S = getScope(id);
    callchain.push_back(&S);
    nDisplayEvts.push_back(0);
    return S;
}

Scope* Context::requestScope(Scope::ID id) {
    if(!callchain.size()) return &enterScope(id);
    auto& S = *callchain.back();
    if(!strcmp(get<1>(S.id), get<1>(id)) && !strcmp(get<0>(S.id), get<0>(id))) return nullptr;
    return &enterScope(id);
}

void Context::exitScope(Scope::ID id) {
    assert(callchain.size());
    if(get<0>(id)) assert(id == callchain.back()->id);
    callchain.pop_back();
    nDisplayEvts.pop_back();
}

Context*& Context::_TheContext() {
    static Context* C = nullptr;
    return C;
}

Context& Context::TheContext() {
    auto& C = _TheContext();
    if(!C) C = new Context();
    return *C;
}

void Context::DeleteContext() {
    auto& C = _TheContext();
    delete C;
    C = nullptr;
}

void Context::displayScope() {
    printf(ANSI_COLOR_BLUE "In scope" ANSI_COLOR_CYAN);
    for(auto S: callchain) printf(" > %s", S->getName().c_str());
    printf(ANSI_COLOR_RESET "\n");
}

void Context::addNote(int l) {
    assert(callchain.size());
    if(!(nDisplayEvts.back()++)) displayScope();
    auto n = callchain.back()->getNote(l);
    assert(n);
    printf(ANSI_COLOR_YELLOW);
    for(auto c: callchain) printf("--"); 
    printf(ANSI_COLOR_GREEN " %s\n" ANSI_COLOR_RESET, n->getText().c_str());
}

