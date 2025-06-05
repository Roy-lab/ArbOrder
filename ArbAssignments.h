//
// Created by Halberg, Spencer on 6/3/25.
//

#ifndef ARBASSIGNMENTS_H
#define ARBASSIGNMENTS_H

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

class ArbAssignments {
	public:
	~ArbAssignments();

	int read_arb_assignments(string filename);

	int read_order(string filename);

	int read_mean_value(string filename);

	int assign_features(int min_k, int max_k);
	int getclusterIndex(const string & name);
	map<int, int>* get_reorder_map(int idx);

	int write_assignment(string outdir);
	void writeMappedFeatureAssign(string outfile);
	void writeMappingFiles(string outdir);
	void writeSortedClusterMeans(string outfile);






	private:
	map<int, vector<int>*> m_arb_assignment_map;
	map<int, int> m_order;
	vector<string> m_cluster;
	vector<string> m_feature;
	map<int, map<int, double>*> m_cluster_means;
	map<int, map<int, int>*> m_cluster_reordering;
	multimap<int, int> m_arb_assignment;
};



#endif //ARBASSIGNMENTS_H
