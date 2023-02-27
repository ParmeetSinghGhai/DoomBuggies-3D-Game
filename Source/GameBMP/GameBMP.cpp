#define _CRT_SECURE_NO_DEPRECATE
#include <GameBMP/GameBMP.h>
/*************************************************
THIS FUNCTIONS ACCEPTS THE PATH TO A BMP FILE AND OUTPUTS:
1) ARRAY OF BYTE DATA CONTAINED IN THAT BMP FILE
2) THE WDITH OF THAT BMP FILE
3) THE HEIGHT OF  THAT BMP FILE

THE BITMAP NEEDS TO BE IN 24 BIT FORMAT ( 1 BYTES FOR EACH RED, GREEN AND BLUE COLOR ) AND SHOULD NOT CONTAIN ALPHA VALUES

THE PROCEDURE FOLLOWED FOR READING THE DATA OFF OF THE BMP FILE:
1) THE FUNCTION FIRST READS THE FIRST 54 BYTES OF THE BMP FILE. THESE 54 BYTES REPRESENT THE HEADER OF THE BMP FILE
   WHICH CONTAINS INFORMATION LIKE DATA POSITION, IMAGESIZE, WIDTH , HEIGHT ETC
2) IF THE BYTES READ DO NOT EQUAL TO 54 BYTES THEN THE FUNCTION RETURNS NULL POINTER.
3) IF THE FIRST BYTE OF THE HEADER DOES NOT EQUAL TO CHARACTER 'B' OR 'M' THEN THE FUNCTION RETURNS NULL POINTER.
4) READ THE BYTE AT OFFSET DEC 10 / HEX 0A FROM THE HEADER TO GET THE DATA POSITION INTO THE ACTUAL BITMAP DATA
5) READ THE BYTE AT OFFSET DEC 18 / HEX 12 FROM THE HEADER TO GET THE WIDTH
6) READ THE BYTE AT OFFSET DEC 22 / HEX 16 FROM THE HEADER TO GET THE HEIGHT
7) READ THE BYTE AT OFFSET DEC 34 / HEX 22 FROM THE HEADER TO GET THE IMAGESIZE
8) READ THE BITMAP DATA STARTING THE 54TH BYTE HENCE SKIPPING THE HEADER

THE PIXEL DATA IN A BMP FILE IS STORED BACKWARDS I.E IT STORES THE LOWER ROWS FIRST AND THEN HIGHER ROWS LAST

    H=4  *  | | |  *  | | |  *  | | |  *  | | |  *  | | |  *
    H=3  *  | | |  *  | | |  *  | | |  *  | | |  *  | | |  *
    H=2  *  | | |  *  | | |  *  | | |  *  | | |  *  | | |  *
    H=1  *  | | |  *  | | |  *  | | |  *  | | |  *  | | |  *
    H=0  *  | | |  *  | | |  *  | | |  *  | | |  *  | | |  *
        W=0 r g b  W1 r g b  W3 r g b  W2 r g b  W4 r g b  W5

        A 24 BIT 5 X 5 BITMAP IMAGE EXAMPLE

A PIXEL AT A ROW CAN BE RETRIEVED USING THE FOLLOWING FORMULAE
PIXEL COLOR = 3 BYTES OFFSET * (HEIGHT  * WIDTH + WIDTH) + BYTE INDEX], WHERE BYTE INDEX=0 FOR RED,BYTE INDEX=1 FOR GREEN AND BYTE INDEX=2 FOR BLUE

FOR EXAMPLE THE 3RD PIXEL AT ROW 2 (H=2,W=2) CAN BE RETRIVED AS FOLLOWS:
PIXEL BYTE OFFSET = 3 * [2 * 5 + 2]
	          = 3 * [12]
	          = 36 BYTES

THIS PIXEL'S RED COLOR IS FOUND AT 36 BYTES + 0 BYTE
THIS PIXEL'S GREEN COLOR IS FOUND AT 36 BYTES + 1 BYTE
THIS PIXEL'S BLUE COLOR IS FOUND AT 36 BYTES + 2 BYTE
**************************************************/
unsigned char* GameBMP::LoadBMP(const char*Path,int *width,int *height,std::string Message)
{
unsigned char header[54]; // EACH BMP FILE BEGINS WITH A 54 BYTE HEADER
unsigned int dataPos;     // IT REPRESENTS THE POSITION IN FILE WHERE THE ACTUAL PIXEL DATA BEGINS
unsigned int imageSize;   // IMAGESIZE IS CALCULATED AS = WIDTH *  HEIGHT * 3 BYTES. EACH PIXEL IS MADE UP OF 3 BYTES / 24 BITS WHERE EACH BYTE REPRESENTS THE
                          // RED, GREEN AND BLUE COLOR VALUE

FILE * file = fopen(Path,"rb");
if (!file)
{
    std::cout<<"ERROR::EXCEPTION IN GAMEBMP LOAD MODULE. COULD NOT FIND FILE::"<<Path<<" ::Info::"<<Message<<"\n";
    return nullptr;
}
if ( fread(header, 1, 54, file)!=54 )
{
    std::cout<<"ERROR::EXCEPTION IN GAMEBMP LOAD MODULE. BMP FILE FORMAT INCORRECT"<<" ::Info::"<<Message<<"\n";
    return nullptr;
}
if ( header[0]!='B' || header[1]!='M' )
{
    std::cout<<"ERROR::EXCEPTION IN GAMEBMP LOAD MODULE. BMP FILE FORMAT INCORRECT"<<" ::Info::"<<Message<<"\n";
    return nullptr;
}

dataPos    = *(int*)&(header[0x0A]);
imageSize  = *(int*)&(header[0x22]);
(*width)   = *(int*)&(header[0x12]);
(*height)  = *(int*)&(header[0x16]);
if (imageSize==0)
imageSize=(*width)*(*height)*3;
if (dataPos==0)
dataPos=54;


int bytecount;
unsigned char *data = new unsigned char[imageSize];
bytecount=fread(data,1,imageSize,file);
std::cout<<"SUCCESS:"<<bytecount<<" BYTES READ INTO IMAGE DATA"<<" ::Info::"<<Message<<"\n";
fclose(file);
return data;
}

/*************************************************
THIS FUNCTION FLIPS A BMP FILE VERTICALLY BY SAVING THE LOWEST ROW PIXEL DATA LAST
AND THE HIGHEST ROW PIXEL DATA FIRST
*************************************************/
unsigned char* GameBMP::FlipVertically(unsigned char* data,int width,int height)
{
unsigned char *temp = new unsigned char[width * height * 3];
unsigned char *original=temp;
unsigned char pix[3];
for(int h=0;h<height;h++)
{
    for(int w=0;w<width;w++)
    {
      pix[0]=data[3 * (h * width + w) + 0];
      pix[1]=data[3 * (h * width + w) + 1];
      pix[2]=data[3 * (h * width + w) + 2];

      temp[3 * ((height-1-h) * width + w) + 0]=pix[2];
      temp[3 * ((height-1-h) * width + w) + 1]=pix[1];
      temp[3 * ((height-1-h) * width + w) + 2]=pix[0];
    }
}
free(data);
return original;
}

void GameBMP::FreeBMP(unsigned char* data)
{
free(data);
}

