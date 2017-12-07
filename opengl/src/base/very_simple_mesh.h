/*
 * simple_mesh.h
 *
 *  Created on: Nov 23, 2017
 *      Author: manuel
 */

#ifndef VERY_SIMPLE_MESH_H_
#define VERY_SIMPLE_MESH_H_


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int VERTEX_SIZE = 6; //x, y, z, r, g, b
const int TRIANG_POINTS = 3;

class VerySimpleMesh {
public:
	unsigned int vertexCount;
    vector<unsigned int> indices;

    /*  Functions  */
    // constructor
    VerySimpleMesh(vector<unsigned int> indices, unsigned int vertexCount)
    {
    	this->indices = indices;
    	this->vertexCount = vertexCount;
    }

};




#endif /* SIMPLE_MESH_H_ */
