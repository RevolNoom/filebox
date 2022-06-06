#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <filesystem>

class File;

class Filesystem 
{
public:

    // Create a filesystem rooted at the directory
    // the executable is in
    Filesystem();

    Filesystem(const std::string& rootDirectory);
 
    File GetFile(const std::string& filePath);

    // Return the paths to all the files and empty directories in this filesystem 
    // Each path is separated by a newline "\n"
    // If @absolutePath = true, return absolute paths instead of relative path from
    // this filesystem root
    std::string GetRecursiveFileList(bool absolutePath = false);

private:
    std::filesystem::path _rootDirectory;
};

class File
{
public:
    friend class Filesystem;

    // Return the size in bytes
    // -1 if the file is invalid.
    int GetSize();

    // Return true if the file is a regular file.
    bool IsValid();

    std::string GetContent();

    const std::string GetFileName();

    // Return the path to this file
    // If @absolutePath = true, the path will be 
    // absolute on the host Operating System 
    const std::string GetPath(bool absolutePath = false);

private:
    File() {}
    File(const std::string& filePath);

    std::filesystem::path _path;
};

#endif /* FILESYSTEM_HPP */
