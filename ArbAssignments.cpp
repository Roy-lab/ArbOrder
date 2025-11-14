//
// Created by Halberg, Spencer on 6/3/25.
//

#include "ArbAssignments.h"

ArbAssignments::~ArbAssignments()
{
	for (auto &pair: m_arb_assignment_map)
	{
		delete pair.second;
	}
	m_arb_assignment_map.clear();

	for (auto &pair: m_cluster_means)
	{
		delete pair.second;
	}
	m_cluster_means.clear();

	for (auto &pair: m_expression)
	{
		delete pair.second;
	}
	m_expression.clear();
}

int
ArbAssignments::read_arb_assignments(std::string filename)
{
	// Open the input file
	ifstream infile(filename);
	if (!infile.is_open())
	{
		cerr << "Failed to open file: " << filename << std::endl;
		return 1;
	}

	string line;
	int line_c = 0;

	while (getline(infile, line))
	{
		stringstream ss(line);
		string token;
		vector<string> tokens;

		// Parse the line into tokens
		while (getline(ss, token, '\t'))
		{
			tokens.push_back(token);
		}

		// Handle the header row (first line)
		if (line_c == 0)
		{
			line_c++;
			// Skip "Gene" and extract cluster names
			for (int i = 1; i < tokens.size(); ++i)
			{
				m_cluster.push_back(tokens[i]);
			}
			continue;
		}

		if (!tokens.empty()){

			
			// First column is the gene name
			string gene = tokens[0];
			// Skip if the gene name contains "Dummy"
			if (gene.find("Dummy") != string::npos)
			{
				continue;
			}


			size_t pos = gene.find('_');
			if (pos != string::npos) {
				gene = gene.substr(pos + 1);
			}

			m_feature.push_back(gene);

			//initialize map
			vector<int>* handle = new vector<int>;
			m_arb_assignment_map[line_c - 1] = handle;

			// Remaining columns are the assignments
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				handle->push_back(stoi(tokens[i]));
			}
		}
		line_c++;
	}

	// Close the file
	infile.close();
	return 0;
}

int
ArbAssignments::read_order(string filename)
{
	ifstream infile(filename);
	string line;
	getline(infile, line);

	stringstream ss(line);
	string token;
	int order;

	int index = 0;
	// Parse the line into tokens
	while (getline(ss, token, '\t'))
	{
		order = stoi(token);
		m_order[index] = order;
		index ++;
	}

	infile.close();
	return 0;
}

int
ArbAssignments::assign_features(int min_k, int max_k)
{
	int init_assign;
	map<int, int>* cluster_remap;
	int remap_assign
	;

	for (int i = 0; i < m_feature.size(); ++i)
	{
		for (int j = 0; j < m_cluster.size(); j++)
		{
			init_assign = m_arb_assignment_map[i]->at(j);
			cluster_remap = m_cluster_reordering[j];
			remap_assign = (*cluster_remap)[init_assign];

			if (remap_assign >= min_k && remap_assign <= max_k)
			{
				m_arb_assignment.insert(pair<int, int>(j, i));
			}
		}
	}
	return 0;
}


int
ArbAssignments::write_assignment(string outdir)
{
	int cluster_idx;
	int feature_idx;
	multimap<int, int>::iterator start_it, end_it, it;
	string outfile;
	ofstream fout;

	for (int j = 0; j < m_cluster.size(); j++)
	{
		outfile = outdir + '/' + m_cluster[j] + ".txt";
		fout.open(outfile);
		if (!fout.is_open())
		{
			cout << "Failed to open file: " << outfile << endl;
			return 1;
		}

		start_it = m_arb_assignment.lower_bound(j);
		end_it = m_arb_assignment.upper_bound(j);

		for (it = start_it; it != end_it; ++it)
		{
			cluster_idx = it->first;
			feature_idx = it->second;
			fout << m_feature[feature_idx] << "\t" << m_cluster[cluster_idx]  << endl;
		}
		fout.close();
	}
	return 0;
}


