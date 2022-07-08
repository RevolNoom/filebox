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
 
    // Return a handle to manipulate a file
    File GetFile(const std::string& filePath);

    // Return the file names and directories at @path
    // Directory path ends with "/"
    // @path can be absolute or relative. Default to current directory
    // @absolute=true returns the absolute path from _rootDirectory.
    // (Only list the files that your executable has permission to Read)
    //
    // If path points to a non-directory, return the filename of the path, without \n at end
    std::string ls(const std::string& path = "", bool absolute = false);

    //void cd(const std::string& path);

    std::filesystem::path pwd();

    //bool CanRead(const std::filesystem::path &p);
    //bool CanWrite(const std::filesystem::path &p);
private:
    // Print Real Working Directory. 
    // I know. I'm a genius at making up names
    // Returns working directory from the system root ("/")
    std::filesystem::path prwd();

    //bool CanHelper(const std::filesystem::path &p, int read_or_write_test_macro);

    // Return the real path that leads to p on the host machine
    std::filesystem::path ResolveVirtualPath(const std::filesystem::path &p);

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

    // Return true if the file is a regular file
    // and can be write / read
    //bool IsValid();

    std::string GetContent();

    void WriteContent(const std::string& content);

    std::string Name();

    bool Exist() const;
    bool IsDirectory() const;

    bool CanRead() const;
    bool CanWrite() const;

    // Return the path to this file
    // If @absolutePath = true, the path will be 
    // absolute on the host Operating System 
    //const std::string GetPath(bool absolutePath = false);

private:
    bool CanHelper(int read_or_write_test_macro) const;

private:
    File(const std::filesystem::path& filePath);

    std::filesystem::path _filePath;
};

#endif /* FILESYSTEM_HPP */
