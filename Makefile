###########################################
# Makefile for Exegete documentation system
###########################################

CXXFLAGS += -std=c++11
objects = EX_Context.o EX_Note.o EX_Scope.o
execs = main

all: $(execs)
objs: $(objects)

# generic rule for everything else .cc
%: %.cc objs
	$(CXX) $(CXXFLAGS) -DENABLE_EXEGETE $< $(objects) $(LDFLAGS) -o $@

.PHONY: doc clean

doc:
	doxygen Doxyfile

clean:
	-rm -f *.o *.a $(execs)

