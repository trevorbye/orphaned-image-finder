#include "LocationEntity.h"
#include <string>

ImageLocationEntity::ImageLocationEntity() {
}

string ImageLocationEntity::getFileName() {
	return mfileName;
}

void ImageLocationEntity::setFileName(string fileName) {
	mfileName = fileName;
}

string ImageLocationEntity::getPath() {
	return mpath;
}

void ImageLocationEntity::setPath(string path) {
	mpath = path;
}