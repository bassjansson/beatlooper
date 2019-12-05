CC       = gcc
CXX      = g++
CCFLAGS  = -Wall
CXXFLAGS = -Wall # -Wno-undefined-var-template
LDFLAGS  = -lportaudio -lportmidi -lsndfile -lfftw3f

TARGET  = beatlooper
OBJECTS = Main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OBJECTS)
	mv $@ ../bin/

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm ../bin/$(TARGET)
	rm $(OBJECTS)

touch:
	touch ./*.cpp
	touch ./*.h
