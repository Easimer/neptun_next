#include <stdio.h>
#include <check.h>
#include "suites.h"

#define X2(x) #x
#define X(x) X2(x)
#define NN_PWD X(NN__PWD)

using suite_creator_t = Suite* (*)();

int run_suite(Suite* (*suite_creator)()) {
    int num_failed;
    Suite* s;
    SRunner* sr;

    s = suite_creator();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    num_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(int argc, char** argv) {
    int ret = 0;
    fprintf(stderr, "NOTE: will load *.ics files from " NN_PWD "\n");

    ret |= run_suite(create_suite_main);
    ret |= run_suite(create_suite_ini_config);

    return ret ? EXIT_FAILURE : EXIT_SUCCESS;
}
