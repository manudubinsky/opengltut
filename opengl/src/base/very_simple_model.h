/*
 * model.h
 *
 *  Created on: Oct 18, 2017
 *      Author: manuel
 */

#ifndef VERY_SIMPLE_MODEL_H_
#define VERY_SIMPLE_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <glm/detail/type_vec.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "very_simple_mesh.h"

using namespace std;


class VerySimpleModel
{
public:
    vector<VerySimpleMesh> meshes;
    string directory;

    VerySimpleModel(string const &path)
    {
        loadModel(path);
    }

private:
    void loadModel(string const &path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    VerySimpleMesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
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
	            verticesMap[vector] = 1;
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
        return VerySimpleMesh(mesh->mNumVertices, indices);
    }
};


#endif /* MODEL_H_ */
