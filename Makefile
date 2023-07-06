# CILK_ROOT must contains the GCC/Cilk root directory
CXX=g++-5
OS	 		= $(shell uname)
CPPFLAGS    = -DNDEBUG -DMAX_GENUS=$(MAX_GENUS)
CXXFLAGS    = -std=c++11 -fcilkplus -g -Wall -O3 # -fsanitize=thread # -Winline

ifeq ($(shell uname), Darwin)
TARGET_ARCH = -march=corei7 -march=corei7
LDFLAGS 	= -lcilkrts
else
TARGET_ARCH = -march=native -mtune=native
LDFLAGS     = # -Wl,-rpath=$(CILK_ROOT)/lib64
LDLIBS      = -lcilkrts # -ltsan
endif

TARGET = travessia_arvore

SAGE = sage
PYTHON = $(SAGE) -python

# Experimental TBB stuff
# Suppose that TBB is installed in $(TBB_ROOT)
ifdef USE_TBB
CPPFLAGS += -DTBB=1 -I$(TBB_ROOT)/include
LDFLAGS +=-Wl,-rpath=$(TBB_ROOT)/lib/intel64/gcc4.4 -L$(TBB_ROOT)/lib/intel64/gcc4.4 -ltbbmalloc
endif

# Pour compiler avec une valeur différente: make MAX_GENUS=35
DEFAULT_MAX_GENUS=40
MAX_GENUS=$(DEFAULT_MAX_GENUS)

all: $(TARGET)

semigrupo.o: semigrupo.cpp semigrupo.hpp
travessia_arvore.o: travessia_arvore.cpp travessia_arvore.hpp semigrupo.hpp
travessia_arvore: travessia_arvore.o semigrupo.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -rf $(TARGET) *.o
