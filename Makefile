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

#-----------------------------------------------------
# directories
#-----------------------------------------------------
PROJROOT     =  $(PWD)
DOCDIR       =  $(PROJROOT)/docs

#-----------------------------------------------------
# project source, object, dictionary and lib filenames
#-----------------------------------------------------
# binaries
BINSRC       =
BINSRC       += rootls.cc

BINFILES     =  $(BINSRC:%.cc=%)


#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------
.PHONY:		all clean

all:		$(BINFILES)

# Binaries
$(BINFILES): %:	%.cc
	$(CXX) $(OPT) $(ROOTCFLAGS) $(ROOTLIBS) $< -o $@

clean:
	rm -f $(BINFILES)
