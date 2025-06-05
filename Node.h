//
// Created by Halberg, Spencer on 6/4/25.
//

#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Matrix.h"


using namespace std;


class Node {
public:
    Node();
    Node(const string& name);
    Node(const string& name, Node* parent);
    Node(const string& name, Node* parent, vector<double> &initProb);

    ~Node();


    void setIsRoot(bool isRoot) { m_isRoot = isRoot; }
    bool getIsRoot() const { return m_isRoot; }

    void setIsLeaf(bool isLeaf) { m_isLeaf = isLeaf; }
    bool getIsLeaf() const { return m_isLeaf; }

    void setName(string name) { m_name = name; }
    string getName() const { return m_name; }

    void setParentPointer(Node* parent) { m_parentPointer = parent; }
    Node* getParentPointer() const { return m_parentPointer; }

    void addChildPointer(Node* child) {  m_childPointers.push_back(child);}
    vector<Node*>* getChildPointer() { return &m_childPointers;}

    void setTransitionMatrix(Matrix* transitionMatrix) { m_transitionMatrix = transitionMatrix; }
    Matrix* getTransitionMatrix() const { return m_transitionMatrix; }

    void setInitProb(vector<double> &initProb) { m_initProb = initProb; }
    void setInitProb(int idx, double val)
    {
        m_initProb[idx] = val;
    }

    void reorderInitProb(map<int, int> *);


    vector<double> getInitProb() const { return m_initProb;}

private:
    bool m_isRoot;
    bool m_isLeaf;
    string m_name;
    Node* m_parentPointer;
    vector<Node*> m_childPointers;
    Matrix* m_transitionMatrix;
    vector<double> m_initProb;
};



#endif //NODE_H
