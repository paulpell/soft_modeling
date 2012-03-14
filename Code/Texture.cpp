

#include "Texture.h"

Texture::Texture(const char * fname, int biWidth, int biHeight) {

using namespace std;

	imagewidth = biWidth;
	imageheight = biHeight;

	ifstream fileBMP;
	PBITMAPFILEHEADER bmfh;
	PBITMAPINFOHEADER bmih;

	fileBMP.open(fname, ios::binary);

	//fileBMP.read(reinterpret_cast<char*>(&bmfh), sizeof(bmfh));	
	//fileBMP.read(reinterpret_cast<char*>(&bmih), sizeof(bmih));	
	char* tempa = new char;
	fileBMP.read(tempa, sizeof(bmfh));
	fileBMP.read(tempa, sizeof(bmih));

	xData [biHeight][biWidth][4];

	for(int y = 0; y < biHeight; y++){
	for(int x = 0; x < biWidth; x++) {

		char* temp = new char;
		fileBMP.read(temp , sizeof(char));
		xData[y][x][1] =  *temp;

		fileBMP.read(temp , sizeof(char));
		xData[y][x][2] =  *temp;

		fileBMP.read(temp , sizeof(char));
		xData[y][x][1] =  *temp;

		xData[x][y][3] =  255;

	}}

}


Texture::~Texture() {
	// TODO Auto-generated destructor stub
}


