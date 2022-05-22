#include "Filesystem.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

// I won't do Abstract Factory. Not yet. 
// There's too little differences currently to bother doing it
// Implementation: https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
Filesystem::Filesystem()
{
    char pBuf[256];
    size_t len = sizeof(pBuf); 
#ifdef _WIN32
    int bytes = GetModuleFileName(NULL, pBuf, len);
    return bytes ? bytes : -1;{
#else
    int bytes = std::min<int>(readlink("/proc/self/exe", pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
#endif
    _rootDirectory = std::filesystem::path(std::string(pBuf)).remove_filename();
    if (!std::filesystem::exists(_rootDirectory))
        throw std::invalid_argument(std::string("Root directory ") + _rootDirectory.string() + " does not exist!");
}


Filesystem::Filesystem(const std::string& rootDirectory):
    _rootDirectory(rootDirectory)
{ 
    if (!std::filesystem::exists(_rootDirectory))
        throw std::invalid_argument(std::string("Root directory ") + _rootDirectory.string() + " does not exist!");
}

File Filesystem::GetFile(const std::string& filePath)
{
    return File(filePath);
}

std::string Filesystem::GetRecursiveFileList(bool absolutePath)
{
    std::string result("");

    std::filesystem::recursive_directory_iterator rdi(_rootDirectory);


    // Recursively iterate this filesystem, looking for files and empty directories.
    for (auto &r: rdi)
    {
        std::string appendToResult("");

        std::string path(r.path().string());
        if (!absolutePath)
            path = path.substr(_rootDirectory.string().length());

        if (r.is_regular_file())
            appendToResult = path + std::string("\n");
        else if (r.is_directory())
        {
            bool directoryEmpty = std::filesystem::begin(std::filesystem::directory_iterator(r)) == std::filesystem::directory_iterator();
            if (directoryEmpty)
                appendToResult = path + std::string("/\n");
        }

        result += appendToResult;
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
