#pragma once

#include <filesystem>
#include <string>

bool createFolders(std::string folderName);

bool fileExists(std::string filename);
bool pathExists(std::filesystem::path path);

void renameFile(std::filesystem::path path, std::string newName);

void deleteFolder(std::string folderName);