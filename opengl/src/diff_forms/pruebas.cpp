#undef MAIN
#ifdef MAIN

#include <Eigen/Sparse>
#include <vector>
#include <queue>
#include "../base/very_simple_model.h"

typedef Eigen::SparseMatrix<bool> SpColBool;
typedef Eigen::SparseMatrix<bool,Eigen::RowMajor> SpRowBool;
typedef Eigen::Triplet<bool> T;

int main()
{
/*
	VerySimpleModel ourModel("/home/manuel/doctorado/opengl/opengltut/opengl/models/torus/torus.obj");
	std::cout << ourModel.meshes.size() << std::endl;
*/
/*
	std::vector<T> coeffs;
	coeffs.push_back(T(0,0,true)); coeffs.push_back(T(0,1,true)); coeffs.push_back(T(0,1,true));  coeffs.push_back(T(2,2,true));
	SpRowBool A(3,3);
	A.setFromTriplets(coeffs.begin(), coeffs.end());
	for (int k=0; k<A.outerSize(); ++k) {
		std::cout <<"fila: "  << k  << std::endl;
		for (SpRowBool::InnerIterator it(A,k); it; ++it)
		{
		  std::cout << "\trow: " << it.row() << " col: "  << it.col() <<
				  " index: " << it.index() << " value: " << it.value() << std::endl;
		}
	}
	//std::cout << A << std::endl;
*/
/*
	vector<int> v;
	v.resize(10);
	std::cout << v[0] << " " << v[9] << std::endl;
	//for (std::vector<int>::const_iterator i = v.begin(); i != v.end(); ++i)
	//    std::cout << *i << std::endl;
*/
/*
	std::queue<unsigned int> q;
	q.push(1); q.push(2); q.push(3); q.push(4);
	while (!q.empty()) {
		std::cout << q.front() << std::endl;
		q.pop();
	}
*/

	std::vector<T> coeffs;
	coeffs.push_back(T(0,0,true)); coeffs.push_back(T(0,1,true)); coeffs.push_back(T(0,1,true));  coeffs.push_back(T(2,2,true));
	SpRowBool A(3,3);
	A.setFromTriplets(coeffs.begin(), coeffs.end());
	//std::vector<T> coeffs2;
	//coeffs2.push_back(T(2,0,true)); coeffs.push_back(T(2,1,true));
	//A.setFromTriplets(coeffs2.begin(), coeffs2.end());
	std::cout << "A(0,1): " << A.coeff(0,1) << std::endl;
	for (int k=0; k<A.outerSize(); ++k) {
		std::cout <<"fila: "  << k  << std::endl;
		for (SpRowBool::InnerIterator it(A,k); it; ++it)
		{
		  std::cout << "\trow: " << it.row() << " col: "  << it.col() <<
				  " index: " << it.index() << " value: " << it.value() << std::endl;
		}
	}

	return 0;
}

#endif
