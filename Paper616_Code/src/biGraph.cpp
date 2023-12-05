#include "biGraph.h"

using namespace std;

BiGraph::BiGraph(string dir, bool bin)
{
    printf("Loading graph.bin...\n");
    FILE* fin = fopen((dir + "graph.bin").c_str(), "rb");
	fread(&num_v1, sizeof(int), 1, fin);
	fread(&num_v2, sizeof(int), 1, fin);
	fread(&num_edges, sizeof(int), 1, fin);
	fread(&v1_max_degree, sizeof(int), 1, fin);
	fread(&v2_max_degree, sizeof(int), 1, fin);

	degree_v1.resize(num_v1);
	degree_v2.resize(num_v2);
	edges.resize(num_edges);
	neighbor_v1.resize(num_v1);
	neighbor_v2.resize(num_v2);
	//weight_ID.resize(num_edges);
	fread(&degree_v1[0], sizeof(int), num_v1, fin);
	fread(&degree_v2[0], sizeof(int), num_v2, fin);
	fread(&edges[0], sizeof(Edge), num_edges, fin);
	//fread(&weight_ID[0], sizeof(int), num_edges, fin);
	cout << "n1: " << num_v1 << " n2: " << num_v2 << " m: " << num_edges << endl;
	for (int i = 0; i < num_v1; i++) {
		neighbor_v1[i].resize(degree_v1[i]);
		fread(&neighbor_v1[i][0], sizeof(weighted_neighbor_node), degree_v1[i], fin);
	}
	for (int i = 0; i < num_v2; i++) {
		neighbor_v2[i].resize(degree_v2[i]);
		fread(&neighbor_v2[i][0], sizeof(weighted_neighbor_node), degree_v2[i], fin);
	}
	fclose(fin);

	alpha_max_ = 0;
	for (int i = 0; i < num_v1; i++) {
		if (degree_v1[i] > alpha_max_)
			alpha_max_ = degree_v1[i];
	}

	beta_max_ = 0;
	for (int i = 0; i < num_v2; i++) {
		if (degree_v2[i] > beta_max_)
			beta_max_ = degree_v2[i];
	}

}

void BiGraph::init(unsigned int num1, unsigned int num2, unsigned int n_edges)
{
	num_v1 = num1;
	num_v2 = num2;
	num_edges = n_edges;
	
	// neighbor_v1.resize(num_v1);
	// neighbor_v2.resize(num_v2);
	
	// degree_v1.resize(num_v1);
	// degree_v2.resize(num_v2);

	// fill_n(degree_v1.begin(), num_v1, 0);
	// fill_n(degree_v2.begin(), num_v2, 0);

	left_delete.resize(num_v1);
	right_delete.resize(num_v2);

	fill_n(left_delete.begin(), num_v1, 0);
	fill_n(right_delete.begin(), num_v2, 0);

	// fill_n(left_marked.begin(), num_v1, 0);
	// fill_n(right_marked.begin(), num_v2, 0);

	// fill_n(community_left_set.begin(), num_v1, 0);
	// fill_n(community_right_set.begin(), num_v2, 0);

}