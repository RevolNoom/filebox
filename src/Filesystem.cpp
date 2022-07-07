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
        return File(_rootDirectory.string() + "/" + filePath);
    return File(prwd() / filePath);
}

void Filesystem::cd(const std::string& path)
{
    std::filesystem::path cwd(prwd());
    cwd /= path;

    if (!std::filesystem::is_directory(cwd.string()))
        ThrowErrnoMsg(std::string("Not a directory: ") + cwd.string() + ". ");
    if (!CanRead(cwd))
        ThrowErrnoMsg(std::string("Can't cd: ") + cwd.string() + ". Permission denied.");
    
    _cwd.assign(cwd);
}

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

    std::cout<<"path: "<<path<<"\n";
    // TODO: Maybe, just maybe, I want to refactor a ResolvePath()?

    std::filesystem::path lsPath(path);

    std::cout<<"prwd: "<<prwd()<<"\n";
    std::cout<<"lsPath: "<<lsPath<<"\n\n";

    if (lsPath.is_absolute())
        lsPath.assign(path.substr(1));
    
    std::cout<<"prwd: "<<prwd()<<"\n";
    std::cout<<"lsPath: "<<lsPath<<"\n\n";

    lsPath.assign(_cwd / lsPath);

    std::cout<<"prwd: "<<prwd()<<"\n";
    std::cout<<"lsPath: "<<lsPath<<"\n\n";

    std::filesystem::directory_iterator di(lsPath);
    for (auto &r: di)
    {
        std::string appendToResult("");

        std::filesystem::path path(r.path().string());

        path.assign(path.filename());
        
        appendToResult = path.string();
        if (r.is_directory())
            appendToResult +='/';

        result += appendToResult + "\n";
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
    struct stat s;
    if (stat(p.string().c_str(), &s) == 0)
        return s.st_mode & RWMacro;
    return false;
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
