#include "Filesystem.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sys/stat.h>

// For ThrowErrMsg. 
// Should I put ThrowErrMsg in a separate header???
#include "Connection.hpp"

Filesystem::Filesystem(): 
    Filesystem(std::filesystem::current_path())
{
}


Filesystem::Filesystem(const std::string& rootDirectory):
    _rootDirectory(rootDirectory),
    _cwd(".")
{ 
    std::cout<<"Virtual filesystem initialized at root path: "<<_rootDirectory<<"\n";
    if (!std::filesystem::exists(_rootDirectory))
        throw std::invalid_argument(std::string("Root directory ") + _rootDirectory.string() + " does not exist!");
}

File Filesystem::GetFile(const std::string& filePath)
{
    // TODO: Check read permissions?
    std::filesystem::path p(filePath);
    if (p.is_absolute())
        return File(_rootDirectory.string() + filePath);
    return File(prwd() / filePath);
}

/*
void Filesystem::cd(const std::string& path)
{
    std::filesystem::path cwd(path);
    if (cwd.is_absolute())
        cwd.assign(prwd().string() + path);
    else
        cwd.assign(prwd() / path);

    if (!std::filesystem::is_directory(cwd.string()))
        ThrowErrnoMsg(std::string("Not a directory: ") + cwd.string() + ". ");
    if (!CanRead(cwd))
        ThrowErrnoMsg(std::string("Can't cd: ") + cwd.string() + ". Permission denied.");
    
    _cwd /= path;
}*/


std::filesystem::path Filesystem::pwd()
{
    return _cwd;
}

std::filesystem::path Filesystem::prwd()
{
    return _rootDirectory / _cwd;
}

std::string Filesystem::ls(const std::string& path, bool absolute)
{
    std::string result("");

    std::filesystem::path lsPath(ResolveVirtualPath(path));

    if (!std::filesystem::is_directory(lsPath))
        return lsPath.filename();

    std::filesystem::directory_iterator di(lsPath);
    for (auto &r: di)
    {
        result += r.path().filename();
        if (r.is_directory())
            result +='/';
        result += '\n';
    }
    return result;
}


bool Filesystem::CanRead(const std::filesystem::path& p)
{
    bool can = CanHelper(p, R_OK);
    std::cout<<"Can read "<<p.string()<<"? "<<can<<"\n";
    return can;
}


bool Filesystem::CanWrite(const std::filesystem::path& p)
{
    bool can = CanHelper(p, W_OK);
    std::cout<<"Can write "<<p.string()<<"? "<<can<<"\n";
    return CanHelper(p, W_OK);
}


bool Filesystem::CanHelper(const std::filesystem::path& p, int RWMacro)
{
    // Error when access() return -1 and errno is set
    return access(ResolveVirtualPath(p).c_str(), RWMacro) == 0;
}

std::filesystem::path Filesystem::ResolveVirtualPath(const std::filesystem::path& p)
{
    std::filesystem::path result;
    if (p.is_absolute())
        result = _rootDirectory / p.string().substr(1);
    else
        result = prwd() / p;

    result = std::filesystem::weakly_canonical(result);

    std::cout<<"Resolved "<<p<<" to be "<<result<<"\n";
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
