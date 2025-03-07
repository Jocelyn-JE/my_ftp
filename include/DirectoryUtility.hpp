/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DirectoryUtility
*/

#ifndef DIRECTORYUTILITY_HPP_
    #define DIRECTORYUTILITY_HPP_

    #include <bits/stdc++.h>
    #include "Socket.hpp"
    #include <sys/stat.h>

namespace ftp {
    namespace DirectoryUtility {
        std::string &removeTrailingSlash(std::string &path);
        std::string getParentDirectory(const std::string &path);
        bool dirExists(const std::string &path);
    }
}

#endif /* !DIRECTORYUTILITY_HPP_ */