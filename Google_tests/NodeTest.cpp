//
// Created by Halberg, Spencer on 6/4/25.
//

#include "gtest/gtest.h"
#include "../Node.h"

class NodeTest : public ::testing::Test
{
    protected:
    void
    SetUp() override
    {
    }

    void
    TearDown() override
    {
    }
};

TEST_F(NodeTest, ConstructorAndName)
{
    Node node("TestNode");
    string name = node.getName();
    EXPECT_EQ(name, "TestNode");
}

TEST_F(NodeTest, ParentPointerOperations)
{
    Node parent("Parent");
    Node child("Child");
    child.setParentPointer(&parent);
    EXPECT_EQ(child.getParentPointer(), &parent);
}

TEST_F(NodeTest, ChildPointerOperations)
{
    Node parent("Parent");
    Node *child1 = new Node("Child1");
    Node *child2 = new Node("Child2");

    parent.addChildPointer(child1);
    parent.addChildPointer(child2);

    auto children = parent.getChildPointer();
    EXPECT_EQ(children->size(), 2);
    EXPECT_TRUE(find(children->begin(), children->end(), child1) != children->end());
    EXPECT_TRUE(find(children->begin(), children->end(), child2) != children->end());

    delete child1;
    delete child2;
}

TEST_F(NodeTest, RootAndLeafFlags)
{
    Node node("TestNode");

    node.setIsRoot(true);
    EXPECT_TRUE(node.getIsRoot());

    node.setIsLeaf(true);
    EXPECT_TRUE(node.getIsLeaf());
}

TEST_F(NodeTest, TransitionMatrix)
{
    Node node("TestNode");
    Matrix *matrix = new Matrix(2);
    matrix->setElement(0, 0, 0.5);

    node.setTransitionMatrix(matrix);
    EXPECT_EQ(node.getTransitionMatrix(), matrix);
    EXPECT_DOUBLE_EQ(node.getTransitionMatrix()->getElement(0, 0), 0.5);
}

TEST_F(NodeTest, InitialProbability)
{
    Node node("TestNode");
    std::vector<double> initProb = {0.3, 0.7};

    node.setInitProb(initProb);
    EXPECT_EQ(node.getInitProb(), initProb);
}

TEST_F(NodeTest, ReorderInitialProbability)
{
    Node node("TestNode");
    std::vector<double> initProb = {0.3, 0.7};
    node.setInitProb(initProb);

    std::map<int, int> reorderMap = {{0, 1}, {1, 0}};
    node.reorderInitProb(&reorderMap);

    std::vector<double> expectedProb = {0.7, 0.3};
    EXPECT_EQ(node.getInitProb(), expectedProb);
}


