/*
 * simple_mesh.h
 *
 *  Created on: Nov 23, 2017
 *      Author: manuel
 */

#ifndef VERY_SIMPLE_MESH_H_
#define VERY_SIMPLE_MESH_H_

#include <glad/glad.h> // holds all OpenGL type declarations

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

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

    /*  Functions  */
    // constructor
    SimpleMesh(vector<SimpleVertex> orig_vertices, vector<unsigned int> orig_indices)
    {
    	setupData(orig_vertices, orig_indices);
    	//std::cout << "termine" << std::endl;
        /*
        std::cout << vertices.size() << " " << indices.size() << std::endl;
        if (vertices.size() < 100) {
        	for (int i = 0; i < vertices.size(); i++) {
        		std::cout << i << ": v[x,y,z]: " << vertices[i].Position.x << ","
        										<< vertices[i].Position.y << ","
        										<< vertices[i].Position.z << std::endl;
        	}
        	for (int i = 0; i < indices.size(); i+=3) {
        		std:cout << "f[" << i << "]: " << indices[i] << ","
        				 	 	 	 	 	 	 << indices[i+1] << ","
        				 	 	 	 	 	 	 << indices[i+2] << std::endl;
        	}
        }
		*/
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader shader)
    {
    	std::map<std::string,int>::iterator it;
        float color = sin(glfwGetTime()) / 2.0f + 0.5f;
        for (it = verticesMap.begin(); it != verticesMap.end(); ++it) {
        	int element = it->second;
        	vertices[element].Color = color;
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), &vertices[0], GL_STATIC_DRAW);

        // draw mesh
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
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
            vertex.Color = 0.5f;
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

    	//vertices originales
    	for (unsigned int i = 0; i < orig_vertices.size(); i++) {
			vertices.push_back(orig_vertices[i]);
    	}


    	unsigned int midVertex;
    	for (unsigned int i = 0; i < orig_indices.size(); i+=3) {
    		int p1 = orig_indices[i];
    		int p2 = orig_indices[i+1];
    		int p3 = orig_indices[i+2];
    		//p1 <-> p2
    		midVertex = getVertexIndex(p1, p2, orig_vertices);
    		addLines(p1, p2, midVertex);
    		/*
    		if (orig_vertices.size() < 100) {
    			std::cout << p1 << "," << p2 << "," << midVertex << std::endl;
    		}*/

    		//p1 <-> p3
    		midVertex = getVertexIndex(p1, p3, orig_vertices);
    		addLines(p1, p3, midVertex);

    		//p2 <-> p3
    		midVertex = getVertexIndex(p2, p3, orig_vertices);
    		addLines(p2, p3, midVertex);
    	}
    }

};




#endif /* SIMPLE_MESH_H_ */
