/*
 * model.h
 *
 *  Created on: Oct 18, 2017
 *      Author: manuel
 */

#ifndef SIMPLE_MODEL_H_
#define SIMPLE_MODEL_H_

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "simple_mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

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

class SimpleModel
{
public:
    /*  Model Data */
    vector<SimpleMesh> meshes;
    string directory;
    bool gammaCorrection;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    SimpleModel(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        //cout << "processNode: " << node->mName.C_Str() << " " << node->mNumMeshes << " " << node->mNumChildren << endl;
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            //cout << "mesh: " << mesh->mName.C_Str() << endl;
            meshes.push_back(processMesh(mesh, scene));
        }
        //cout << "entre meshes e hijos" << endl;
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        //cout << "fin hijos" << endl;

    }

    SimpleMesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<SimpleVertex> vertices;
        vector<unsigned int> indices;
        std::map<glm::vec3, int, classcomp> verticesMap;
        std::map<glm::vec3, int, classcomp>::iterator it;
        std::vector<unsigned int> vertexesClasses(mesh->mNumVertices);

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            it = verticesMap.find(vector);
			if (it == verticesMap.end()) {
				SimpleVertex vertex;
				vertex.Position = vector;
	            vertex.Color = 0.0f;
	            verticesMap[vector] = vertices.size();
	            vertices.push_back(vertex);
			}
			vertexesClasses[i] = verticesMap[vector];
            //std::cout << i << ": " << vector.x << ", " << vector.y << ", "  << vector.z << std::endl;
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        //std::cout << "#faces: " << mesh->mNumFaces << std::endl;
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(vertexesClasses[face.mIndices[j]]);
        }
        // return a mesh object created from the extracted mesh data
        //std::cout << "#indices: " << indices.size() << std::endl;
        return SimpleMesh(vertices, indices);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
};


#endif /* MODEL_H_ */
