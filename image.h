//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html

#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

//Represents an image
class Image {
public:
	Image(char* _pixels, int _width, int _height);
	~Image();

	char* pixels;
	int width;
	int height;
};

//Reads a bitmap image from file.
Image* loadBMP(const char* filename);

#endif
