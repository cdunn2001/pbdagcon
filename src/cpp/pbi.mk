# Darwin/Clang is unhappy with -L for a non-existent directory, so we
# cannot use this file to build on OSX.
# Instead of a bunch of ifdefs, we can conditionally include this file.

# Defines some variables specific to the PBI build env using relative paths
BASEDIR  ?= ../../../../../..
PREBUILT := $(BASEDIR)/smrtanalysis/prebuilt.out
BIFX     := $(BASEDIR)/smrtanalysis/bioinformatics
BLASR    ?= $(BIFX)/lib/cpp/alignment
PBDATA   ?= $(BIFX)/lib/cpp/pbdata
PBBAM    := $(BIFX)/staging/PostPrimary/pbbam
ZLIB     := $(PREBUILT)/zlib/zlib-1.2.5/$(OS_STRING2)/lib

EXTRA_INCDIRS := -I$(PBBAM)/include -I$(PBBAM)/third-party/htslib
EXTRA_LDFLAGS := -L$(PBBAM)/lib -L$(PBBAM)/third-party/htslib -L$(ZLIB)

# We are moving to BAM, which requires extra lib support when compiling against
# libblasr.  This conditional allows backward compatable compilations with 
# PacificBiosciences/blasr_libcpp.
EXTRA_LDLIBS = -lpbbam -lhts -lz -lpthread
