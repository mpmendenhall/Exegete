/// \file Exegete.hh Central include for Exegete documentation system
/*
 * Exegete.hh from the Exegete runtime documentation system
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

#ifndef EXEGETE_HH
#define EXEGETE_HH

/// define this to enable Exegete functions; undefine to remove them
#ifdef ENABLE_EXEGETE

/// helper functions to glom items together into a token name
#define TOKENCAT(x, y) x ## y
/// extra layer of indirection needed for use in preprocession macros
#define TOKENCAT2(x, y) TOKENCAT(x, y)

#include "EX_Context.hh"
#include "EX_VariableNote.hh"

/// Simple text comment attached to current scope
#define _EXPLAIN(S) EX::ScopeRequest TOKENCAT2(_EX_sr_, __LINE__)({__FILE__, __func__, __LINE__}); EX::Note::makeNote(S, __LINE__);

/// Start a new named scope with a descriptive string
#define _EXSCOPE(S) EX::ScopeGuard TOKENCAT2(_EX_sg_, __LINE__)({__FILE__, __func__, __LINE__}, S);

/// Text comment showing the value of a variable
#define _EXPLAINVAR(S,v) EX::ScopeRequest TOKENCAT2(_EX_sr_, __LINE__)({__FILE__, __func__, __LINE__}); EX::VariableNote<decltype(v)>::makeVariableNote(S, __LINE__, #v, v);

/// Optional, memory cleanup at end of program --- must occur after all annotated scopes have closed
#define _EXEXIT() EX::Context::DeleteContext();


#else

#define _EXSCOPE(S)
#define _EXPLAIN(S)
#define _EXPLAINVAR(S,v)
#define _EXEXIT()

#endif

#endif

