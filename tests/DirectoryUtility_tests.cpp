/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/DirectoryUtility.hpp"

static void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(dirExists, true, .init = redirect_all_stdout) {
    cr_assert_eq(ftp::DirectoryUtility::dirExists("tests"), true);
}

Test(dirExists, false, .init = redirect_all_stdout) {
    cr_assert_eq(ftp::DirectoryUtility::dirExists("abcd"), false);
}

Test(dirExists, false_file, .init = redirect_all_stdout) {
    cr_assert_eq(ftp::DirectoryUtility::dirExists("Makefile"), false);
}

Test(removeTrailingSlash, remove, .init = redirect_all_stdout) {
    std::string path = "tests/";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(&path), "tests");
}

Test(removeTrailingSlash, one_length, .init = redirect_all_stdout) {
    std::string path = "/";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(&path), "/");
}

Test(removeTrailingSlash, empty_str, .init = redirect_all_stdout) {
    std::string path = "";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(&path), "");
}

Test(removeTrailingSlash, without_backslash, .init = redirect_all_stdout) {
    std::string path = "wowowow";
    cr_assert_eq(ftp::DirectoryUtility::removeTrailingSlash(&path), "wowowow");
}

Test(getParentDirectory, root) {
    std::cout << "Root: " << ftp::DirectoryUtility::getParentDirectory("/")
        << std::endl;
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/"), "");
}

Test(getParentDirectory, one_dir) {
    std::cout << "One_dir: " << ftp::DirectoryUtility::getParentDirectory(
        "/home") << std::endl;
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/home"), "");
}

Test(getParentDirectory, two_dir) {
    std::cout << "Two_dir: " << ftp::DirectoryUtility::getParentDirectory(
        "/home/user") << std::endl;
    cr_assert_eq(ftp::DirectoryUtility::getParentDirectory("/home/user"),
        "/home");
}
