/// \file Exegete.hh Central include for Exegete documentation system

#ifndef EXEGETE_HH
#define EXEGETE_HH
#ifdef ENABLE_EXEGETE

#define TOKENCAT(x, y) x ## y
#define TOKENCAT2(x, y) TOKENCAT(x, y)

#include "EX_Context.hh"
#define _EXSCOPE() EX::ScopeGuard TOKENCAT2(_EX_sg_, __LINE__)({__FILE__, __func__, __LINE__});
#define _EXPLAIN(S) EX::ScopeRequest TOKENCAT2(_EX_sr_, __LINE__)({__FILE__, __func__, __LINE__}); EX::Note::makeNote(S, __LINE__);
#define _EXEXIT() EX::Context::DeleteContext();

#else

#define _EXSCOPE()
#define _EXPLAIN(S)
#define _EXEXIT()

#endif
#endif

