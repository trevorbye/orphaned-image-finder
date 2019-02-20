#include "ContainerEntity.h"
#include <string>
#include <list>

ContainerEntity::ContainerEntity() {
}

std::list<string> ContainerEntity::getIncludedPaths() {
	return mIncludedPathList;
}

void ContainerEntity::setIncludedPaths(std::list<string> includedPaths) {
	mIncludedPathList = includedPaths;
}

std::list<ImageLocationEntity> ContainerEntity::getImageList() {
	return mImageList;
}

void ContainerEntity::setImageList(std::list<ImageLocationEntity> imageList) {
	mImageList = imageList;
}

std::list<string> ContainerEntity::getBadImagePaths() {
	return mBadImageTagsList;
}

void ContainerEntity::setBadImagePaths(std::list<string> badPaths) {
	mBadImageTagsList = badPaths;
};