/// \file main.cc Example program using Exegete runtime documentation

/*
TODO:
    - squelch repeats
    - indentify "different" approaches
    - nicer nested colored brackets class
    - variables annotation
    - HTML output interface
 */

#include "Exegete.hh"
#include <stdio.h>
#include <stdlib.h>

class CFoo {
public:
    
    void foo();
    
};

void baz() {
    _EXPLAIN("glorble");
}

void CFoo::foo() { _EXPLAIN("CFoo the bar"); baz(); }



int main(int, char**) {
    printf("Hello, world!\n");
    {
        _EXPLAIN("First annotation");
        for(int i=0; i<10000; i++) { _EXPLAIN("Second annotation"); }
        _EXPLAIN("Third annotation");
        
        for(int i=0; i<3; i++) baz();
        CFoo f;
        f.foo();
    }
    
    printf("Goodbye, world!\n");
    _EXEXIT();
    return EXIT_SUCCESS;
}

