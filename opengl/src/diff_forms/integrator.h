/*
 * integrator.h
 *
 *  Created on: Nov 30, 2017
 *      Author: manuel
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <queue>

typedef Eigen::SparseMatrix<int,Eigen::RowMajor> SpRowInt;
typedef Eigen::SparseMatrix<bool,Eigen::RowMajor> SpRowBool;
typedef Eigen::Triplet<bool> TBool;
typedef Eigen::Triplet<int> TInt;

class Integrator
{
public:
	SpRowInt LABIMatrix;

    Integrator(const VerySimpleMesh& mesh) {
    	vertexCount = mesh.vertexCount;
        this->mesh = &mesh;
        buildLABIMatrix(0);
    }

private:
    int vertexCount;
    const VerySimpleMesh* mesh;
    vector<int> label2index; //mapea labels a índices
    vector<int> index2label; //mapea índices a labels
    vector<int> parents;	  //padre en el árbol

    //BFS desde el nodo "indexTreeRoot" mapeando los índices y los labels
    void buildLABIMatrix(int indexTreeRoot) {
    	std::vector<TInt> LABITriplets;
    	std::vector<TInt> nodes2EdgeIndexTriplets; //para armar la matriz de NxN / v[i][j] = #eje i->j
    	int edgeCount;
    	std::vector<std::vector<int> > loopEdges;

    	//Armado de las submatrices L y A
    	buildLAMatrix(indexTreeRoot, edgeCount, LABITriplets, nodes2EdgeIndexTriplets, loopEdges);

        SpRowInt nodes2EdgeIndexMatrix(vertexCount,vertexCount);
        nodes2EdgeIndexMatrix.setFromTriplets(nodes2EdgeIndexTriplets.begin(), nodes2EdgeIndexTriplets.end());
        //std::cout << node2EdgeMatrix << std::endl;

        //Armado de la submatriz I
        buildIMatrix(edgeCount, LABITriplets);

        //Armado de la submatriz B
        buildBMatrix(nodes2EdgeIndexMatrix, LABITriplets, loopEdges);

        LABIMatrix.resize(edgeCount, edgeCount);
        LABIMatrix.setFromTriplets(LABITriplets.begin(), LABITriplets.end());
    }

    void buildBMatrix(const SpRowInt& nodes2EdgeIndexMatrix, std::vector<TInt>& LABITriplets,
    					std::vector<std::vector<int> >& loopEdges) {
    	//std::cout << "ACA: " << nodes2EdgeIndexMatrix << std::endl;
    	std::vector<std::vector<int> >::iterator it;
    	int col = vertexCount - 1;
    	for(it = loopEdges.begin(); it != loopEdges.end(); it++)    {
    		int minIndex = (*it)[0];
    		int maxIndex = (*it)[1];
    		//std::cout << minIndex << " " << maxIndex << std::endl;
    		setBTriplets(col++, minIndex, maxIndex, nodes2EdgeIndexMatrix, loopEdges , LABITriplets);
    	}
    	//std::cout << parents[0] << " " << parents[1] << " " << parents[2] << " " << parents[3] << std::endl;
    }

    void setBTriplets(int col, int minBranchIndex, int maxBranchIndex, const SpRowInt& nodes2EdgeIndexMatrix,
    					std::vector<std::vector<int> >& loopEdges, std::vector<TInt>& LABITriplets) {
    	int child; int parent;
    	while (minBranchIndex != maxBranchIndex) {
    		if (minBranchIndex < maxBranchIndex) {
    			child = maxBranchIndex;
    			parent = parents[maxBranchIndex];
    			LABITriplets.push_back(TInt(nodes2EdgeIndexMatrix.coeff(parent,child), col, -1));
    			maxBranchIndex = parent;
    		} else {
    			child = minBranchIndex;
    			parent = parents[minBranchIndex];
    			LABITriplets.push_back(TInt(nodes2EdgeIndexMatrix.coeff(parent,child), col, 1));
    			minBranchIndex = parent;
    		}
    	}
    }

    void buildIMatrix(int edgeCount, std::vector<TInt>& LABITriplets) {
    	for (int i = vertexCount - 1; i < edgeCount; i++) {
    		LABITriplets.push_back(TInt(i, i, 1));
    	}
    }

    void buildLAMatrix(int indexTreeRoot, int &edgeCount,
    						std::vector<TInt>& LABITriplets, std::vector<TInt>& node2EdgeTriplets,
    						std::vector<std::vector<int> >& loopEdges) {
    	//matriz de adyacencia
    	std::vector<TBool> adjTriplets;
    	buildAdjacencyTriplets(adjTriplets);
    	SpRowBool adjMatrix(vertexCount,vertexCount);
    	adjMatrix.setFromTriplets(adjTriplets.begin(), adjTriplets.end());

    	std::queue<unsigned int> visitNodes;
    	label2index.resize(vertexCount, -1);
        index2label.resize(vertexCount, -1);
        parents.resize(vertexCount, -1);

        //inicializacion de la raíz del árbol
        unsigned int currentLabel = 0;
        parents[0] = 0; // la raiz es su propio padre
        index2label[indexTreeRoot] = currentLabel;
        label2index[currentLabel] = indexTreeRoot;
        visitNodes.push(indexTreeRoot);

        unsigned int treeEdgeCount = 0;
        unsigned int loopEdgeCount = vertexCount-1;
        while (!visitNodes.empty()) {
        	unsigned int parentIndex = visitNodes.front(); visitNodes.pop();
			for (SpRowBool::InnerIterator it(adjMatrix,parentIndex); it; ++it) {
				unsigned int childIndex = it.col();
				if (index2label[childIndex] == -1) {
					visitNodes.push(childIndex);
				}
				setParentChild(parentIndex, childIndex, currentLabel,
								treeEdgeCount, loopEdgeCount, LABITriplets, node2EdgeTriplets, loopEdges);
			}
        }
        edgeCount = loopEdgeCount;
    }

    void setParentChild(unsigned int parentIndex, unsigned int childIndex, unsigned int &currentLabel,
    				unsigned int &treeEdgeCount, unsigned int &loopEdgeCount,
    				std::vector<TInt>& LABITriplets, std::vector<TInt>& node2EdgeTriplets,
    				std::vector<std::vector<int> >& loopEdges) {
		if (index2label[childIndex] == -1) { //primera vez que aparece el nodo
	        index2label[childIndex] = ++currentLabel;
	        label2index[currentLabel] = childIndex;
			parents[currentLabel] = index2label[parentIndex];
			//agregar eje al árbol: como es la primera vez que aparece el child,
			//						el eje parent -> child es del árbol
			if (index2label[parentIndex] != 0) // no hay que considerar la columna de la raíz del árbol
				LABITriplets.push_back(TInt(treeEdgeCount, index2label[parentIndex]-1, -1));
			LABITriplets.push_back(TInt(treeEdgeCount, index2label[childIndex]-1, 1));
			node2EdgeTriplets.push_back(TInt(index2label[parentIndex], index2label[childIndex], treeEdgeCount));
			treeEdgeCount++;
		} else {
			if (index2label[parentIndex] < index2label[childIndex]) {
			// hay que agregar un loop-edge
				LABITriplets.push_back(TInt(loopEdgeCount, index2label[parentIndex]-1, -1));
				LABITriplets.push_back(TInt(loopEdgeCount, index2label[childIndex]-1, 1));
				std::vector<int> edge;
				edge.push_back(index2label[parentIndex]);
				edge.push_back(index2label[childIndex]);
				loopEdges.push_back(edge);
				loopEdgeCount++;
			}
		}
    }

    void buildAdjacencyTriplets(std::vector<TBool>& adjTriplets) {
    	vector<unsigned int> indices = mesh->indices;
     	for (unsigned int i = 0; i < indices.size(); i+= 3) {
     		int v0 = indices[i]; int v1 = indices[i+1]; int v2 = indices[i+2];
     		adjTriplets.push_back(TBool(v0,v1,true)); adjTriplets.push_back(TBool(v1,v0,true));
     		adjTriplets.push_back(TBool(v0,v2,true)); adjTriplets.push_back(TBool(v2,v0,true));
     		adjTriplets.push_back(TBool(v1,v2,true)); adjTriplets.push_back(TBool(v2,v1,true));
    	}
    }

};


#endif /* INTEGRATOR_H_ */
