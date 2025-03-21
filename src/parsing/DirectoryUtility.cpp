/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DirectoryUtility
*/

#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <array>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstdio>
namespace fs = std::filesystem;
#include "parsing/DirectoryUtility.hpp"

std::string ftp::DirectoryUtility::getParentDirectory(const std::string &path) {
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash == std::string::npos || lastSlash == 0)
        return "";
    return path.substr(0, lastSlash);
}

bool ftp::DirectoryUtility::dirExists(const std::string &path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

bool ftp::DirectoryUtility::fileExists(const std::string &path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFREG)
        return true;
    else
        return false;
}

std::string const &ftp::DirectoryUtility::removeTrailingSlash(
    std::string *path) {
    if (path->size() > 1 && (*path)[path->size() - 1] == '/')
        *path = path->substr(0, path->size() - 1);
    return *path;
}

std::string ftp::DirectoryUtility::resolvePath(std::string const &root,
    std::string const &currentDirectory, std::string const &inputPath) {
    fs::path rootPath = fs::weakly_canonical(fs::absolute(root));
    fs::path resolvedPath = fs::absolute(currentDirectory) / inputPath;

    // Normalize the path (removes redundant parts like "." and "..")
    resolvedPath = fs::weakly_canonical(resolvedPath);
    // Security check: Ensure the resolved path is within the base directory
    if (resolvedPath.string().find(rootPath.string()) != 0) {
        throw std::runtime_error(resolvedPath.string() +
            " Access denied: Attempt to escape base directory.");
    }
    return resolvedPath.string();
}

std::string ftp::DirectoryUtility::resolveCanonicalPath(std::string const &root,
    std::string const &currentDirectory, std::string const &path) {
    std::string resolvedPath = ftp::DirectoryUtility::resolvePath(root,
        currentDirectory, path);
    if (!ftp::DirectoryUtility::dirExists(resolvedPath))
        throw std::runtime_error(resolvedPath + " No such directory.");
    std::string resolvedRootPath = fs::weakly_canonical(fs::absolute(root));
    return resolvedPath.substr(resolvedRootPath.size());
}

std::string ftp::DirectoryUtility::getLsOutput(const std::string &path) {
    std::array<char, BUFSIZ> buffer;
    std::string result;
    std::string line;
    std::string command = "/bin/ls -la " + path;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        line = buffer.data();
        size_t pos = line.find('\n');
        if (pos != std::string::npos)
            line.replace(pos, 1, "\r\n");
        result += line;
    }
    result.pop_back();
    result.pop_back();
    return result;
}
