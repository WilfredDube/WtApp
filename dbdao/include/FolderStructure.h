#pragma once

#include <filesystem>
#include <string>
// namespace fs = std::filesystem;

bool createFolders(std::string folderName);

bool fileExists(std::string filename);

void renameFile(std::filesystem::path path, std::string newName);

void deleteFolder(std::string folderName);