#-----------------------------------------------------
# shell utilities
#-----------------------------------------------------
SHELL        := /bin/bash
ROOTCONFIG   := root-config

#-----------------------------------------------------
# compiler flags and options
#-----------------------------------------------------
# debug
ifneq ($(findstring debug, $(strip $(shell $(ROOTCONFIG) --config))),)
OPT           = -g
endif

# compile with g++
CXX          := $(shell $(ROOTCONFIG) --cxx) -Wall -fPIC
CXXFLAGS     := -c $(OPT)

# link
LD           := $(shell $(ROOTCONFIG) --ld) -Wall -fPIC
LDFLAGS      := $(shell $(ROOTCONFIG) --ldflags) $(OPT)
SOFLAGS      := -shared

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

#-----------------------------------------------------
# directories
#-----------------------------------------------------
PROJROOT     =  .
INCDIR       =  $(PROJROOT)
SRCDIR       =  $(PROJROOT)
LIBDIR       =  $(PROJROOT)
BINDIR       =  $(PROJROOT)
DICTDIR      =  $(PROJROOT)/dict
DOCDIR       =  $(PROJROOT)/docs
TESTDIR      =  $(PROJROOT)/tests

#-----------------------------------------------------
# project source, object, dictionary and lib filenames
#-----------------------------------------------------
# libraries
LIBS         =
LIBS         += libROOTutils.so

LIBFILES     =  $(LIBS:%=$(LIBDIR)/%)

# libraries with multiple source files need special handling
# libROOTutils.so
LIBSRC       =
LIBSRC       += HistUtils.cxx
# LIBSRC       += Parsers.cxx
# LIBSRC       += StyleUtils.cxx

LIBOBJF      =  $(LIBSRC:%.cxx=%.o)

# binaries
BINSRC       =
BINSRC       += test.cc

BINFILES     =  $(BINSRC:%.cc=%)

#-----------------------------------------------------
# canned recipes
#-----------------------------------------------------
define LINK-LIBS =
$(LD) $(LDFLAGS) $(SOFLAGS) $(ROOTLIBS) $^ -o $@
@echo "$@ done"
endef

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------
.PHONY:		all clean docs

all:		$(LIBFILES) $(BINFILES)

# libraries
libROOTutils.so:	$(LIBOBJF)
	$(LINK-LIBS)

%.o:	%.cxx
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -I$(INCDIR) $< -o $@

# Binaries
$(BINFILES): %:	%.cc $(LIBFILES)
	$(CXX) $(OPT) $(ROOTCFLAGS) -I$(INCDIR) $(ROOTLIBS) -L$(LIBDIR) -lROOTutils $< -o $@

clean:
	rm -f *.o
	rm -f *.so
	rm -f $(BINFILES)
