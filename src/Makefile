#Sean M. Law

include ../makefile.defs

OBJS = libtools.$(LIBEXT)

all:	$(BINDIR) libtools.$(LIBEXT) wham

clean:
	rm -rf *.o
	rm -rf *stackdump
	rm -rf ../bin/*
	rm -rf *~

$(BINDIR):
	mkdir -p $(BINDIR)

libtools.$(LIBEXT):
	$(MAKE) -C ../lib libtools.$(LIBEXT)

wham: wham.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEBUG) $(LINKFLAGS) -o $(BINDIR)/$@ $<

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEBUG) -c $(INCFLAGS) $<
