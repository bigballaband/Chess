CXX = g++-11 -std=c++20
CXXFLAGS = -Wall -g -MMD -Werror=vla # use -MMD to generate dependencies
SOURCES = $(wildcard *.cc)   # list of all .cc files in the current directory
OBJECTS = ${SOURCES:.cc=.o}  # .o files depend upon .cc files with same names
DEPENDS = ${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
EXEC=chess

# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)  $(LIBFLAGS) -lX11

%.o: %.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBFLAGS)

-include ${DEPENDS}

.PHONY: clean tests
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
	