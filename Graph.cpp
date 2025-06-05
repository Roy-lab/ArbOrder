//
// Created by Halberg, Spencer on 6/4/25.
//

#include "Graph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>

#include "ArbAssignments.h"

Graph::~Graph()
{
	for (auto &node: m_Nodes)
	{
		delete node;
	}
	m_Nodes.clear();
}


Node *
Graph::findNode(const string &name)
{
	for (const auto &node: m_Nodes)
		if (node->getName() == name)
			return node;
	return nullptr;
}

void
Graph::identifyRootNode()
{
	int root_count = 0;
	Node* root_holder = nullptr;
	for (const auto &node: m_Nodes)
	{
		if (node->getParentPointer() == nullptr)
		{
			root_count++;
			node->setIsRoot(true);
			root_holder = node;
		}else
		{
			node->setIsRoot(false);
		}
	}
	if (root_count > 1)
	{
		cerr << "More than one root node found" << endl;
		exit(1);
	}
	if (root_count == 0)
	{
		cerr << "No root node found" << endl;
		exit(1);
	}
	m_RootNode = root_holder;
}

void
Graph::identifyLeafNodes()
{
	for (const auto &node: m_Nodes)
	{
		if (node->getChildPointer()->empty())
		{
			node->setIsLeaf(true);
		}else
		{
			node->setIsLeaf(false);
		}
	}
}

Node *
Graph::getRootNode()
{
	return m_RootNode;
}



void
Graph::readGraph(const string & filename)
{
	ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	string line;
	while (getline(file, line))
	{
		stringstream ss(line);
		string child_name, parent_name;
		ss  >> child_name >> parent_name;
		if (child_name.empty() || parent_name.empty())
		{
			continue;
		}
		if (child_name == parent_name)
		{
			continue;
		}

		Node *child = findNode(child_name);
		Node *parent = findNode(parent_name);
		if (child == nullptr)
		{
			child = new Node(child_name);
			m_Nodes.insert(child);
		}
		if (parent == nullptr)
		{
			parent = new Node(parent_name);
			m_Nodes.insert(parent);
		}

		parent->addChildPointer(child);
		child->setParentPointer(parent);
	}
	file.close();
}

void
Graph::printGraph()
{
	if (m_RootNode == nullptr)
	{
		std::cerr << "No root node found" << std::endl;
		return;
	}

	std::queue<Node *> queue;
	std::set<Node *> visited;

	queue.push(m_RootNode);
	visited.insert(m_RootNode);

	while (!queue.empty())
	{
		Node *current = queue.front();
		queue.pop();

		for (Node *child: *(current->getChildPointer()))
		{
			std::cout << current->getName() << "\t" << child->getName() << std::endl;

			if (visited.find(child) == visited.end())
			{
				queue.push(child);
				visited.insert(child);
			}
		}
	}
}


void
Graph::readProbabilityMatrices(const string &indir, int k)
{
	string name;
	bool isRoot;
	string line, token;

	for (Node* N : m_Nodes)
	{
		name = N->getName();
		isRoot = N->getIsRoot();

		string filename = indir + '/' + name;
		ifstream file(filename);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}
		if (isRoot)
		{
			
			getline(file, line);
			stringstream ss(line);
			vector<double> initProb;

			while (getline(ss, token, ' '))
			{
				initProb.push_back(stod(token));
			}

			N->setInitProb(initProb);
		}else
		{
			Matrix *M = new Matrix(k);
			int row(0), col(0);
			while (getline(file, line))
			{
				stringstream ss(line);
				while (getline(ss, token, ' '))
				{
					M->setElement(row, col, stod(token));
					col++;
				}
				row++;
				col = 0;
			}
			N->setTransitionMatrix(M);
		}
	}
}

void
Graph::reorderProbabilityMatrices(ArbAssignments & arb_assignments)
{
	string parent_name, node_name;
	int parent_idx, node_idx;
	map<int, int>* parent_reorder;
	map<int, int>* node_reorder;
	Node* parent;
	Matrix *matrix = nullptr;
	
	for (Node *node: m_Nodes)
	{
		if (!node->getIsRoot())
		{
			parent = node->getParentPointer();
			parent_name = parent->getName();
			node_name = node->getName();

			parent_idx = arb_assignments.getclusterIndex(parent_name);
			node_idx = arb_assignments.getclusterIndex(node_name);

			parent_reorder = arb_assignments.get_reorder_map(parent_idx);
			node_reorder = arb_assignments.get_reorder_map(node_idx);

			matrix = node->getTransitionMatrix();

			if (matrix != nullptr)
			{
				matrix->reorderRows(parent_reorder);
				matrix->reorderCols(node_reorder);
			}else
			{
				cerr << "Matrix object not found." << endl;
			}
		}
		else //Root node 
		{
			node_name = node->getName();
			node_idx = arb_assignments.getclusterIndex(node_name);
			node_reorder = arb_assignments.get_reorder_map(node_idx);
			node->reorderInitProb(node_reorder);
		}
	}
}

void
Graph::writeProbabilityMatrices(string outdir)
{
	string name, outfile;
	bool isRoot;
	Matrix *TransMatrix;

	for (Node *node: m_Nodes)
	{
		name = node->getName();
		isRoot = node->getIsRoot();
		outfile = outdir + '/' + name;

		if (isRoot)
		{
			ofstream fout(outfile);
			if (!fout.is_open())
			{
				cout << "Failed to open file: " << outdir + '/' + name << endl;
				return;
			}

			for (int i = 0; i < node->getInitProb().size(); ++i)
			{
				fout << node->getInitProb()[i];
				if (i < node->getInitProb().size() - 1)
				{
					fout << "\t";
				}

			}
			fout << endl;
			fout.close();
		}else
		{
			TransMatrix = node->getTransitionMatrix();
			TransMatrix->writeMatrix(outfile);
		}
	}
}













