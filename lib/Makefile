#Sean M. Law

include ../makefile.defs

LIBOBJS = Constants.o Misc.o WHAM.o Histogram.o

all:	libtools.$(LIBEXT)

libtools.$(LIBEXT):	$(LIBOBJS)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(LIBFLAGS) $^ -o libtools.$(LIBEXT)

clean:
	rm -rf *.o
	rm -rf *.$(LIBEXT)
	rm -rf *stackdump
	rm -rf *~

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(LIBFLAGS) $(DEBUG) -c $< -o $@
