###########################################
# Makefile for Exegete documentation system
###########################################

CXXFLAGS += -std=c++11
objects = EX_Context.o EX_Note.o EX_Scope.o

all: libExegete.so
test: testme
objs: $(objects)

libExegete.so: objs
	$(CXX) -shared -fPIC -o libExegete.so $(objects)

# generic rule for everything else .cc
%: %.cc objs libExegete.so
	$(CXX) $(CXXFLAGS) -DENABLE_EXEGETE $< $(objects) $(LDFLAGS) -L. -lExegete -o $@

.PHONY: doc clean

doc:
	doxygen Doxyfile

clean:
	-rm -f *.o *.so testme

