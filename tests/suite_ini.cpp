#include <stdio.h>
#include "platform.h"
#include "ini_config.h"
#include "suites.h"
#include <check.h>

#define X2(x) #x
#define X(x) X2(x)
#define NN_PWD X(NN__PWD)

START_TEST(test_open)
{
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_string)
{
    const char* value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_str(cfg, "test_string", &value);
    ck_assert(res);
    ck_assert_str_eq(value, "Test String");

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_empty_string)
{
    const char* value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_str(cfg, "test_empty_string", &value);
    ck_assert(res);
    ck_assert_str_eq(value, "");

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_int)
{
    int value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_int(cfg, "test_int", &value);
    ck_assert(res);
    ck_assert_int_eq(value, 123450);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_overwrite)
{
    const char* value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_str(cfg, "test_overwrite", &value);
    ck_assert(res);
    ck_assert_str_eq(value, "123");

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_bool_false)
{
    bool value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_switch(cfg, "test_bool1", &value);
    ck_assert(res);
    ck_assert(value == false);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_bool_true)
{
    bool value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_switch(cfg, "test_bool2", &value);
    ck_assert(res);
    ck_assert(value == true);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_non_existing_key_bool)
{
    bool value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_switch(cfg, "non_existent", &value);
    ck_assert(!res);
    ck_assert_int_eq(value, false);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_non_existing_key_str)
{
    const char* value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_str(cfg, "non_existent", &value);
    ck_assert(!res);
    ck_assert_ptr_eq(value, NULL);

    close_ini_config(cfg);
}
END_TEST

START_TEST(test_non_existing_key_int)
{
    int value;
    bool res;
    ini_config cfg;
    
    cfg = open_ini_config(NN_PWD "/test_cfg.ini");
    ck_assert(cfg != NULL);

    res = get_ini_config_int(cfg, "non_existent", &value);
    ck_assert(!res);
    ck_assert_int_eq(value, 0);

    close_ini_config(cfg);
}
END_TEST

Suite* create_suite_ini_config(void) {
    Suite* ret;
    TCase *tc_ini;

    ret = suite_create("suite_ini_config");

    tc_ini = tcase_create("INI file reading");
    tcase_add_test(tc_ini, test_open);
    tcase_add_test(tc_ini, test_string);
    tcase_add_test(tc_ini, test_empty_string);
    tcase_add_test(tc_ini, test_int);
    tcase_add_test(tc_ini, test_overwrite);
    tcase_add_test(tc_ini, test_bool_false);
    tcase_add_test(tc_ini, test_bool_true);
    tcase_add_test(tc_ini, test_non_existing_key_bool);
    tcase_add_test(tc_ini, test_non_existing_key_str);
    tcase_add_test(tc_ini, test_non_existing_key_int);
    suite_add_tcase(ret, tc_ini);

    return ret;
}
