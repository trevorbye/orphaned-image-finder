#include <iostream>
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "LocationEntity.h"

using namespace boost::filesystem;
using namespace std;

string testPath(string startPath);
std::list<ImageLocationEntity> traverseImageLocations(string startPath);

int main() {
	string startPath;
	bool invalidPath = true;
	cout << "Scraper initialized.\n";

	while (invalidPath) {
		cout << "Enter a directory path to start recursively crawling." << endl;
		cin >> startPath;
		cin.ignore();

	    startPath = testPath(startPath);
		
		if (startPath != "INVALID") {
			invalidPath = false;
		}
	}

	cout << "Path is valid: " << startPath;
	list<ImageLocationEntity> imageList = traverseImageLocations(startPath);

	for (ImageLocationEntity loc : imageList) {
		cout << loc.getPath() << endl;
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

std::list<ImageLocationEntity> traverseImageLocations(string startPath) {

	list<ImageLocationEntity> imageList;
	path p{ startPath };
	recursive_directory_iterator iter(p), end;

	while (iter != end) {
		path currPath = iter->path();

		if (boost::filesystem::is_regular_file(currPath) && boost::filesystem::extension(currPath) == ".png") {
			ImageLocationEntity imageEntity;
			imageEntity.setFileName(currPath.filename().string());
			imageEntity.setPath(currPath.string());

			imageList.push_front(imageEntity);
		}
		++iter;
	}

	return imageList;
}
