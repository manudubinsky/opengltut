/*
 * solver.h
 *
 *  Created on: Dec 11, 2017
 *      Author: manuel
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
#include "labi_matrix.h"

class LABISolver
{
public:
	LABIMatrix matrix;
	Eigen::VectorXf b;
	Eigen::VectorXf x;

	LABISolver(const LABIMatrix& mat) {
		matrix = mat;
		x.resize(matrix.laplacianMatrix.cols());
		for (int i = 0; i < matrix.getVertexCount() - 1; i++) {
			x.coeffRef(i) = 100.0f;
		}

		b = matrix.laplacianMatrix*x;
    }

	void solve() {
		Eigen::SimplicialCholesky<SpRowFloat> chol(matrix.laplacianMatrix);  // performs a Cholesky factorization of A
		  //std::cout << chol.info();
		Eigen::VectorXf y = chol.solve(b);
		Eigen::VectorXf b2;
		//std::cout << "<0> norm(x-y)/norm(x): " << 1-(x-y).norm()/x.norm() << std::endl;


		Eigen::VectorXf diff;
		for (int i = 0; i < 3; i++) {
			b2 = matrix.laplacianMatrix * y;
			diff = b - b2;
			std::cout << "<" << i << "> norm(diff)/norm(b): " << 1-diff.norm()/b.norm() << std::endl;
			y = y + chol.solve(diff);
			//std::cout << "<" << i+1 << "> norm(x-y)/norm(x): " << 1-(x-y).norm()/x.norm() << std::endl;
		}
	}

private:
};


#endif /* SOLVER_H_ */
