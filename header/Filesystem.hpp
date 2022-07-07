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

    // void CreateFile(const std::string& filePath, const std::string& content = "");

    // Return the file names and directories at @path
    // Directory path ends with "/"
    // @path can be absolute or relative. Default to current directory
    // @absolute=true returns the absolute path from _rootDirectory.
    // (Only list the files that your executable has permission to Read)
    std::string ls(const std::string& path = "", bool absolute = false);

    void cd(const std::string& path);

    std::filesystem::path pwd();

private:
    // Print Real Working Directory. 
    // I know. I'm a genius at making up names
    // Returns working directory from the system root ("/")
    std::filesystem::path prwd();

    bool CanRead(const std::filesystem::path &p);
    bool CanWrite(const std::filesystem::path &p);

    bool CanHelper(const std::filesystem::path &p, int read_or_write_test_macro);

private:
    std::filesystem::path _rootDirectory;
    std::filesystem::path _cwd;
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
