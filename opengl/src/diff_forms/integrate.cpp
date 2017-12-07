#define MAIN
#ifdef MAIN

#include "../base/very_simple_mesh.h"

#include <Eigen/Sparse>
#include <vector>
#include "../base/very_simple_model.h"
#include "integrator.h"

int main()
{
	//VerySimpleModel model("/home/manuel/doctorado/opengl/opengltut/opengl/models/torus/torus.obj");
	//Integrator i(model.meshes[0]);

	vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	VerySimpleMesh mesh = VerySimpleMesh(indices, 4);
	Integrator i(mesh);
	std::cout << i.LABIMatrix << std::endl;
	return 0;
}

#endif



