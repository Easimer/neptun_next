OBJECTS=main.o ics.o ini_config.o hash.o
CXXFLAGS=-maes -msse4 -g -Wall
LDFLAGS=-lcurl

all: neptun_next

neptun_next: $(OBJECTS)
	$(CXX) -o neptun_next $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) neptun_next

.PHONY: clean
