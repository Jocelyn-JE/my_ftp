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
#include "DirectoryUtility.hpp"

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(dirExists, true, .init = redirect_all_stdout)
{
    cr_assert_eq(ftp::DirectoryUtility::dirExists("tests"), true);
}

Test(dirExists, false, .init = redirect_all_stdout)
{
    cr_assert_eq(ftp::DirectoryUtility::dirExists("abcd"), false);
}

Test(dirExists, false_file, .init = redirect_all_stdout)
{
    cr_assert_eq(ftp::DirectoryUtility::dirExists("Makefile"), false);
}

Test(removeTrailingSlash, remove, .init = redirect_all_stdout)
{
    std::string path = "tests/";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(path), "tests");
}

Test(removeTrailingSlash, one_length, .init = redirect_all_stdout)
{
    std::string path = "/";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(path), "/");
}

Test(removeTrailingSlash, empty_str, .init = redirect_all_stdout)
{
    std::string path = "";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(path), "");
}

Test(removeTrailingSlash, without_backslash, .init = redirect_all_stdout)
{
    std::string path = "wowowow";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(path), "wowowow");
}

Test(getParentDirectory, root)
{
    printf("Root: %s\n", ftp::DirectoryUtility::getParentDirectory("/").c_str());
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/"), "/");
}

Test(getParentDirectory, one_dir)
{
    printf("One_dir: %s\n", ftp::DirectoryUtility::getParentDirectory("/home").c_str());
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/home"), "/");
}

Test(getParentDirectory, two_dir)
{
    printf("Two_dir: %s\n", ftp::DirectoryUtility::getParentDirectory("/home/user").c_str());
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/home/user"), "/home");
}