int
ArbAssignments::read_mean_value(string filename)
{
	string line, token, cluster;
	int k_cluster_idx, cluster_idx, tok_idx;
	double mean_value;
	map<int, double>* handle;

	auto cluster_it = m_cluster_means.find(cluster_idx);

	ifstream infile(filename);
	if (!infile.is_open())
	{
		cerr << "Failed to open file: " << filename << std::endl;
		return 1;
	}

	while (getline(infile, line))
	{
		stringstream ss(line);
		tok_idx = 0;
		while (getline(ss, token, '\t'))
		{
			if (tok_idx == 0)
			{
				size_t pos = token.find('_');
				if (pos != string::npos) {
					cluster = token.substr(0, pos);
					cluster_idx = std::find(m_cluster.begin(), m_cluster.end(), cluster) - m_cluster.begin();
					k_cluster_idx = stoi(token.substr(pos + 1));

					cluster_it = m_cluster_means.find(cluster_idx);
					if (cluster_it == m_cluster_means.end())
					{
						handle = new map<int, double>;
						m_cluster_means[cluster_idx] = handle;
					}else
					{
						handle = cluster_it->second;
					}
				}
			}else if (tok_idx == 1)
			{
				mean_value = stod(token);
				(*handle)[k_cluster_idx] = mean_value;
			}
			tok_idx++;
		}
	}
	infile.close();

	//Generate cluster reordering map
	for (auto &pair: m_cluster_means)
	{
		cluster_idx = pair.first;
		handle = pair.second;

		vector<int> keys;
		for (auto &p: *handle)
		{
			keys.push_back(p.first);
		}

		vector<int> sorted_idx(keys.size());
		for (int i = 0; i < keys.size(); ++i)
		{
			sorted_idx[i] = i;
		}

		// sort by mean
		sort(sorted_idx.begin(), sorted_idx.end(), [&](int a, int b) {
			return handle->at(keys[a]) < handle->at(keys[b]);
		});

		// generate reordering map
		map<int, int>* handle_reorder = new map<int, int>;
		(*handle_reorder)[-1] = -1;
		(*handle_reorder)[-2] = -2;
		for (int i = 0; i < keys.size(); ++i)
		{
			(*handle_reorder)[keys[sorted_idx[i]]] = i ;
		}
		m_cluster_reordering[cluster_idx] = handle_reorder;
	}

	return 0;
}

int
ArbAssignments::setBest(string best)
{
	m_Best = best;
	return 0;
}

int ArbAssignments::setK(int k)
{
	m_k = k;
	return 0;
}

int
ArbAssignments::getclusterIndex(const string & name)
{
	vector<string>::iterator iter;
	int idx;
	iter = find(m_cluster.begin(), m_cluster.end(), name);
	if (iter != m_cluster.end())
	{
		idx = iter - m_cluster.begin();
		return idx;
	}else
	{
		cerr << "Cluster " + name + " not found in remap set" << endl;
		return -1;
	}

}


map<int, int>*
ArbAssignments::getReorderMap(int idx)
{
	if (m_cluster_reordering.find(idx) == m_cluster_reordering.end())
	{
		cerr << "cluster not found in reordering map" << endl;
		return nullptr;
	}
	return m_cluster_reordering[idx];
}


void
ArbAssignments::writeMappedFeatureAssign(string outfile)
{

	int init_assign;
	int reorder_assign;


	ofstream fout(outfile);
	if (!fout.is_open())
	{
		cout << "Failed to open file: " << outfile << endl;
		return;
	}

	fout << "Gene";
	for (string cluster: m_cluster)
	{
		fout << "\t" + cluster;
	}
	fout << endl;

	for (int i = 0; i < m_feature.size(); ++i)
	{
		stringstream ss;
		ss << m_feature[i];
		for (int j = 0; j < m_cluster.size(); j++)
		{
			init_assign = m_arb_assignment_map[i]->at(j);
			reorder_assign = (*getReorderMap(j))[init_assign];
			ss << "\t" << reorder_assign;
		}
		ss << endl;
		fout << ss.str();
	}
	fout.close();
}


void ArbAssignments::writeMappingFiles(string outdir)
{
	for (int i=0; i < m_cluster.size(); ++i)
	{
		string cluster = m_cluster[i];
		ofstream fout(outdir + '/' + cluster + "_reorder.txt");
		if (!fout.is_open())
		{
			cout << "Failed to open file: " << outdir + '/' + cluster + "_reorder.txt" << endl;
			return;
		}

		fout << "Init\tReorder" << endl;
		map<int, int>* handle = getReorderMap(i);
		for (auto &pair: *handle)
		{
			fout << pair.first << "\t" << pair.second << endl;
		}
		fout.close();
	}
}

