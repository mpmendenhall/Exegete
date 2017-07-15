/// \file EX_Context.cc

#include "EX_Context.hh"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//--------------------------

void EX_Note::makeNote(const string& s, int l) {
	auto& S = EX_Context::TheContext().currentScope();
	auto& N = S.getNote(l);
	if(!N) N = new EX_Note(s);
	EX_Context::TheContext().addNote(l);
}

//--------------------------

EX_ScopeGuard::EX_ScopeGuard(EX_Scope::ID i): S(EX_Context::TheContext().enterScope(i)) { }

EX_ScopeGuard::~EX_ScopeGuard() {
	EX_Context::TheContext().exitScope(S.id);
}

//--------------------------

EX_ScopeRequest::EX_ScopeRequest(EX_Scope::ID i): S(EX_Context::TheContext().requestScope(i)) { }

EX_ScopeRequest::~EX_ScopeRequest() {
	if(S) EX_Context::TheContext().exitScope(S->id);
}

//--------------------------

EX_Scope& EX_Context::getScope(EX_Scope::ID id) {
	auto it = scopes.find(id);
	if(it != scopes.end()) return *it->second;
	return *(scopes[id] = new EX_Scope(id));
}

EX_Scope& EX_Context::enterScope(EX_Scope::ID id) {
	auto& S = getScope(id);
	callchain.push_back(&S);
	nDisplayEvts.push_back(0);
	return S;
}

EX_Scope* EX_Context::requestScope(EX_Scope::ID id) {
	if(!callchain.size()) return &enterScope(id);
	auto& S = *callchain.back();
	if(!strcmp(get<1>(S.id), get<1>(id)) && !strcmp(get<0>(S.id), get<0>(id))) return nullptr;
	return &enterScope(id);
}

void EX_Context::exitScope(EX_Scope::ID id) {
	assert(callchain.size());
	if(get<0>(id)) assert(id == callchain.back()->id);
	callchain.pop_back();
	nDisplayEvts.pop_back();
}

EX_Context& EX_Context::TheContext() {
	static EX_Context* C = nullptr;
	if(!C) C = new EX_Context();
	return *C;
}

void EX_Context::displayScope() {
	printf(ANSI_COLOR_BLUE "In scope" ANSI_COLOR_CYAN);
	for(auto S: callchain) printf(" > %s", S->getName().c_str());
	printf(ANSI_COLOR_RESET "\n");
}

void EX_Context::addNote(int l) {
	assert(callchain.size());
	if(!(nDisplayEvts.back()++)) displayScope();
	auto n = callchain.back()->getNote(l);
	assert(n);
	printf(ANSI_COLOR_YELLOW);
	for(auto c: callchain) printf("--"); 
	printf(ANSI_COLOR_GREEN " %s\n" ANSI_COLOR_RESET, n->getText().c_str());
}

