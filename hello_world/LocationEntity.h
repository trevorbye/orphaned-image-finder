#include <iostream>
#include <string>

using namespace std;

class ImageLocationEntity {
public:
	ImageLocationEntity();
	string getFileName();
	void setFileName(string fileName);
	string getPath();
	void setPath(string path);
private:
	string mfileName;
	string mpath;
};