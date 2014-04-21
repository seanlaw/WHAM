#Sean M. Law

include ../makefile.defs

LIBOBJS = Bin.o Histogram.o Misc.o WHAM.o

all:	libwham.$(LIBEXT)

libwham.$(LIBEXT):	$(LIBOBJS)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(LIBFLAGS) $^ -o libwham.$(LIBEXT)

clean:
	rm -rf *.o
	rm -rf *.$(LIBEXT)
	rm -rf *stackdump
	rm -rf *~

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(LIBFLAGS) $(DEBUG) -c $< -o $@
