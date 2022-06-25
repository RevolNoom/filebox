#include "Filesystem.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

// For ThrowErrMsg. 
// Should I put ThrowErrMsg in a separate header???
#include "Connection.hpp"

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif



// I won't do Abstract Factory. Not yet. 
// There's too little differences currently to bother doing it
// Implementation: https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
Filesystem::Filesystem(): Filesystem(std::filesystem::current_path())
{
}


Filesystem::Filesystem(const std::string& rootDirectory):
    _rootDirectory(rootDirectory)
{ 
    //std::cout<<"Filesystem initialized at root path: "<<_rootDirectory<<"\n";
    if (!std::filesystem::exists(_rootDirectory))
        throw std::invalid_argument(std::string("Root directory ") + _rootDirectory.string() + " does not exist!");
}

File Filesystem::GetFile(const std::string& filePath)
{
    //TODO
    std::filesystem::path p(filePath);
    if (p.is_absolute())
        return File(_rootDirectory.string() + "/" + filePath);
    return File(prwd() + "/" + filePath);
}


/*
std::string perms(std::filesystem::perms p)
{
    std::stringstream perm;
    perm << ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-")
              << ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-")
              << ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-")
              << ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-")
              << ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-");
    return perm.str();
}*/

void Filesystem::cd(const std::string& path)
{
    std::filesystem::path cwd(prwd());
    cwd.append(path);

    if (!std::filesystem::is_directory(cwd.string()))
    {
        ThrowErrnoMsg(std::string("Not a directory: ") + cwd.string() + ". ");
    }
    
    _cwd.assign(cwd.string().substr(prwd().length() - 1));
    //std::cout<<"After cd: "<<_cwd<<"\n";
}

std::string Filesystem::pwd()
{
    return _cwd.string() + (_cwd.string().back() == '/' ? "" : "/");
}

std::string Filesystem::prwd()
{
    return _rootDirectory.string() + pwd();
}

std::string Filesystem::ls(const std::string& path, bool recursive, bool absolute)
{
    std::string result("");

    // TODO: Maybe, just maybe, I want to refactor a ResolvePath()?
    std::filesystem::path lsPath(prwd() + path);

    //std::cout<<"ls-ing at: "<<lsPath<<"\n";

    if (recursive)
    {
        std::filesystem::recursive_directory_iterator rdi(lsPath);

        // Recursively iterate this filesystem, looking for files and empty directories.
        for (auto &r: rdi)
        {
            //std::cout<<"At: "<<r.path().string()<<". Perm: "<<perms(r.status().permissions())<<"\n";
            // TODO: Check for user's privileges. They might be SU!
            if ((r.status().permissions() & std::filesystem::perms::others_read) == std::filesystem::perms::none)
            {
                rdi.disable_recursion_pending();
                continue;
            }
            std::string appendToResult("");

            std::filesystem::path path(r.path().string());

            path.assign(path.string().substr(lsPath.string().length() + 1));

            // TODO: Refactor this?
            if (r.is_regular_file())
                appendToResult = path.string() + std::string("\n");
            else if (r.is_directory())
            {
                // Only append empty directories
                bool directoryEmpty = std::filesystem::begin(std::filesystem::directory_iterator(r)) == std::filesystem::directory_iterator();
                if (directoryEmpty)
                    appendToResult = path.string() + std::string("/\n");
            }

            result += appendToResult;
        }
    }
    else
    {
        std::filesystem::directory_iterator di(lsPath);
        for (auto &r: di)
        {
            if ((r.status().permissions() & std::filesystem::perms::others_read) == std::filesystem::perms::none)
                continue;

            std::string appendToResult("");

            std::filesystem::path path(r.path().string());

            path.assign(path.filename());
            
            if (r.is_regular_file())
                appendToResult = path.string() + std::string("\n");
            else if (r.is_directory())
            {
                // Only append empty directories
                bool directoryEmpty = std::filesystem::begin(std::filesystem::directory_iterator(r)) == std::filesystem::directory_iterator();
                if (directoryEmpty)
                    appendToResult = path.string() + std::string("/\n");
            }

            result += appendToResult;
        }

    }


    return result;
}

File::File(const std::string& filePath): _path(filePath)
{
}

bool File::IsValid()
{
    return std::filesystem::is_regular_file(_path);
}

int File::GetSize()
{
    if (!IsValid())
        return -1;
    return std::filesystem::file_size(_path);
}

const std::string File::GetPath(bool absolutePath)
{
    if (absolutePath)
        return std::filesystem::absolute(_path);;
    return _path;
}


// https://stackoverflow.com/questions/7064822/how-to-push-binary-data-into-stdstring
// It's not that I don't know how to push binary data into string
// I want to see if there are any better methods.
std::string File::GetContent()
{
    std::ifstream input(_path, std::ios::binary );
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    int buffSize = buffer.size();
    std::string myString(buffer.begin(), buffer.end());
    return myString;
}

const std::string File::GetFileName()
{
    return _path.filename();
}
