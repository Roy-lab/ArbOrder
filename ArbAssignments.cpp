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
		for (int i = 0; i < keys.size(); ++i)
		{
			(*handle_reorder)[keys[sorted_idx[i]]] = i ;
		}
		m_cluster_reordering[cluster_idx] = handle_reorder;
	}

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
ArbAssignments::get_reorder_map(int idx)
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
			reorder_assign = (*get_reorder_map(j))[init_assign];
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
		map<int, int>* handle = get_reorder_map(i);
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


