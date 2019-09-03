OBJECTS=main.o ics.o ini_config.o hash.o
CXXFLAGS=-maes -msse4 -g
LDFLAGS=-lcurl

all: neptun_next

neptun_next: $(OBJECTS)
	$(CXX) -o neptun_next $(OBJECTS) $(LDFLAGS)
