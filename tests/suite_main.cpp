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
    u64 curtime = 0; // 19700101T000000Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(!ongoing);
    ck_assert_str_eq(ev.name, "Event1");
    ck_assert_str_eq(ev.location, "Location1");

    close_ics(file);
}
END_TEST

START_TEST(test_ongoing_event)
{
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = 61 * 60; // 19700101T010100Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(ongoing);
    ck_assert_str_eq(ev.name, "Event1");
    ck_assert_str_eq(ev.location, "Location1");

    close_ics(file);
}
END_TEST

START_TEST(test_ongoing_event_first_second)
{
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = 60 * 60; // 19700101T010000Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(ongoing);
    ck_assert_str_eq(ev.name, "Event1");
    ck_assert_str_eq(ev.location, "Location1");

    close_ics(file);
}
END_TEST

START_TEST(test_ongoing_event_last_second)
{
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = 90 * 60 - 1; // 19700101T012959Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(ongoing);
    ck_assert_str_eq(ev.name, "Event1");
    ck_assert_str_eq(ev.location, "Location1");

    close_ics(file);
}
END_TEST

START_TEST(test_ongoing_event_first_after_last_second)
{
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = 90 * 60; // 19700101T013000Z
    ics file = open_ics("file://" NN_PWD "/single_event.ics");
    bool res;

    res = get_next_event(&ev, &ongoing, file, curtime);

    ck_assert(res);
    ck_assert_int_eq(ev.date_start, 60 * 60);
    ck_assert_int_eq(ev.date_end, 60 * 60 + 30 * 60);
    ck_assert(!ongoing);
    ck_assert_str_eq(ev.name, "Event1");
    ck_assert_str_eq(ev.location, "Location1");

    close_ics(file);
}
END_TEST

Suite* create_suite_main(void) {
    Suite* ret;
    TCase *tc_waiting_for_future_event,
          *tc_ongoing_event;

    ret = suite_create("suite_main");

    tc_waiting_for_future_event = tcase_create("Waiting for future event");
    tc_ongoing_event = tcase_create("Ongoing event");
    tcase_add_test(tc_waiting_for_future_event, test_waiting_for_future_event);
    tcase_add_test(tc_ongoing_event, test_ongoing_event);
    tcase_add_test(tc_ongoing_event, test_ongoing_event_first_second);
    tcase_add_test(tc_ongoing_event, test_ongoing_event_last_second);
    tcase_add_test(tc_ongoing_event, test_ongoing_event_first_after_last_second);
    suite_add_tcase(ret, tc_waiting_for_future_event);
    suite_add_tcase(ret, tc_ongoing_event);

    return ret;
}
