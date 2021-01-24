#include "../include/FolderStructure.h"

namespace fs = std::filesystem;

bool createFolders(std::string folderName)
{
    bool ret = false;

    if (!fs::exists(folderName)) {
      ret = fs::create_directories(folderName);
    }

    return ret;
}

bool fileExists(std::string filename)
{
    return fs::exists(filename);
}

bool pathExists(std::filesystem::path path)
{
    return fs::exists(path);
}

void renameFile(std::filesystem::path path, std::string newName)
{
    fs::rename(path, newName);
}

void deleteFolder(std::string folderName)
{
    if (fs::exists(folderName)) {
        fs::remove_all(folderName);
    }
}