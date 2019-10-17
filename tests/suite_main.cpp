#include <stdio.h>
#include "platform.h"
#include "ics.h"
#include "neptun_next.h"
#include "suites.h"
#include <check.h>

#define X2(x) #x
#define X(x) X2(x)
#define NN_PWD X(NN__PWD)

START_TEST(test_waiting_for_future_event)
{
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = 30; // 19700101T000000Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(!ongoing);

    close_ics(file);
}
END_TEST

Suite* create_suite_main(void) {
    Suite* ret;
    TCase* tc_waiting_for_future_event;

    ret = suite_create("suite_main");

    tc_waiting_for_future_event = tcase_create("Waiting for future event, no ongoing event");
    tcase_add_test(tc_waiting_for_future_event, test_waiting_for_future_event);
    suite_add_tcase(ret, tc_waiting_for_future_event);

    return ret;
}
