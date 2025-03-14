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
#include "parsing/DirectoryUtility.hpp"

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

Test(fileExists, true, .init = redirect_all_stdout) {
    std::ofstream outfile("test_file");
    outfile.close();
    cr_assert_eq(ftp::DirectoryUtility::fileExists("test_file"), true);
    std::remove("test_file");
}

Test(fileExists, false, .init = redirect_all_stdout) {
    cr_assert_eq(ftp::DirectoryUtility::fileExists("non_existent_file"), false);
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

Test(lsOutput, root) {
    std::string expected = "total 0\r\n"
"drwxr-xr-x. 1 epi-jo epi-jo 74 11 mars  23:16 .\r\n"
"drwxr-xr-x. 1 epi-jo epi-jo 76 11 mars  16:31 ..\r\n"
"-rw-r--r--. 1 epi-jo epi-jo  0 11 mars  16:31 test_fileA\r\n"
"-rw-r--r--. 1 epi-jo epi-jo  0 11 mars  16:31 test_fileA copy\r\n"
"drwxr-xr-x. 1 epi-jo epi-jo 50 14 mars  14:13 test_folder2";
    cr_assert_eq(ftp::DirectoryUtility::getLsOutput("./tests/test_folder1"),
        expected);
}