void ArbAssignments::writeSortedClusterMeans(string outfile)
{
	ofstream fout(outfile);
	if (!fout.is_open())
	{
		cout << "Failed to open file: " << outfile << endl;
		return;
	}

	for (int i=0; i < m_cluster.size(); ++i)
	{
		string cluster_id = m_cluster[i];
		map<int, double>* cluster_means = m_cluster_means[i];
		map<int, int>* cluster_order_remap = m_cluster_reordering[i];

		// Sort in new order.
		vector<pair<int, int> > sorted_pairs;
		for (const auto &r: *cluster_order_remap)
		{
			sorted_pairs.push_back(r);
		}
		sort(sorted_pairs.begin(), sorted_pairs.end(),
		     [](const auto &a, const auto &b) { return a.second < b.second; });

		for (const auto &r: sorted_pairs)
		{
			int old_id = r.first;
			int new_id = r.second;
			double mean = (*cluster_means)[old_id];
			fout << cluster_id << "_" << new_id << "\t" << mean << endl;
		}
	}
	fout.close();
}

void ArbAssignments::writeClusterAssign(string outdir)
{
	string cluster;
	string feature;

	string outfile1;
	string outfile2;

	int init_assign;
	int reorder_assign;

	for (int i = 0; i < m_cluster.size(); i++)
	{
		cluster = m_cluster[i];
		outfile1 = outdir + '/' + cluster + "_clusterassign.txt";
		outfile2 = outdir + '/' + cluster + "_speciesspecnames_clusterassign.txt";

		ofstream fout1(outfile1);
		ofstream fout2(outfile2);

		fout1 << "Gene\t" << cluster << endl;
		fout2 << "Gene\t" << cluster << endl;

		for (int j = 0; j < m_feature.size(); j++)
		{
			feature = m_feature[j];
			init_assign = m_arb_assignment_map[j]->at(i);
			reorder_assign = (*getReorderMap(i))[init_assign];
			if (reorder_assign < 0) // Skip unassigned or missing!
			{
				fout1 << m_Best  << '_' << feature << "\t" << reorder_assign << endl;
				fout2 << cluster << '_' << feature << "\t" << reorder_assign << endl;
			}
		}

		fout1.close();
		fout2.close();
	}
}


/**
 * @brief Reads and orders expression values into a multimap, splitting groups by "Dummy" lines.
 * 
 * The file is expected to have the following format:
 * Gene    Value
 * C2_Sobic.001G001300  0.029983
 * "Dummy" lines signify a new group, incrementing the group index in the multimap.
 * 
 * @param filename The path to the file containing expression values.
 * @return 0 on success, -1 on failure.
 */
int ArbAssignments::reorderExpressionValues(string filename, string cluster, string outfile) {
	ifstream infile(filename);
	if (!infile.is_open()) {
		cerr << "Error: Could not open file " << filename << endl;
		return -1; // File could not be opened
	}

	string line;
	multimap<int, pair<string, double>> temp_expression_map; // Temporary structure
	int group_index = 0; // Start group index at zero
	getline(infile, line); // skip header
	while (getline(infile, line)) {
		stringstream ss(line);
		string gene;
		string second_col;

		// Read the gene name and second column as string first
		if (!(ss >> gene >> second_col)) {
			cerr << "Warning: Skipping invalid line: " << line << endl;
			continue;
		}

		if (gene.substr(0, 5) == "Dummy" || second_col == "-100") {
			group_index++; // Increment group index on encountering "Dummy"
			continue;
		}


		double value;
		try {
			value = stod(second_col);
		} catch (const exception &e) {
			cerr << "Warning: Could not parse value for gene " << gene << ": " << second_col << endl;
			continue;
		}
		// Add the gene and value to the multimap
		temp_expression_map.insert(make_pair(group_index, make_pair(gene, value)));
	}

	infile.close();

	// Debug/logging output
	//cout << "Successfully read and grouped expression values." << endl;
	//cout << "Number of groups: " << (group_index) << endl;

	// Process the temporary map into m_cluster_reordering here (if required)
	int cluster_idx = getclusterIndex(cluster);
	map<int, int> *reorder_map = getReorderMap(cluster_idx);
	map<int, int> inverted_map;

	for (auto &map_item: *reorder_map)
	{
		inverted_map[map_item.second] = map_item.first;
	}

	ofstream fout(outfile);
	if (!fout.is_open()) {
		cerr << "Error: Could not open file " << outfile << endl;
		return -1; // File could not be opened
	}
	fout << "Gene\tCol1" << endl;
	for (auto &i_map_item:inverted_map)
	{
		int key_cluster = i_map_item.first;
		int exp_cluster = i_map_item.second;
		if (exp_cluster >= 0)
		{
			for (auto exp_iter = temp_expression_map.lower_bound(exp_cluster); exp_iter !=  temp_expression_map.upper_bound(exp_cluster); exp_iter++)
			{
				pair<string, double> exp_val = exp_iter->second;
				fout << exp_val.first << "\t" << exp_val.second << endl;
			}
			fout << "Dummy" <<key_cluster << "\t" << -100 << endl;
		}
	}
	fout.close();
	return 0;

}

