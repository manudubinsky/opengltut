
#undef MAIN
#ifdef MAIN

#include "labi_solver.h"

#include <Eigen/Sparse>
#include <vector>
#include "labi_matrix.h"
#include "../base/types.h"
#include "../base/simple_mesh.h"
#include "../base/simple_model.h"


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


/*
//	VerySimpleMesh mesh = VerySimpleMesh(indices, 7);
	LABIMatrix mat(7, indices);
	// std::cout << mat.laplacianMatrix.nonZeros() << std::endl << mat.laplacianMatrix << std::endl;
	LABISolver s(mat);
	s.solve();
*/
//	std::cout << i.LABIMatrix.nonZeros() << std::endl;
//	std::cout << i.LABIMatrix << std::endl;



	//SimpleModel model("/home/manuel/doctorado/opengl/opengltut/opengl/models/torus/torus.obj");
	SimpleModel model("/home/manuel/doctorado/opengl/opengltut/opengl/models/dragon/dragon.ply");

	LABIMatrix labiMatrix = model.meshes[0].labiMatrix;
	std::cout << labiMatrix.laplacianMatrix.size() << " (" <<
			labiMatrix.laplacianMatrix.rows() << "x" << labiMatrix.laplacianMatrix.cols() << ") nnZ:" <<
			labiMatrix.laplacianMatrix.nonZeros() << std::endl;
	LABISolver s(labiMatrix);
	s.solve();
	return 0;
}

#endif



