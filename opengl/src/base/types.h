/*
 * types.h
 *
 *  Created on: Dec 11, 2017
 *      Author: manuel
 */

#ifndef TYPES_H_
#define TYPES_H_


#include <Eigen/Sparse>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef Eigen::SparseMatrix<int,Eigen::RowMajor> SpRowInt;
typedef Eigen::SparseMatrix<float> SpRowFloat;
typedef Eigen::SparseVector<float> SpVecRowFloat;
typedef Eigen::SparseMatrix<bool,Eigen::RowMajor> SpRowBool;
typedef Eigen::Triplet<bool> TBool;
typedef Eigen::Triplet<int> TInt;
typedef Eigen::Triplet<float> TFloat;

struct classcomp {
    bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
    {
        if(lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y) ||
        		(lhs.x == rhs.x && lhs.y == rhs.y && lhs.z < rhs.z))
            return true;
        else
            return false;
    }
};


#endif /* TYPES_H_ */
