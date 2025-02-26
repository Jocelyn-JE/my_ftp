/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** tests
*/

#include <iostream>
#include <fstream>
#include <string>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(output_test_suite, hello_world_test, .init = redirect_all_stdout)
{
    std::cout << "Hello, World!" << std::endl;
    cr_assert_stdout_eq_str("Hello, World!\n");
}