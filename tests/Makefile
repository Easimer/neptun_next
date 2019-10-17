CXXFLAGS=`pkg-config --cflags check` -I ../ -DNN__PWD="`pwd`"
LDFLAGS=`pkg-config --libs check` -lcurl
MAIN=main.o
SUITES=suite_main.o
MODULES=../neptun_next.o ../ics.o ../hash.o ../ini_config.o
RUN_TESTS=run_tests

all: $(RUN_TESTS)

$(RUN_TESTS): $(MAIN) $(SUITES) $(MODULES)
	$(CXX) $(LDFLAGS) -o $(RUN_TESTS) $(MAIN) $(SUITES) $(MODULES)

clean:
	rm -f $(MAIN) $(SUITES) $(RUN_TESTS)

.PHONY: clean
