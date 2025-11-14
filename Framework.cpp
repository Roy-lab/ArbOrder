//
// Created by Halberg, Spencer on 6/3/25.
//

#include "Framework.h"
#include "ArbAssignments.h"
#include <iostream>
#include <string>
#include <filesystem>

#include "Graph.h"

using namespace std;



int
main(int argc, char *argv[])
{
	string arb_output_dir;
	string tree_file;
	string config_file;
	int min_k;
	int k;
	bool fix_unassigned = false;
	string best;
	string output_dir;


	//Initialize input parameters
	if (argc == 9)
	{
		arb_output_dir = argv[1];
		config_file = argv[2];
		tree_file = argv[3];
		min_k = atoi(argv[4]);
		k = atoi(argv[5]);
		best = argv[6];
		fix_unassigned = (atoi(argv[7]) == 1);
		output_dir = argv[8];
	}else if (argc == 8)
	{
		arb_output_dir = argv[1];
		config_file = argv[2];
		tree_file = argv[3];
		k = atoi(argv[4]);
		best = argv[5];
		fix_unassigned = (atoi(argv[6]) == 1);
		output_dir = argv[7];
	}else
	{
		std::cerr << "Usage: " << argv[0] << " <arb_output_dir> <config_file> <tree_file> <min_k> <k> <best> <fix_unassigned[0/1]> <outdir> " << std::endl;
		std::cerr << "Usage: " << argv[0] << " <arb_output_dir> <config_file> <tree_file> <k> <best> <fix_unassigned[0/1]> <outdir>" << std::endl;
		return 1;
	}

	//Identify files in the arboretum output directory
	string arb_assignments = arb_output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	string cluster_mean_file = arb_output_dir + "/clustermeans.txt";

	//Read in input data
	ArbAssignments assigner;
	assigner.setBest(best);
	assigner.setK(k);
	assigner.read_arb_assignments(arb_assignments);
	assigner.read_mean_value(cluster_mean_file);

	//Read in input expression
	ifstream fin(config_file);
	if (!fin.is_open())
	{
		cerr << "Error: Could not open file " << config_file << endl;
		return 1;
	}

	string line;
	stringstream ss;
	string cluster;
	string cluster_init_file;
	string cluster_expression_file;
	while (getline(fin, line))
	{
		ss.clear();
		ss.str(line);
		ss >> cluster >> cluster_init_file >> cluster_expression_file;
		assigner.readInputExpression(cluster_expression_file, cluster);
	}

	//If the fix_unassigned is passed, we will go through assignments and identify any -1 with have data. if they have data we will
	//assign them to the cluster with closest mean, usually the one with the highest expression.
	if (fix_unassigned)
	{
		assigner.fixUnassignedFeatures();
	}

	string mapped_feature_assign = output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	assigner.writeMappedFeatureAssign(mapped_feature_assign);
	assigner.writeMappingFiles(output_dir);
	assigner.writeClusterAssign(output_dir);

	if (argc == 9)
	{
		assigner.assign_features(min_k - 1, k - 1);
		string assignment_sub = output_dir + "/feature_assign_" + to_string(min_k) + "_" + to_string(k) + "/";
		assigner.write_assignment(assignment_sub);
	}

	string mean_file = output_dir + "/clustermeans.txt";
	assigner.writeSortedClusterMeans(mean_file);

	Graph graph;
	graph.readGraph(tree_file);
	graph.identifyRootNode();
	graph.identifyLeafNodes();
	graph.readProbabilityMatrices(arb_output_dir, k);
	graph.reorderProbabilityMatrices(assigner);
	graph.writeProbabilityMatrices(output_dir);

	// Right Expression Tables
	assigner.reorderExpression();
	assigner.writeReorderedExpression(output_dir);
	return 0;
}