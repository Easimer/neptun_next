#include <stdio.h>
#include <check.h>
#include "suites.h"

#define X2(x) #x
#define X(x) X2(x)
#define NN_PWD X(NN__PWD)

int main(int argc, char** argv) {
    int num_failed;
    Suite* s;
    SRunner* sr;

    fprintf(stderr, "NOTE: will load *.ics files from " NN_PWD "\n");

    s = create_suite_main();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    num_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
