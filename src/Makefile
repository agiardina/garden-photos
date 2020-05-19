CXX = $(shell wx-config --cxx)
GCC = g++
PROGRAM = canvas
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.SUFFIXES:      .o .cpp

.cpp.o :
	$(CXX) $(DEBUG) -Wpedantic -Wall -Wextra -Weffc++ -Wuninitialized -std=c++11 -c `wx-config --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):     $(OBJECTS)
	$(CXX) -Wall -Wpedantic -Wextra -Weffc++ -Wuninitialized -v -o $(PROGRAM) sqlite3.o $(OBJECTS) `wx-config --libs`

clean:
	rm -f canvas.o image_panel.o photos.o $(PROGRAM)

debug: DEBUG = -g

debug: $(PROGRAM)
