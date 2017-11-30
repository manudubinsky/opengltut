#undef MAIN
#ifdef MAIN

//============================================================================
// Name        : opengl.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

int main() {
	int width, height, nrChannels;
	unsigned char *data = stbi_load("/home/manuel/doctorado/eclipse/opengl/opengl/textures/container.jpg", &width, &height, &nrChannels, 0);
	cout << "W: " << width << " H: " << height << " channels: " << nrChannels << endl;
	return 0;
}

#endif
