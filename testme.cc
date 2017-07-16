/// \file testme.cc Example program using Exegete runtime documentation

/*
TODO:
    - squelch repeats of subscopes
    - indentify "different" call chain patters
    - nicer nested colored brackets class
    - HTML output interface
 */

#include "Exegete.hh"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using std::vector;

void baz() { _EXPLAIN("glorble"); }

class CFoo {
public:
    void foo() { _EXPLAIN("CFoo the bar"); baz(); }
};

double product(float i, const float j) {
    _EXSCOPE("Floats multiplication");
    _EXPLAINVAR("First argument is",i);
    _EXPLAINVAR("Second argument is",j);
    double k = i*j;
    _EXPLAINVAR("Product is", k);
    return k;
}

int main(int, char**) {
    printf("Hello, world!\n");
    
    { // extra inner scope for annotations, so we can call _EXEXIT() outside.
        
        _EXSCOPE("Example main() function");
        _EXPLAIN("Let's do some silly stuff");
        
        product(3,4);
        for(int i=0; i<10000; i++) {
            //_EXSCOPE("Loopty-loop");
            _EXPLAINVAR("The loop index is",i);
        }
        
        vector<double> foo(20);
        _EXPLAINVAR("A vector full of zeroes",foo);
        float blerg[15];
        _EXPLAINVAR("A static array",blerg);
        
        for(int i=0; i<3; i++) baz();
        CFoo f;
        f.foo();
    }
    
    printf("Goodbye, world!\n");
    _EXEXIT(); // must be outside scope with annotations
    return EXIT_SUCCESS;
}

