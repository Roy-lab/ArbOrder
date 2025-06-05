//
// Created by Halberg, Spencer on 6/4/25.
//

#include "Node.h"

Node::Node() {
	m_isRoot = false;
	m_isLeaf = false;
	string m_name;
	m_parentPointer = nullptr;
	m_childPointers = vector<Node*>();
	m_transitionMatrix = nullptr;
	m_initProb = vector<double>();
}

Node::~Node() {
	if (m_transitionMatrix != nullptr) {
		delete m_transitionMatrix;
	}
}

Node::Node(const string& name) : Node() {
	m_name = name;
}

Node::Node(const string& name, Node* parent) : Node() {
	m_name = name;
	m_parentPointer = parent;
}

Node::Node(const string& name, Node* parent, vector<double> &initProb) : Node(){
	m_name = name;
	m_isRoot = true;
	
	for(auto &p: initProb) {
		m_initProb.push_back(p);
	}
}





void
Node::reorderInitProb(map<int, int> *reorderMap)
{
	if (reorderMap == nullptr || m_initProb.empty())
	{
		return;
	}

	vector<double> reorderedProb(m_initProb.size(), 0.0);
	for (size_t i = 0; i < m_initProb.size(); ++i)
	{
		if (reorderMap->find(i) != reorderMap->end())
		{
			reorderedProb[(*reorderMap)[i]] = m_initProb[i];
		}
	}
	m_initProb = reorderedProb;
}	