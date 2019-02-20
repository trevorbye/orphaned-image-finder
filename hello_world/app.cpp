#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "ContainerEntity.h"

using namespace boost::filesystem;
using namespace std;

string testPath(string startPath);
ContainerEntity traverseDirs(string startPath);
std::string replaceChars(string raw);
std::list<string> runComparison(std::list<string> includedPathList, std::list<ImageLocationEntity> imageList);

int main() {
	string startPath;
	bool invalidPath = true;
	cout << "Scraper initialized....\n";

	while (invalidPath) {
		cout << "Enter a directory path to start recursively crawling." << endl;
		cin >> startPath;
		cin.ignore();

	    startPath = testPath(startPath);
		
		if (startPath != "INVALID") {
			invalidPath = false;
		}
	}

	cout << "Path is valid: " << startPath << "\n";
	ContainerEntity container = traverseDirs(startPath);
	list<ImageLocationEntity> imageList = container.getImageList();
	list<string> includedPathList = container.getIncludedPaths();
	list<string> badPathList = container.getBadImagePaths();

	cout << "\n";
	cout << "=============================================================================\n";
	cout << "Orphaned images:" << endl;
	cout << "=============================================================================\n";
	cout << "\n";

	list<string> excludedPaths = runComparison(includedPathList, imageList);

	for (std::string path : excludedPaths) {
		cout << path << endl;
	}

	cout << "\n";
	cout << "=============================================================================\n";
	cout << "Articles with inconsistent image refs:" << endl;
	cout << "=============================================================================\n";
	cout << "\n";

	for (std::string path : badPathList) {
		cout << path << endl;
	}

	cin.get();
}

string testPath(string startPath) {
	path p{ startPath };
	if (boost::filesystem::is_directory(p)) {
		return startPath;
	}
	else {
		return "INVALID";
	}	
}

ContainerEntity traverseDirs(string startPath) {

	std::list<string> referencedFiles;
	std::list<ImageLocationEntity> imageList;
	std:list<string> filesWithBadImageTags;

	path p{ startPath };
	recursive_directory_iterator iter(p), end;

	while (iter != end) {
		path currPath = iter->path();

		if (boost::filesystem::is_regular_file(currPath) && boost::filesystem::extension(currPath) == ".md") {
			// && currPath.filename() == "convert-training-experiment-to-scoring-experiment.md"
			// convert cwd to string, parse out file name
			// used to later append relative path from .md files image tags
			string cwd = currPath.string();
			std::reverse(cwd.begin(), cwd.end());
			cwd.erase(0, cwd.find("\\"));
			std::reverse(cwd.begin(), cwd.end());

			// stream file
			std::ifstream file(currPath.string());
			std::stringstream buffer;
			buffer << file.rdbuf();

			//convert file to string=s
			std::string s = buffer.str();

			if (s.find("![") >= 0) {
				while (s.length() > 0) {
					int tagSegmentLoc = s.find("![");

					if (tagSegmentLoc >= 0) {
						s.erase(0, s.find("!["));
						s.erase(0, s.find("]") + 1);
						char currentChar = s.at(0);

						if (currentChar != '(') {
							//inconsitent image tag format, log file name
							filesWithBadImageTags.push_back(currPath.string());
							break;
						}
						else {
							s.erase(0, s.find("(") + 1);
							std::string relPath = s.substr(0, s.find(")"));

							if (relPath.find("./") >= 0) {
								relPath.erase(0, relPath.find("/") + 1);
							}

							try {
								relPath = replaceChars(relPath);
							}
							catch (const std::bad_alloc&) {
								string bp = "bp";
							}

							if (relPath.find("#") > 0) {
								relPath = relPath.substr(0, relPath.find("#"));
							}

							string fullyQualifiedImagePath = cwd + relPath;
							referencedFiles.push_front(fullyQualifiedImagePath);
							s.erase(0, s.find(")") + 1);
						}
					}
					else {
						break;
					}
				}
			}
		}
		else if (boost::filesystem::is_regular_file(currPath) && boost::filesystem::extension(currPath) == ".png") {
			ImageLocationEntity imageEntity;
			imageEntity.setFileName(currPath.filename().string());
			imageEntity.setPath(currPath.string());

			imageList.push_front(imageEntity);
		}
		++iter;
	}

	ContainerEntity container;
	container.setImageList(imageList);
	container.setIncludedPaths(referencedFiles);
	container.setBadImagePaths(filesWithBadImageTags);

	return container;
}

std::string replaceChars(std::string raw) {
	std::list<char> charList;

	for (char& c : raw) {
		if (c == '/') {
			char newChar = '\\';
			charList.push_back(newChar);
		} 
		else {
			charList.push_back(c);
		}
	}

	std::string returnString;
	for (char c : charList) {
		returnString += c;
	}

	return returnString;
}

//returns image paths that aren't referenced in any .md files
std::list<string> runComparison(std::list<string> includedPathList, std::list<ImageLocationEntity> imageList) {
	std::list<string> excludedPaths;

	for (ImageLocationEntity imageEntity : imageList) {
		std::string path = imageEntity.getPath();

		bool existsInFiles = (std::find(includedPathList.begin(), includedPathList.end(), path) != includedPathList.end());
		if (!existsInFiles) {
			excludedPaths.push_back(path);
		}
	}
	return excludedPaths;
}
