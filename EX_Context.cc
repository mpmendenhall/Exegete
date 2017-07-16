/// \file EX_Context.cc
/*
 * EX_Context.cc from the Exegete runtime documentation system
 * Copyright (c) 2017 Dr. Michael P. Mendenhall
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "EX_Context.hh"
using namespace EX;
#include <cmath>
#include "TermColor.hh"

Scope& Context::getScope(Scope::ID id) {
    auto it = scopes.find(id);
    if(it != scopes.end()) return *it->second;
    return *(scopes[id] = new Scope(id));
}

Scope& Context::enterScope(Scope::ID id) {
    auto& S = getScope(id);
    if(callchain.size()) callchain.back().scopecounts[&S]++;
    callchain.push_back(Subcontext(&S, (int)callchain.size()));
    if(callchain.size() % 2) callchain.back().dpfx = ANSI_COLOR_YELLOW "|" ANSI_COLOR_RESET;
    else callchain.back().dpfx = ANSI_COLOR_RED "|" ANSI_COLOR_RESET;
    return S;
}

Scope* Context::requestScope(Scope::ID id) {
    if(!callchain.size()) return &enterScope(id);
    auto& S = *callchain.back().S;
    if(!strcmp(get<1>(S.id), get<1>(id)) && !strcmp(get<0>(S.id), get<0>(id))) return nullptr;
    return &enterScope(id);
}

void Context::dispbracket(int d) const {
    for(auto& s: callchain) if(s.depth <= d) printf("%s", s.dpfx.c_str());
}

void Context::exitScope(Scope::ID id) {
    assert(callchain.size());
    auto& SC = callchain.back();
    if(get<0>(id)) assert(id == SC.S->id);
    if(SC.notecounts.size()) {
        dispbracket(SC.depth-1);
        if(SC.depth % 2) printf(ANSI_COLOR_RED "+--\n" ANSI_COLOR_RESET);
        else printf(ANSI_COLOR_YELLOW "+--\n" ANSI_COLOR_RESET);
    }
    callchain.pop_back();
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
    assert(!C->callchain.size());
    delete C;
    C = nullptr;
}

void Context::displayScope() {
    if(callchain.size() % 2) printf(ANSI_COLOR_YELLOW "+--" ANSI_COLOR_CYAN);
    else  printf(ANSI_COLOR_RED "+--" ANSI_COLOR_CYAN);
    for(auto& SC: callchain) printf(" > %s", SC.S->getName().c_str());
    if(callchain.size() && callchain.back().S->descrip.size()) printf(ANSI_COLOR_BLUE " '%s'", callchain.back().S->descrip.c_str());
    printf(ANSI_COLOR_RESET "\n");
}

inline bool doDisplay(int i) {
    int ii = pow(10, (int)log10(i));
    return !(i%ii) && i/ii <= 3;
}

void Context::addNote(int l) {
    assert(callchain.size());
    auto& SC = callchain.back();
    if(!SC.notecounts.size()) {
        dispbracket(SC.depth-1);
        displayScope();
    }
    
    auto n = SC.S->getNote(l);
    assert(n);
    int nrpt = ++SC.notecounts[l];
    if(doDisplay(nrpt)) {
        dispbracket(SC.depth);
        printf(ANSI_COLOR_BLUE " [%i", l);
        if(nrpt > 1) printf(" repeat %i", nrpt);
        printf("] " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, n->getText().c_str());
    }
}

