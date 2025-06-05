//
// Created by Halberg, Spencer on 6/4/25.
//

#include "gtest/gtest.h"
#include "../Graph.h"
#include <fstream>

class GraphTest : public ::testing::Test
{
    protected:
    void
    SetUp() override
    {
        graph = new Graph();
    }

    void
    TearDown() override
    {
        delete graph;
    }

    Graph *graph;
};

TEST_F(GraphTest, ReadGraphTest)
{
    std::ofstream testFile("test_graph.txt");
    testFile << "child1 parent1\n";
    testFile << "child2 parent1\n";
    testFile.close();

    graph->readGraph("test_graph.txt");
    EXPECT_NE(graph->findNode("parent1"), nullptr);
    EXPECT_NE(graph->findNode("child1"), nullptr);
    EXPECT_NE(graph->findNode("child2"), nullptr);

    std::remove("test_graph.txt");
}

TEST_F(GraphTest, FindNodeTest)
{
    std::ofstream testFile("test_graph.txt");
    testFile << "child1 parent1\n";
    testFile.close();

    graph->readGraph("test_graph.txt");
    EXPECT_NE(graph->findNode("parent1"), nullptr);
    EXPECT_EQ(graph->findNode("nonexistent"), nullptr);

    std::remove("test_graph.txt");
}

TEST_F(GraphTest, IdentifyRootNodeTest)
{
    std::ofstream testFile("test_graph.txt");
    testFile << "child1 parent1\n";
    testFile << "child2 parent1\n";
    testFile.close();

    graph->readGraph("test_graph.txt");
    graph->identifyRootNode();
    EXPECT_NE(graph->getRootNode(), nullptr);
    EXPECT_EQ(graph->getRootNode()->getName(), "parent1");

    std::remove("test_graph.txt");
}

TEST_F(GraphTest, IdentifyLeafNodesTest)
{
    std::ofstream testFile("test_graph.txt");
    testFile << "child1 parent1\n";
    testFile << "child2 parent1\n";
    testFile.close();

    graph->readGraph("test_graph.txt");
    graph->identifyLeafNodes();
    EXPECT_TRUE(graph->findNode("child1")->getIsLeaf());
    EXPECT_TRUE(graph->findNode("child2")->getIsLeaf());
    EXPECT_FALSE(graph->findNode("parent1")->getIsLeaf());

    std::remove("test_graph.txt");
}

TEST_F(GraphTest, ReadProbabilityMatricesTest)
{
    std::ofstream testFile("test_graph.txt");
    testFile << "child1 parent1\n";
    testFile.close();

    std::ofstream InitProbFile("test_dir/parent1");
    InitProbFile << "0.75\t0.25\n";
    InitProbFile.close();

    std::ofstream ProbFile("test_dir/child1");
    ProbFile << "0.25\t0.75\n0.35\t0.65\n";
    ProbFile.close();

    graph->readGraph("test_graph.txt");
    graph->identifyRootNode();
    graph->readProbabilityMatrices("test_dir", 2);

    EXPECT_NE(graph->findNode("parent1")->getInitProb().size(), 0);
    EXPECT_DOUBLE_EQ(graph->findNode("parent1")->getInitProb()[0], 0.75);
    EXPECT_DOUBLE_EQ(graph->findNode("parent1")->getInitProb()[1], 0.25);

    EXPECT_DOUBLE_EQ(graph->findNode("child1")->getTransitionMatrix()->getElement(0, 0), 0.25);
    EXPECT_DOUBLE_EQ(graph->findNode("child1")->getTransitionMatrix()->getElement(0, 1), 0.75);
    EXPECT_DOUBLE_EQ(graph->findNode("child1")->getTransitionMatrix()->getElement(1, 0), 0.35);
    EXPECT_DOUBLE_EQ(graph->findNode("child1")->getTransitionMatrix()->getElement(1, 1), 0.65);

    std::remove("test_graph.txt");
    std::remove("test_dir/parent1");
}


