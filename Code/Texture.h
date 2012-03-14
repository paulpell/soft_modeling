
#ifndef TEXTURE_H_
#define TEXTURE_H_

//#include ".h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <fstream>
#include <iostream>

// just a quick hack, should maybe be improved :)
typedef unsigned char WORD;
typedef unsigned char DWORD;
typedef unsigned char LONG;
typedef unsigned char BYTE;

//File information header
//provides general information about the file
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD   bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

//Bitmap information header
//provides information specific to the image data
typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

//Colour palette
typedef struct tagRGBQUAD {
  BYTE  rgbBlue;
  BYTE  rgbGreen;
  BYTE  rgbRed;
  BYTE  rgbReserved;
} RGBQUAD;

class Texture {
public:
	Texture(const char * fname, int biWidth, int biHeight);

	virtual ~Texture();

//private:

	int imagewidth;
	int imageheight;

	char xData[128][128][4];

};

#endif /* TEXTURE_H_ */
