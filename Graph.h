//
// Created by Halberg, Spencer on 6/4/25.
//

#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Matrix.h"
#include "ArbAssignments.h"
#include <string>
#include <set>

using namespace std;

class Graph {
	public:
	~Graph();

	void readGraph(const string &filename);
	void readProbabilityMatrices(const string &indir, int k);
	void printGraph();
	void writeProbabilityMatrices(string outdir);


	void identifyRootNode();
	void identifyLeafNodes();

	Node* findNode(const string& name);
	Node* getRootNode();

	void reorderProbabilityMatrices(ArbAssignments & arb_assignments);

	private:
	Node* m_RootNode;
	set<Node*> m_Nodes;
};

#endif //GRAPH_H
