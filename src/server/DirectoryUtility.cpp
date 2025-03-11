/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DirectoryUtility
*/

#include <sys/stat.h>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#include "../../include/DirectoryUtility.hpp"

std::string ftp::DirectoryUtility::getParentDirectory(const std::string &path) {
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash == std::string::npos || lastSlash == 0)
        return "/";
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
    if (!dirExists(resolvedPath.string())) {
        throw std::runtime_error(resolvedPath.string() +
            " No such directory.");
    }
    return resolvedPath.string().substr(rootPath.string().size());
}
