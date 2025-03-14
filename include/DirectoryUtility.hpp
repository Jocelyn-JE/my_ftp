/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DirectoryUtility
*/

#ifndef DIRECTORYUTILITY_HPP_
    #define DIRECTORYUTILITY_HPP_

    #include <bits/stdc++.h>
    #include <string>
    #include "Socket.hpp"

namespace ftp {
class DirectoryUtility {
 public:
    static std::string const &removeTrailingSlash(std::string *path);
    static std::string resolvePath(std::string const &root,
        std::string const &currentDirectory, std::string const &path);
    static std::string resolveCanonicalPath(std::string const &root,
        std::string const &currentDirectory, std::string const &path);
    static std::string getParentDirectory(const std::string &path);
    static bool dirExists(const std::string &path);
    static bool fileExists(const std::string &path);
};
}  // namespace ftp

#endif  // DIRECTORYUTILITY_HPP_
