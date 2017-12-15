/*
 * simple_mesh.h
 *
 *  Created on: Nov 23, 2017
 *      Author: manuel
 */

#ifndef SIMPLE_MESH_H_
#define SIMPLE_MESH_H_

#include <glad/glad.h> // holds all OpenGL type declarations

#include <cmath>
#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "shader.h"
#include "../diff_forms/labi_matrix.h"

using namespace std;

struct SimpleVertex {
    // position
    glm::vec3 Position;
    float Color;
};

const int VERTEX_SIZE = 6; //x, y, z, r, g, b
const int TRIANG_POINTS = 3;

class SimpleMesh {
public:
    /*  Mesh Data  */
    vector<SimpleVertex> vertices;
    vector<unsigned int> indices;
    unsigned int VAO;
    LABIMatrix labiMatrix;

    /*  Functions  */
    // constructor
    SimpleMesh(vector<SimpleVertex> orig_vertices, vector<unsigned int> orig_indices)
    {
    	setupData(orig_vertices, orig_indices);
        //setupMesh();
    }

    // render the mesh
    void Draw(Shader shader)
    {
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    /*  Render data  */
    unsigned int VBO, EBO;
    std::map<std::string, int> verticesMap;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);

        // vertex color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, Color));


        glBindVertexArray(0);
    }

    unsigned int getVertexIndex(int p1, int p2, vector<SimpleVertex> orig_vertices) {
    	std::map<std::string,int>::iterator it;
    	if (p1 > p2) {
    		intSwap(p1, p2);
    	}
    	std::stringstream convert;
    	convert << p1 << "_" << p2;
    	std::string key = convert.str();
    	it = verticesMap.find(key);
    	if (it == verticesMap.end()) {
            SimpleVertex vertex;
            glm::vec3 vector;
            vector.x = (orig_vertices[p1].Position.x + orig_vertices[p2].Position.x) / 2;
            vector.y = (orig_vertices[p1].Position.y + orig_vertices[p2].Position.y) / 2;
            vector.z = (orig_vertices[p1].Position.z + orig_vertices[p2].Position.z) / 2;
            vertex.Position = vector;
            if (labiMatrix.isTreeEdge(p1, p2)) {
            	float edge = (float)labiMatrix.getTreeEdge(p1, p2);
            	vertex.Color = (1 - edge/(float)orig_vertices.size()) * 0.75f + 0.25f;
            } else {
            	vertex.Color = 0.0f;
            }
            vertices.push_back(vertex);
            verticesMap[key] = vertices.size() - 1;
    	}

    	return verticesMap[key];
    }

    void addLines(int p1, int p2, int midVertex) {
    	indices.push_back(p1);
    	indices.push_back(midVertex);
		indices.push_back(p2);
		indices.push_back(midVertex);
    }

    void setupData(vector<SimpleVertex> orig_vertices, vector<unsigned int> orig_indices) {
    	//std::cout << "ACA: " << orig_vertices.size() << std::endl;
    	//std::cout << "ACA: " << orig_indices.size() << std::endl;
    	labiMatrix.set(orig_vertices.size(), orig_indices);
    	std::cout << "ACA!: " << labiMatrix.LABI.size() << " (" <<
    			labiMatrix.LABI.rows() << "x" << labiMatrix.LABI.cols() << ") nnZ:" <<
    			labiMatrix.LABI.nonZeros() << std::endl;

    	//vertices originales
    	for (unsigned int i = 0; i < orig_vertices.size(); i++) {
			vertices.push_back(orig_vertices[i]);
    	}

//    	std::cout << "<0>" << std::endl;
/*
    	unsigned int midVertex;
    	for (unsigned int i = 0; i < orig_indices.size(); i+=3) {
    		int p1 = orig_indices[i];
    		int p2 = orig_indices[i+1];
    		int p3 = orig_indices[i+2];
    		//p1 <-> p2
    		midVertex = getVertexIndex(p1, p2, orig_vertices);
    		addLines(p1, p2, midVertex);

    		//p1 <-> p3
    		midVertex = getVertexIndex(p1, p3, orig_vertices);
    		addLines(p1, p3, midVertex);

    		//p2 <-> p3
    		midVertex = getVertexIndex(p2, p3, orig_vertices);
    		addLines(p2, p3, midVertex);
    		//std::cout << p1 << "," << p2 << "," << p3 << std::endl;
    	}
*/
//    	std::cout << "<1>" << std::endl;
    }

};




#endif /* SIMPLE_MESH_H_ */
