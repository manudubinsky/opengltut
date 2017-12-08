#undef MAIN
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
/*	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
*/
/*
//Ejemplo 1
	indices.push_back(0); indices.push_back(1); indices.push_back(0);
	indices.push_back(0); indices.push_back(2); indices.push_back(0);
	indices.push_back(1); indices.push_back(3); indices.push_back(1);
	indices.push_back(2); indices.push_back(4); indices.push_back(2);
	indices.push_back(2); indices.push_back(5); indices.push_back(2);
	indices.push_back(3); indices.push_back(4); indices.push_back(3);
	indices.push_back(3); indices.push_back(6); indices.push_back(3);
	indices.push_back(3); indices.push_back(7); indices.push_back(3);
	indices.push_back(4); indices.push_back(8); indices.push_back(4);
	indices.push_back(7); indices.push_back(8); indices.push_back(7);
*/

//Ejemplo 2
	indices.push_back(0); indices.push_back(1); indices.push_back(0);
	indices.push_back(0); indices.push_back(2); indices.push_back(0);
	indices.push_back(1); indices.push_back(3); indices.push_back(1);
	indices.push_back(2); indices.push_back(4); indices.push_back(2);
	indices.push_back(3); indices.push_back(4); indices.push_back(3);
	indices.push_back(3); indices.push_back(5); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(4);
	indices.push_back(4); indices.push_back(6); indices.push_back(4);
	indices.push_back(5); indices.push_back(6); indices.push_back(5);



//	VerySimpleMesh mesh = VerySimpleMesh(indices, 7);
	Integrator i(7, indices);
	std::cout << i.LABIMatrix.nonZeros() << std::endl;
	std::cout << i.LABIMatrix << std::endl;
	return 0;
}

#endif