int ArbAssignments::fixUnassignedFeatures()
{
	string feature;
	string cluster;
	vector<int>* assign_vec;
	map<int, double>* cluster_means;
	int feature_idx;
	int assign;
	double exp;
	double dist;
	double min_dist;
	int nearest_cluster;

	for (auto &feature_pair:m_arb_assignment_map)
	{
		feature_idx = feature_pair.first;
		assign_vec = feature_pair.second;

		for (int i=0; i < assign_vec->size(); ++i)
		{
			assign = (*assign_vec)[i];
			if (assign < 0)
			{
				feature = m_feature[feature_idx];
				cluster = m_cluster[i];
				exp = m_expression[feature]->at(cluster);

				cluster_means = m_cluster_means[i];
				min_dist = std::numeric_limits<double>::max();
				for (pair <int, double> p: *cluster_means)
				{
					dist = abs(exp - p.second);
					if (dist < min_dist)
					{
						min_dist = dist;
						nearest_cluster = p.first;
					}

				}
				(*assign_vec)[i] = nearest_cluster;
			}
		}
	}
	return 0;
}


int ArbAssignments::readInputExpression(string filename, string cluster)
{
	ifstream infile(filename);
	if (!infile.is_open())
	{
		cerr << "Error: Could not open file " << filename << endl;
	}

	string line;
	stringstream instream;
	string gene;
	double exp;
	while (getline(infile, line))
	{
		instream.clear();
		instream.str(line);
		instream >> gene >> exp;

		gene = gene.substr(gene.find('_') + 1);

		if (m_expression.find(gene) == m_expression.end())
		{
			m_expression[gene] = new map<string, double>;
		}
		(*m_expression[gene])[cluster] = exp;
	}
	return 0;
}


int ArbAssignments::reorderExpression()
{
	string gene;
	string cluster;
	int old_assign;
	int new_assign;
	double exp;
	for (int i=0; i < m_cluster.size(); i ++)
	{
		cluster = m_cluster[i];
		if (m_reordered_expression.find(cluster) == m_reordered_expression.end())
		{
			m_reordered_expression[cluster] = new multimap<int, pair<string, double>>;
		}
		multimap<int,pair<string, double>>* cluster_assigns = m_reordered_expression[cluster];
		for (int j=0; j < m_feature.size(); j++)
		{
			gene = m_feature[j];
			old_assign = m_arb_assignment_map[j]->at(i);
			new_assign = (*getReorderMap(i))[old_assign];
			exp = m_expression[gene]->at(cluster);
			if (new_assign > 0)
			{
				cluster_assigns->insert(make_pair(new_assign, make_pair(gene, exp)));
			}
		}
	}
	return 0;
}

int ArbAssignments::writeReorderedExpression(string outdir)
{
	string filename;
	ofstream fout;
	for (string cluster: m_cluster)
	{
		filename = outdir + "/" + cluster + "_exprtab.txt";
		fout.open(filename);
		if (!fout.is_open())
		{
			cerr << "Error: Could not open file " << filename << " for writing." <<endl;
			return -1;
		}
		fout << "Gene\tCol1" << endl;
		multimap<int, pair<string, double>>* cluster_multi_map = m_reordered_expression[cluster];
		for (int i = 0; i < m_k; i++)
		{
			auto iter = cluster_multi_map->lower_bound(i);
			auto end = cluster_multi_map->upper_bound(i);
			while (iter != end)
			{
				pair<string, double> exp_val = iter->second;
				fout << exp_val.first << "\t" << exp_val.second << endl;
				iter++;
			}
			fout << "Dummy" << i << "\t" << -100 << endl;
		}
	}
	return 0;
}







