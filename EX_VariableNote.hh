/// \file EX_VariableNote.hh Annotation of (template-typed) variable value
/*
 * EX_VariableNote.hh from the Exegete runtime documentation system
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

#include "EX_Note.hh"
#include "EX_Scope.hh"
#include "TermColor.hh"
#include <typeinfo>
    
namespace EX {

    /// helper for stringizing std::vectors
    template<typename T>
    string to_str(const vector<T>& v) {
        string s = "[ ";
        for(auto& x: v) s += to_str(x)+" ";
        return s + "]";
    }

    /// Annotated commentary on a variable
    template<typename T>
    class VariableNote: public Note {
    public:
        /// add variable note in current context
        static void makeVariableNote(const string& s, int l, const string& varname, const T& v) {
                auto& S = Context::TheContext().currentScope();
                auto& N = S.getNote(l);
                auto VN = static_cast<VariableNote*>(N);
                if(!N) {
                    N = VN = new VariableNote(s);
                    VN->varname = varname;
                }
                VN->var = &v;
                Context::TheContext().addNote(l);
        }
        
        /// Get text representation
        string getText() override {
            string t =  S+": "+ANSI_COLOR_BLUE;
            //t += "("+typeid(*var).name() + ")"
            return t + varname + ANSI_COLOR_MAGENTA + " = "+(var? to_str(*var) : "NULL");
        }
    
        string varname;         ///< name of variable
        const T* var = nullptr; ///< pointer to the variable
    protected:
        using Note::Note;
    };
}
