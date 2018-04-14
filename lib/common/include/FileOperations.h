// Copyright 2017 Michał F. Stępniak
#ifndef LIB_UTILITIES_FILE_OPERATIONS_H_
#define LIB_UTILITIES_FILE_OPERATIONS_H_

#include <memory>
#include <mutex>
#include <string>

namespace utilities
{

bool checkFileExists(const std::string & pathToFile);
void removeFile(const std::string & pathToFile);
void moveFile(const std::string & sourcePath, const std::string & destinationPath);
void copyFile(const std::string & sourcePath, const std::string & destinationPath);

std::string readFile(const std::string & pathToFile);
void writeFile(const std::string & pathToFile, const std::string & content, bool truncate = false);

static std::shared_ptr<std::mutex> getMutex(const std::string & pathToFile);

} // namespace utilities

#endif // LIB_UTILITIES_FILE_OPERATIONS_H_
