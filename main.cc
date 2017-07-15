/// \file main.cc Example program using Exegete runtime documentation

#include "Exegete.hh"
#include <stdio.h>
#include <stdlib.h>

class CFoo {
public:
	
void foo();

};

void CFoo::foo() {   _EXPLAIN("CFoo the bar"); }

void foo() {
	_EXPLAIN("foo the bar");
}

int main(int, char**) {
	_EXPLAIN("First annotation");
	_EXPLAIN("Second annotation");
	_EXPLAIN("Third annotation");

	for(int i=0; i<3; i++) foo();
	CFoo f;
	f.foo();

	return EXIT_SUCCESS;
}

