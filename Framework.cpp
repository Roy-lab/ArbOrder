//
// Created by Halberg, Spencer on 6/3/25.
//

#include "Framework.h"
#include "ArbAssignments.h"
#include <iostream>
#include <string>

#include "Graph.h"

using namespace std;



int
main(int argc, char *argv[])
{
	if (argc != 7)
	{
		std::cerr << "Usage: " << argv[0] << "<arb_output_dir> <tree_file> <min_k> <k> <best> <outdir> " << std::endl;
		return 1;
	}

	string arb_output_dir = argv[1];
	string tree_file = argv[2];
	int min_k = atoi(argv[3]);
	int k = atoi(argv[4]);
	string best = argv[5];
	string output_dir = argv[6];

	string arb_assignments = arb_output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	string cluster_mean_file = arb_output_dir + "/clustermeans.txt";


	ArbAssignments assigner;
	assigner.setBest(best);
	assigner.read_arb_assignments(arb_assignments);
	assigner.read_mean_value(cluster_mean_file);
	assigner.assign_features(min_k - 1, k - 1);

	string mapped_feature_assign = output_dir + "/allcelltypes_clusterassign_lca_brk.txt";
	assigner.writeMappedFeatureAssign(mapped_feature_assign);
	assigner.writeMappingFiles(output_dir);
	assigner.writeClusterAssign(output_dir);

	string assignment_sub = output_dir + "/feature_assign_" + to_string(min_k) + "_" + to_string(k) + "/";
	assigner.write_assignment(assignment_sub);
	string mean_file = output_dir + "/clustermeans.txt";
	assigner.writeSortedClusterMeans(mean_file);

	Graph graph;
	graph.readGraph(tree_file);
	graph.identifyRootNode();
	graph.identifyLeafNodes();
	graph.readProbabilityMatrices(arb_output_dir, k);
	graph.reorderProbabilityMatrices(assigner);
	graph.writeProbabilityMatrices(output_dir);

	return 0;
}
