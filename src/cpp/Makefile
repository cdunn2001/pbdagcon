ifeq ($(wildcard $(PBBAM)/*),)
  include pbi.mk
endif
include boost.mk

COMMON_OBJECTS := Alignment.o AlnGraphBoost.o
PBDAGCON_OBJECTS := BlasrM5AlnProvider.o main.o SimpleAligner.o
DAZCON_OBJECTS := DB.o align.o DazAlnProvider.o dazcon.o

CXXFLAGS = -O3 -std=c++11 -Wall -Wuninitialized -pedantic -I third-party \
		   -I $(BOOST_HEADERS)

CFLAGS = -O3 -Wall -Wextra -fno-strict-aliasing

INCDIRS := -I$(PBDATA) -I$(BLASR) $(EXTRA_INCDIRS)
LDFLAGS := -L$(PBDATA) -L$(BLASR) $(EXTRA_LDFLAGS)

all: pbdagcon

dazcon: LDLIBS = -lpthread
dazcon: $(COMMON_OBJECTS) $(DAZCON_OBJECTS)
	$(CXX) -Wl,--no-as-needed $(OPTIMIZE) -o $@ $^ $(LDLIBS)

pbdagcon: LDLIBS = -lpbdata -lblasr -lpthread $(EXTRA_LDLIBS)
pbdagcon: CXXFLAGS += $(INCDIRS)
pbdagcon: $(COMMON_OBJECTS) $(PBDAGCON_OBJECTS)
	$(CXX) $(LIBDIRS) -Bstatic -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(COMMON_OBJECTS): $(BOOST_HEADERS)

$(BOOST_HEADERS):
	cd third-party && $(GET_BOOST)

clean:
	$(RM) *.d
	$(RM) *.o
	$(RM) pbdagcon
	$(RM) dazcon

.PHONY: all clean
