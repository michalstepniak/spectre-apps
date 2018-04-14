// Copyright 2017 Michał F. Stępniak
#include <fstream>
#include <map>
#include <sys/stat.h>

#include "../exceptions/CommandShellException.h"
#include "../include/FileOperations.h"
#include "../include/OutputPrinter.h"
#include "../include/SystemCommandShell.h"

namespace
{

std::map<std::string, std::shared_ptr<std::mutex>> fileLocks;

} // namespace

namespace utilities
{

bool checkFileExists(const std::string & pathToFile)
{
	struct stat buffer;
	return (stat(pathToFile.c_str(), &buffer) == 0);
}

void removeFile(const std::string & pathToFile)
{
	std::lock_guard<std::mutex> lock(*getMutex(pathToFile));
	std::string command{"rm -f " + pathToFile};

	execute(command);
}

void moveFile(const std::string & sourcePath, const std::string & destinationPath)
{
	std::lock_guard<std::mutex> lock(*getMutex(sourcePath));
	std::string command{"mv " + sourcePath + " " + destinationPath};

	execute(command);

}

void copyFile(const std::string & sourcePath, const std::string & destinationPath)
{
	std::lock_guard<std::mutex> lock(*getMutex(sourcePath));
	std::string command{"cp " + sourcePath + " " + destinationPath};

	execute(command);
}

std::string readFile(const std::string & pathToFile)
{
	std::lock_guard<std::mutex> lock(*getMutex(pathToFile));
	std::ifstream file(pathToFile);
	std::string content, line;

	if (file.is_open())
	{
		while (getline(file,line))
		{
			content.append(line + "\n");
		}

		file.close();
	}
	else
	{
		printOutput("Failed to open file " + pathToFile, Severity::ERROR);
	}

	return content;
}

void writeFile(const std::string & pathToFile, const std::string & content, bool truncate)
{
	std::lock_guard<std::mutex> lock(*getMutex(pathToFile));
	std::ofstream file(pathToFile, truncate ? std::ios_base::trunc : std::ios_base::app);

	if (file.is_open())
	{
		file << content;
		file.close();
	}
	else
	{
		printOutput("Failed to open file " + pathToFile, Severity::ERROR);
	}
}

std::shared_ptr<std::mutex> getMutex(const std::string & pathToFile)
{
	if (fileLocks.find(pathToFile) == fileLocks.end())
	{
		fileLocks[pathToFile] = std::make_shared<std::mutex>();
	}

	return fileLocks[pathToFile];
}

} // namespace utilities