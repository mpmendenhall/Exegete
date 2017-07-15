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

template<typename T>
string to_str(T x) {
	std::stringstream ss;
	ss << x;
        return ss.str();
}


/// Anootated commentary on a scope
class EX_Note {
public:
	/// Destructor
	virtual ~EX_Note() { }
	/// Get text representation
	virtual string getText() { return S; }

	size_t n = 0;	///< number of times note has been displayed
	string S;	///< note contents

	/// add note at line number in current context
	static void makeNote(const string& s, int l);
protected:
	/// Constructor
	EX_Note(const string& s): S(s) { }
};

class EX_NoCopy {
public:
	/// Destructor
	~EX_NoCopy() { }
	/// No copy!
	void operator=(EX_NoCopy const&) = delete;
	/// No copy!
	EX_NoCopy(const EX_NoCopy&) = delete;
protected:
	/// Default constructor
	EX_NoCopy() { }
};


/// Scope for annotation (file, function, line)
class EX_Scope: protected EX_NoCopy {
public:
	/// unique identifier for scope: (file name, function name, line number)
	typedef tuple<const char*, const char*, int> ID;

	/// Constructor
	EX_Scope(ID i): id(i) { }
	/// Destructor
	virtual ~EX_Scope() { for(auto& kv: notes) delete kv.second; }

	/// get name in string format
	virtual string getName() const { return get<0>(id)+string(":")+get<1>(id) + "["+ to_str(get<2>(id))+"]"; }
	/// display to stdout
	virtual void display() const { printf("Scope %s\n", getName().c_str()); }
	/// get note by line number
	EX_Note*& getNote(int l) { return notes[l]; }
	/// show all notes
	void displayNotes() const { for(auto& kv: notes) printf("%i\t%s\n", kv.first, kv.second? kv.second->getText().c_str() : "[NULL]"); }

	ID id;				///< unique ID for scope
	map<int, EX_Note*> notes;	///< annotations provided in scope, by line number
};

/// Create entrance and exit from scope
class EX_ScopeGuard {
public:
	/// Constructor
	EX_ScopeGuard(EX_Scope::ID i);
	/// Destructor
	~EX_ScopeGuard();
protected:
	const EX_Scope& S;	///< associated scope
};

/// Request current or create compatible scope (same file, function)
class EX_ScopeRequest {
public:
	/// Constructor
	EX_ScopeRequest(EX_Scope::ID i);
	/// Destructor
	~EX_ScopeRequest();
protected:
	const EX_Scope* const S;	///< associated scope if new created, or nullptr
};

/// Top-level singleton tracking call chain and handling UI
class EX_Context {
public:
	/// Retrieve context singleton
	static EX_Context& TheContext();
	/// Destructor
	~EX_Context() { }

	/// Get current scope
	EX_Scope& currentScope() { assert(callchain.size()); return *callchain.back(); }
	/// Get (or create) idendified scope
	EX_Scope& getScope(EX_Scope::ID id);
	
	/// Enter (or create) scope identified by ID
	EX_Scope& enterScope(EX_Scope::ID id);
	/// Enter and return new scope if file/function different than before; otherwise, return nullptr
	EX_Scope* requestScope(EX_Scope::ID id);
	/// Exit current scope (optional consistency check)
	void exitScope(EX_Scope::ID id = {nullptr,nullptr,-1});
	/// Trigger enumerated note for current context
	virtual void addNote(int l);

	/// Display current scope information
	void displayScope();
private:
	/// Constructor
	EX_Context() { }

	map<EX_Scope::ID, EX_Scope*> scopes; ///< index to known scopes
	vector<EX_Scope*> callchain;         ///< chain of progress through scopes
	vector<int> nDisplayEvts;	     ///< number of events displayed at each callchain level
};

