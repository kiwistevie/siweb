PROGRAM = siweb
ARGUMENTS =

CC = clang++ # use C compiler
CPPFLAGS = -O3 -fopenmp # use C++ standard

LD = clang++ # use C linker
LDFLAGS = -lpthread -lgomp

CFILES = src/$(wildcard *.cpp) # collect c files
OFILES = build/$(CFILES:.cpp=.o)

all: clean depend $(PROGRAM)

$(PROGRAM): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f $(OFILES) $(PROGRAM) .depend

run: $(PROGRAM)
	./build/$(PROGRAM) $(ARGUMENTS)

debug: $(PROGRAM)
	gdb build/$(PROGRAM) $(ARGUMENTS)

DEPENDFILE = .depend

depend:
	$(CC) $(CCFLAGS) -MM $(CFILES) > $(DEPENDFILE)

-include $(DEPENDFILE)
