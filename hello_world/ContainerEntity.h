#include "LocationEntity.h"
#include <string>
#include <list>

using namespace std;

class ContainerEntity {
public:
	ContainerEntity();
	std::list<string> getIncludedPaths();
	void setIncludedPaths(std::list<string> includedPaths);

	std::list<ImageLocationEntity> getImageList();
	void setImageList(std::list<ImageLocationEntity> imageList);

	std::list<string> getBadImagePaths();
	void setBadImagePaths(std::list<string> badPaths);
private:
	std::list<string> mIncludedPathList;
	std::list<ImageLocationEntity> mImageList;
	std::list<string> mBadImageTagsList;
};
