#pragma once
#include <windows.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/glaux.h>
//#include <jpeglib.h>
#include <exception>

//#pragma comment(lib, "jpeg.lib")

unsigned int LoadTexture(LPCSTR fileName);
unsigned int LoadTexture(LPCWSTR fileName);


//tImageJPG *SwiftLoadJpeg (const char *srFileName);
//void SwiftTextureJpeg(unsigned int tTexture[], LPSTR strFileName, int ID);