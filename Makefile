OPTFLAGS=-O2
ifeq ($(DEBUG), 1)
	OPTFLAGS=-g -O0
endif
OBJECTS=entry.o neptun_next.o ics.o ini_config.o hash.o
CXXFLAGS=-maes -msse4 -Wall $(OPTFLAGS)
LDFLAGS=-lcurl

GENERATED_EXES=neptun_next
ifeq ($(BUILD_TESTS), 1)
	GENERATED_EXES=neptun_next tests/run_tests
endif

all: $(GENERATED_EXES)

neptun_next: $(OBJECTS)
	$(CXX) -o neptun_next $(OBJECTS) $(LDFLAGS)

test: tests/run_tests
	tests/run_tests

tests/run_tests:
	$(MAKE) -C tests

clean:
	rm -f $(OBJECTS) neptun_next
	$(MAKE) -C tests clean

.PHONY: clean test tests/run_tests
