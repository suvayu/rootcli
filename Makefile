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
PROJROOT     =  $(PWD)
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

LIBOBJF      =  $(LIBSRC:%.cxx=$(LIBDIR)/%.o)

# binaries
BINSRC       =
BINSRC       += test.cc

BINFILES     =  $(BINSRC:%.cc=$(BINDIR)/%)

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
.PHONY:		all clean clean-obj clean-so docs

all:		$(LIBFILES) $(BINFILES)

# libraries
$(LIBDIR)/libROOTutils.so:	$(LIBOBJF) | $(LIBDIR)
	$(LINK-LIBS)

$(LIBDIR)/%.o:	$(SRCDIR)/%.cxx #| $(LIBDIR)
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -I$(INCDIR) $< -o $@

# $(LIBDIR):
# 	mkdir -p $(LIBDIR)

# Binaries
$(BINFILES): $(BINDIR)/%:	$(SRCDIR)/%.cc $(LIBFILES) #| $(BINDIR)
	$(CXX) $(OPT) $(ROOTCFLAGS) -I$(INCDIR) $(ROOTLIBS) -L$(LIBDIR) -lROOTutils $< -o $@

# $(BINDIR):
# 	mkdir -p $(BINDIR)

clean:		clean-obj clean-so

clean-obj:
	rm -f $(LIBDIR)/*.o

clean-so:
	rm -f $(LIBDIR)/*.so
