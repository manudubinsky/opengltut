#define MAIN
#ifdef MAIN

#include<ostream>
#include "../base/simple_model.h"

int main()
{
	VerySimpleModel ourModel("/home/manuel/doctorado/opengl/opengltut/opengl/models/ring/ring.obj");
	std::cout << ourModel.meshes.size() << std::endl;
	return 0;
}

#endif


