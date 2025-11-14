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

	int setBest(string best);
	int setK(int k);

	int assign_features(int min_k, int max_k);
	int getclusterIndex(const string & name);
	map<int, int>* getReorderMap(int idx);
	vector<string>* getClusterSet()
	{
		return &m_cluster;
	}

	int write_assignment(string outdir);
	void writeMappedFeatureAssign(string outfile);
	void writeMappingFiles(string outdir);
	void writeSortedClusterMeans(string outfile);
	void writeClusterAssign(string outdir);

	int reorderExpressionValues(string filename, string cluster, string outdir);

	int fixUnassignedFeatures();
	int readInputExpression(string filename, string cluster);

	int reorderExpression();
	int writeReorderedExpression(string outdir);

	private:
	int m_k;
	map<int, vector<int>*> m_arb_assignment_map;
	map<string, map<string, double>*> m_expression;
	map<int, int> m_order;
	vector<string> m_cluster;
	vector<string> m_feature;

	map<int, map<int, double>*> m_cluster_means;
	map<int, map<int, int>*> m_cluster_reordering;
	multimap<int, int> m_arb_assignment;
	map<string, multimap<int, pair<string, double> > *> m_reordered_expression; //cluster -> assignment -> (gene, expression)
	string m_Best;
};



#endif //ARBASSIGNMENTS_H
