/// \file EX_Scope.hh Defines a "scope" associated with a specific annotated block of code
/*
 * EX_Scope.hh from the Exegete runtime documentation system
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

#ifndef EX_SCOPE_HH
#define EX_SCOPE_HH

#include "EX_Note.hh"
#include <map>
using std::map;
#include <tuple>
using std::tuple;
using std::get;
#include <stdio.h>
#include <sstream>

namespace EX {
    
    template<typename T>
    string to_str(T x) {
        std::stringstream ss;
        ss << x;
        return ss.str();
    }
    
    /// Base class to avoid copying objects that shouldn't be
    class NoCopy {
    public:
        /// Destructor
        ~NoCopy() { }
        /// No copy!
        void operator=(NoCopy const&) = delete;
        /// No copy!
        NoCopy(const NoCopy&) = delete;
    protected:
        /// Default constructor
        NoCopy() { }
    };
    
    
    /// Scope for annotation (file, function, line)
    class Scope: protected NoCopy {
    public:
        /// unique identifier for scope: (file name, function name, line number)
        typedef tuple<const char*, const char*, int> ID;
        
        /// Constructor
        Scope(ID i): id(i) { }
        /// Destructor
        virtual ~Scope() { for(auto& kv: notes) delete kv.second; }
        
        /// get name in string format
        virtual string getName() const { return get<0>(id)+string(":")+get<1>(id) + "["+ to_str(get<2>(id))+"]"; }
        /// display to stdout
        virtual void display() const { printf("Scope %s\n", getName().c_str()); }
        /// get note by line number
        Note*& getNote(int l) { return notes[l]; }
        /// show all notes
        void displayNotes() const { for(auto& kv: notes) printf("%i\t%s\n", kv.first, kv.second? kv.second->getText().c_str() : "[NULL]"); }
        
        ID id;                  ///< unique ID for scope
        map<int, Note*> notes;  ///< annotations provided in scope, by line number
    };
    
    /// Create entrance and exit from scope
    class ScopeGuard {
    public:
        /// Constructor
        ScopeGuard(Scope::ID i);
        /// Destructor
        ~ScopeGuard();
    protected:
        const Scope& S; ///< associated scope
    };
    
    /// Request current or create compatible scope (same file, function)
    class ScopeRequest {
    public:
        /// Constructor
        ScopeRequest(Scope::ID i);
        /// Destructor
        ~ScopeRequest();
    protected:
        const Scope* const S;   ///< associated scope if new created, or nullptr
    };
}

#endif
