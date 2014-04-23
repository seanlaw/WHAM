#Sean M. Law

include ../makefile.defs

OBJS = libwham.$(LIBEXT)

all:	$(BINDIR) libwham.$(LIBEXT) wham

clean:
	rm -rf *.o
	rm -rf *stackdump
	rm -rf ../bin/*
	rm -rf *~

$(BINDIR):
	mkdir -p $(BINDIR)

libwham.$(LIBEXT):
	$(MAKE) -C ../lib libwham.$(LIBEXT)

wham: wham.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEBUG) $(LIBFLAGS) $(INCFLAGS) -o $(BINDIR)/$@ $< $(LINKFLAGS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEBUG) $(INCFLAGS) -c $<
