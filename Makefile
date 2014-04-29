# shell utilities
SHELL        := /bin/bash
ROOTCONFIG   := root-config

# compiler, flags, and options
CC           := $(shell $(ROOTCONFIG) --cc)
CXX          := $(shell $(ROOTCONFIG) --cxx)
CSTD         := -std=c++11
OPTS         := -g -Wall -fPIC
CFLAGS       := -c $(OPTS)

# linker
LD           := $(shell $(ROOTCONFIG) --ld)
LDFLAGS      := $(shell $(ROOTCONFIG) --ldflags) $(OPTS) -shared

# linking to Boost libraries
BOOSTLIBS    := -lboost_regex

# ROOT compile flags
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)

# linking to ROOT
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOFITLIBS   := -lRooFitCore -lRooFit
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)

# others
HASTHREAD    := $(shell $(ROOTCONFIG) --has-thread)
ROOTDICTTYPE := $(shell $(ROOTCONFIG) --dicttype)
ROOTCINT     := rootcint

# sources
LIBSRC       := $(wildcard *.cxx)
LIBDEPS      := $(LIBSRC:%.cxx=.deps/%.d)
BINSRC       := $(wildcard *.cc)
BINDEPS      := $(BINSRC:%.cc=.deps/%.d)
TESTS        := $(wildcard tests/*.cc)
TESTDEPS     := $(TESTS:%.cc=.deps/%.d)

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------
.PHONY:		all clean distclean

all:		libROOTutils.so $(BINSRC:%.cc=%)

# libraries
libROOTutils.so:	$(LIBSRC:%.cxx=%.o)
	$(LD) $(LDFLAGS) $(BOOSTLIBS) $(ROOTLIBS) $^ -o $@

.deps .deps/tests:%:
	mkdir -p $@

$(LIBDEPS):.deps/%.d:	%.cxx | .deps
	$(CXX) $(CSTD) -MM $< -MF $@

$(BINDEPS):.deps/%.d:	%.cc | .deps
	$(CXX) $(CSTD) -MM $< -MF $@

# do not include when cleaning
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(LIBDEPS)
-include $(BINDEPS)
endif

$(LIBSRC:%.cxx=%.o):%.o:	%.cxx
	$(CXX) $(CSTD) $(CFLAGS) $(ROOTCFLAGS) $< -o $@

$(BINSRC:%.cc=%):%:	%.cc libROOTutils.so
	$(CXX) $(OPTS) $(CSTD) $(ROOTCFLAGS) $(ROOTLIBS) -L. -lROOTutils $< -o $@

$(TESTDEPS):.deps/%.d:	%.cc | .deps/tests
	$(CXX) $(CSTD) -I. -MM $< -MF $@

# do not include when cleaning
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(TESTDEPS)
endif

$(TESTS:%.cc=%):%:	%.cc libROOTutils.so
	$(CXX) $(OPTS) $(CSTD) $(ROOTCFLAGS) -I. $(BOOSTLIBS) $(ROOTLIBS) -L. -lROOTutils $< -o $@

$(TESTS:tests/%.cc=%):%:	tests/%
	@LD_LIBRARY_PATH=. $< $(args)

clean:
	rm -f *.o *.so $(BINSRC:%.cc=%)

distclean:	clean
	rm -rf .deps/
