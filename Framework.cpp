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
	int min_k;
	int k;
	string best;
	string output_dir;


	if (argc == 7)
	{
		arb_output_dir = argv[1];
		tree_file = argv[2];
		min_k = atoi(argv[3]);
		k = atoi(argv[4]);
		best = argv[5];
		output_dir = argv[6];
	}else if (argc == 6)
	{
		arb_output_dir = argv[1];
		tree_file = argv[2];
		k = atoi(argv[3]);
		best = argv[4];
		output_dir = argv[5];
	}else
	{
		std::cerr << "Usage: " << argv[0] << " <arb_output_dir> <tree_file> <min_k> <k> <best> <outdir> " << std::endl;
		std::cerr << "Usage: " << argv[0] << " <arb_output_dir> <tree_file> <k> <best> <outdir>" << std::endl;
		return 1;
	}


	string arb_assignments = arb_output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	string cluster_mean_file = arb_output_dir + "/clustermeans.txt";


	ArbAssignments assigner;


	assigner.setBest(best);
	assigner.read_arb_assignments(arb_assignments);
	assigner.read_mean_value(cluster_mean_file);
	string mapped_feature_assign = output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	assigner.writeMappedFeatureAssign(mapped_feature_assign);
	assigner.writeMappingFiles(output_dir);
	assigner.writeClusterAssign(output_dir);


	if (argc == 7)
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

// Process expression table files
	for (string &cluster_id: *assigner.getClusterSet())
	{
		string infile_name = arb_output_dir + "/" + cluster_id + "_exprtab.txt";
		string basename = infile_name.substr(infile_name.find_last_of("/\\") + 1);
		assigner.reorderExpressionValues(infile_name, cluster_id, output_dir + "/" + basename);
	}


	return 0;
}