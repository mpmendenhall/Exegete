/// \file Exegete.hh Central include for Exegete documentation system

#ifndef EXEGETE_HH
#define EXEGETE_HH
#ifdef ENABLE_EXEGETE

#define TOKENCAT(x, y) x ## y
#define TOKENCAT2(x, y) TOKENCAT(x, y)

#include "EX_Context.hh"
#define _EXSCOPE() EX_ScopeGuard TOKENCAT2(_ex_sg_, __LINE__)({__FILE__, __func__, __LINE__});
#define _EXPLAIN(S) EX_ScopeRequest TOKENCAT2(_ex_sr_, __LINE__)({__FILE__, __func__, __LINE__}); EX_Note::makeNote(S, __LINE__);

#else

#define _EXSCOPE()
#define _EXPLAIN(S)

#endif
#endif

