/// \file main.cc Example program using Exegete runtime documentation

/*
TODO:
    - squelch repeats
    - indentify "different" approaches
    - nicer nested colored brackets class
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
    {
        _EXSCOPE("Example main() function");
        _EXPLAIN("First annotation");
        product(3,4);
        for(int i=0; i<10000; i++) {
            //_EXSCOPE("Loopty-loop");
            _EXPLAIN("Second annotation");
            _EXPLAINVAR("The loop index is",i);
        }
        _EXPLAIN("Third annotation");
        
        vector<double> foo(20);
        _EXPLAINVAR("A vector full of zeroes",foo);
        float blerg[15];
        _EXPLAINVAR("A static array",blerg);
        
        for(int i=0; i<3; i++) baz();
        CFoo f;
        f.foo();
    }
    
    printf("Goodbye, world!\n");
    _EXEXIT();
    return EXIT_SUCCESS;
}

