#ifndef IMAGEPNG_H
#define IMAGEPNG_H

#include <string>

struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

class Imagepng {
public:
	enum Type {RGB, RGBA};
	Imagepng(const std::string& filename, Type t, int width, int height);
	~Imagepng();
	Color& operator()(int i);
	Color* operator[](int i);
	void save();

private:
	Type type_;
	int width_;
	int height_;
	unsigned char* rgb_;
	unsigned char* rgba_;
	Color* color_;

	std::string filename_;
	FILE* fp_;
};

#endif