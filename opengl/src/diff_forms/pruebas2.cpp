#undef MAIN
#ifdef MAIN

#include <Eigen/Sparse>
#include <vector>
#include "../base/very_simple_model.h"

typedef Eigen::SparseMatrix<bool,Eigen::RowMajor> SpRowBool;
typedef Eigen::Triplet<bool> T;

void buildProblem(std::vector<T>& coeffs, VerySimpleModel& model) {
	vector<unsigned int> indices = model.meshes[0].indices;
 	for (unsigned int i = 0; i < indices.size(); i+= 3) {
 		int v0 = indices[i]; int v1 = indices[i+1]; int v2 = indices[i+2];
 		//std::cout << v0 << " " << v1 << " " << v2 << std::endl;
 		coeffs.push_back(T(v0,v1,true)); coeffs.push_back(T(v1,v0,true));
 		coeffs.push_back(T(v0,v2,true)); coeffs.push_back(T(v2,v0,true));
 		coeffs.push_back(T(v1,v2,true)); coeffs.push_back(T(v2,v1,true));
	}
}

int main()
{
	VerySimpleModel model("/home/manuel/doctorado/opengl/opengltut/opengl/models/torus/torus.obj");
	std::vector<T> coeffs;


	buildProblem(coeffs, model);

	unsigned int N = model.meshes[0].vertexCount;
	std::cout << N << std::endl;
	SpRowBool A(N,N);

	A.setFromTriplets(coeffs.begin(), coeffs.end());
	std::cout << "nF: " << model.meshes[0].indices.size() << " nnZ: " << A.nonZeros() << std::endl;
	return 0;
}

#endif



