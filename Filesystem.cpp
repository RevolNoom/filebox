#include "Filesystem.hpp"

Filesystem::Filesystem()
{
}


Filesystem::Filesystem(std::string rootDirectory)
{
    // TODO: Check for directory existence
    _rootDirectory = rootDirectory;
}

std::string Filesystem::GetFile(std::string filePath)
{
    return "abcxyz";
}

std::string Filesystem::GetRecursiveFileList(bool absolutePath = false)
{
    std::string prefix("");
    if (absolutePath)
    {
        prefix = _rootDirectory;
    }

    std::string result("");

    for (auto &s: {"Yay.txt", "Hooray.txt"})
        result += prefix + s + std::string(" ");

    // Discard the last extraneous space
    result.pop_back();

    return result;
}