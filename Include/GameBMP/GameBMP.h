#ifndef GAMEBMP_H
#define GAMEBMP_H
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
class GameBMP
{
public:
static unsigned char* LoadBMP(const char*Path,int *width,int *height,std::string Message);
static unsigned char* FlipVertically(unsigned char* data,int width,int height);
static void FreeBMP(unsigned char* data);
};
#endif // GAMEBMP_H
