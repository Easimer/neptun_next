OPTFLAGS=-O2
ifeq ($(DEBUG), 1)
	OPTFLAGS=-g -O0
endif
OBJECTS=main.o ics.o ini_config.o hash.o
CXXFLAGS=-maes -msse4 -Wall $(OPTFLAGS)
LDFLAGS=-lcurl

all: neptun_next

neptun_next: $(OBJECTS)
	$(CXX) -o neptun_next $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) neptun_next

.PHONY: clean
