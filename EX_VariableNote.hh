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

namespace EX {
    /// Annotated commentary on a variable
    template<typename T>
    class VariableNote: public Note {
    public:
        /// add variable note in current context
        static void makeVariableNote(const string& s, const string& varnm, const T& v);
        /// Get text representation
        string getText() override { return S; }
    
        string varname;         ///< name of variable
        const T* var = nullptr; ///< pointer to the variable
    };
}
