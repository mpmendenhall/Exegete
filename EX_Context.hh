/// \file EX_Context.hh Call-chain context


#include <stdio.h>
#include <map>
using std::map;
#include <tuple>
using std::tuple;
using std::get;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <cassert>
#include <sstream>

namespace EX {
    
    template<typename T>
    string to_str(T x) {
        std::stringstream ss;
        ss << x;
        return ss.str();
    }
    
    
    /// Anootated commentary on a scope
    class Note {
    public:
        /// Destructor
        virtual ~Note() { }
        /// Get text representation
        virtual string getText() { return S; }
        
        size_t n = 0;	///< number of times note has been displayed
        string S;	///< note contents
        
        /// add note at line number in current context
        static void makeNote(const string& s, int l);
    protected:
        /// Constructor
        Note(const string& s): S(s) { }
    };
    
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
        
        ID id;				///< unique ID for scope
        map<int, Note*> notes;	///< annotations provided in scope, by line number
    };
    
    /// Create entrance and exit from scope
    class ScopeGuard {
    public:
        /// Constructor
        ScopeGuard(Scope::ID i);
        /// Destructor
        ~ScopeGuard();
    protected:
        const Scope& S;	///< associated scope
    };
    
    /// Request current or create compatible scope (same file, function)
    class ScopeRequest {
    public:
        /// Constructor
        ScopeRequest(Scope::ID i);
        /// Destructor
        ~ScopeRequest();
    protected:
        const Scope* const S;	///< associated scope if new created, or nullptr
    };
    
    /// Top-level singleton tracking call chain and handling UI
    class Context {
    public:
        /// Retrieve context singleton
        static Context& TheContext();
        /// Clear/delete all context information
        static void DeleteContext();
        
        /// Destructor
        ~Context() { assert(!callchain.size()); for(auto& kv: scopes) delete kv.second;  }
        
        /// Get current scope
        Scope& currentScope() { assert(callchain.size()); return *callchain.back(); }
        /// Get (or create) idendified scope
        Scope& getScope(Scope::ID id);
        
        /// Enter (or create) scope identified by ID
        Scope& enterScope(Scope::ID id);
        /// Enter and return new scope if file/function different than before; otherwise, return nullptr
        Scope* requestScope(Scope::ID id);
        /// Exit current scope (optional consistency check)
        void exitScope(Scope::ID id = {nullptr,nullptr,-1});
        /// Trigger enumerated note for current context
        virtual void addNote(int l);
        
        /// Display current scope information
        void displayScope();
    private:
        /// Constructor
        Context() { }
        
        /// Retrieve context singleton (underlying pointer for cleanup deletion)
        static Context*& _TheContext();
        
        map<Scope::ID, Scope*> scopes; ///< index to known scopes
        vector<Scope*> callchain;         ///< chain of progress through scopes
        vector<int> nDisplayEvts;	     ///< number of events displayed at each callchain level
    };
    
}

