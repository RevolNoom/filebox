#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

class Filesystem 
{
public:

    // Create a filesystem rooted at the directory
    // the executable is in
    Filesystem();

    Filesystem(std::string rootDirectory);

    // Return the file, converted into an std::string
    // Don't bother cout-ing it. Just imagine it's absolute gibberish
    // Return ??? (I haven't decided yet) if the file is not found.
    std::string GetFile(std::string filePath);

    // Return the paths to all the files and empty directories in this filesystem 
    // Each path is separated by a space " "
    // If @absolutePath = true, return absolute paths instead of relative path from
    // this filesystem root
    std::string GetRecursiveFileList(bool absolutePath = false);

private:
    std::string _rootDirectory;
};

#endif /* FILESYSTEM_HPP */