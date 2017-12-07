/*
 * model.h
 *
 *  Created on: Oct 18, 2017
 *      Author: manuel
 */

#ifndef VERY_SIMPLE_MODEL_H_
#define VERY_SIMPLE_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
        vector<unsigned int> indices;
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        return VerySimpleMesh(indices, mesh->mNumVertices);
    }
};


#endif /* MODEL_H_ */
