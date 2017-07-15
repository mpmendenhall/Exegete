###########################################
# Makefile for Exegete documentation system
###########################################

#.SUFFIXES:
#.SUFFIXES: .c .cc .o

CXXFLAGS += -std=c++11
objects = EX_Context.o
execs = main

.PHONY: doc clean

all: $(objects) $(execs)

# generic rule for everything else .cc
%: %.cc
	$(CXX) $(CXXFLAGS) -DENABLE_EXEGETE $< $(objects) $(LDFLAGS) -o $@

doc:
	doxygen Doxyfile

clean:
	-rm -f *.o *.a $(execs)
