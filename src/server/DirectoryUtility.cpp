/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DirectoryUtility
*/

#include "DirectoryUtility.hpp"

std::string ftp::DirectoryUtility::getParentDirectory(const std::string &path)
{
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash == std::string::npos || lastSlash == 0)
        return "/";
    return path.substr(0, lastSlash);
}

bool ftp::DirectoryUtility::dirExists(const std::string &path)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

std::string &ftp::DirectoryUtility::removeTrailingSlash(std::string &path)
{
    if (path.size() > 1 && path[path.size() - 1] == '/')
        path = path.substr(0, path.size() - 1);
    return path;
}
