#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>

void error()
{
    fprintf(stderr, "This is an error message\n");
}

Test(Basics, simple_test)
{
    int actual_value = 0;
    int expected = 1;
    cr_expect_eq(actual_value, expected);
}
