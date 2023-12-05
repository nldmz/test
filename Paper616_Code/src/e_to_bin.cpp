#include "utility.h"
using namespace std;

struct weighted_neighbor_node {
	int vertex;
	int weight;
};

struct Edge
{
	int u;
	int v;
	int w;
	float weight;
};

bool inc_weight(const Edge& a, const Edge& b) {
	return a.weight < b.weight;
}
bool dec_w(const Edge& a, const Edge& b) {
	return a.w > b.w;
}

bool dec_1(const Edge& a, const Edge& b) {
	return a.u < b.u;
}
bool dec_2(const Edge& a, const Edge& b) {
	return a.v < b.v;
}
void quicksort_v1(vector< weighted_neighbor_node>& nodes, int first, int last, vector<int>& degree_v2) {
	int lower = first + 1;
	int upper = last;
	swap(nodes[first], nodes[(first + last) / 2]);
	int bound = degree_v2[nodes[first].vertex];
	while (lower <= upper)
	{
		while (lower <= upper && degree_v2[nodes[lower].vertex] > bound)
			lower++;
		while (lower <= upper && degree_v2[nodes[upper].vertex] < bound)
			upper--;
		if (lower < upper)
			swap(nodes[lower++], nodes[upper--]);
		else lower++;
	}
	swap(nodes[upper], nodes[first]);
	if (first < upper - 1)
		quicksort_v1(nodes, first, upper - 1, degree_v2);
	if (upper + 1 < last)
		quicksort_v1(nodes, upper + 1, last, degree_v2);
}
void quicksort_v2(vector< weighted_neighbor_node>& nodes, int first, int last, vector<int>& degree_v1) {
	int lower = first + 1;
	int upper = last;
	swap(nodes[first], nodes[(first + last) / 2]);
	int bound = degree_v1[nodes[first].vertex];
	while (lower <= upper)
	{
		while (lower <= upper && degree_v1[nodes[lower].vertex] > bound)
			lower++;
		while (lower <= upper && degree_v1[nodes[upper].vertex] < bound)
			upper--;
		if (lower < upper)
			swap(nodes[lower++], nodes[upper--]);
		else lower++;
	}
	swap(nodes[upper], nodes[first]);
	if (first < upper - 1)
		quicksort_v2(nodes, first, upper - 1, degree_v1);
	if (upper + 1 < last)
		quicksort_v2(nodes, upper + 1, last, degree_v1);
}

void save_graph_to_bin(string path, string str) {

	FILE* graphEF = fopen((path+str).c_str(), "r");
	cout<<endl << str << endl;
	int r;
	int u, v;
	float w;
	int t;
	vector<vector<weighted_neighbor_node>> neighbor_v1;
	vector<vector<weighted_neighbor_node>> neighbor_v2;
	vector<Edge> edges;
	vector<int> weight_ID;

	vector<int> degree_v1;
	vector<int> degree_v2;
	int n1 = 0, n2 = 0, m = 0;
	int max_degree_v1 = 0, max_degree_v2 = 0;
	

	while ((r = fscanf(graphEF, "%d %d %f", &u, &v, &w)) != EOF)
	{
			// fprintf(stderr, "%d, %d\n", u, v);
			if (r != 3)
			{
				fprintf(stderr, "Bad file format: u v incorrect\n");
				exit(1);
			}
			Edge e = { u,v,0,w };
			edges.push_back(e);
			
	}
	
	fclose(graphEF);

	m = edges.size();

	//renumber vertex
	cout << "reorder vertex u..." << endl;
	sort(edges.begin(), edges.end(), dec_1);
	int temp1 = edges[0].u;
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i].u != temp1)
			n1++;
		temp1 = edges[i].u;
		edges[i].u = n1;
		
	}
	
	n1++;
	cout << "reorder vertex v..." << endl;
	sort(edges.begin(), edges.end(), dec_2);
	int temp2 = edges[0].v;
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i].v != temp2)
			n2++;
		temp2 = edges[i].v;
		edges[i].v = n2;
		
	}
	n2++;

	degree_v1.resize(n1);
	degree_v2.resize(n2);
	neighbor_v1.resize(n1);
	neighbor_v2.resize(n2);

	sort(edges.begin(), edges.end(), dec_2);
	vector<Edge> valid_edges;
	vector<vector<weighted_neighbor_node>> neighbor_temp;
	neighbor_temp.resize(n1);
	// deleting repetitive edges
	cout << "counting edges..." << endl;
	for (auto e = edges.begin(); e != edges.end(); e++) {
		int u = e->u, v = e->v, weight_id = e->w;
		if (neighbor_temp[u].size() > 0 && neighbor_temp[u][neighbor_temp[u].size() - 1].vertex == v) {
			continue;
		}
		valid_edges.push_back(*e);
		weighted_neighbor_node wu = { v, weight_id };
		neighbor_temp[u].push_back(wu);
	}

	m = valid_edges.size();
	sort(valid_edges.begin(), valid_edges.end(), inc_weight);
	for (int i = 0; i < valid_edges.size(); i++) {
		//valid_edges[i].w = i + 1;
		//weight_ID.push_back(valid_edges[i].w);  //original_weight = weight_ID[edges[i].w-1]
		valid_edges[i].w = i + 1;
	}

	cout << "init neighbor..." << endl;
	for (auto e = valid_edges.begin(); e != valid_edges.end(); e++) {
		int u = e->u, v = e->v, weight = e->w;
		weighted_neighbor_node wu = { v, weight };
		neighbor_v1[u].push_back(wu);
		weighted_neighbor_node wv = { u, weight };
		neighbor_v2[v].push_back(wv);
	}
	
	cout << "init degree..." << endl;
	
	for (int i = 0; i < neighbor_v1.size(); i++) {
		degree_v1[i] = neighbor_v1[i].size();
		if (degree_v1[i] > max_degree_v1) max_degree_v1 = degree_v1[i];
	}
	for (int i = 0; i < neighbor_v2.size(); i++) {
		degree_v2[i] = neighbor_v2[i].size();
		if (degree_v2[i] > max_degree_v2) max_degree_v2 = degree_v2[i];
	}
	cout << "sorting neighbor..." << endl;
	for (int i = 0; i < n1; ++i)
	{
		neighbor_v1[i].shrink_to_fit();
		if (neighbor_v1[i].size() == 0) continue;
		quicksort_v1(neighbor_v1[i], 0, neighbor_v1[i].size() - 1, degree_v2);
	}
	for (int i = 0; i < n2; ++i)
	{
		neighbor_v2[i].shrink_to_fit();
		if (neighbor_v2[i].size() == 0) continue;
		quicksort_v2(neighbor_v2[i], 0, neighbor_v2[i].size() - 1, degree_v1);
	}
	sort(valid_edges.begin(), valid_edges.end(), dec_w);
	printf("Saving binary...\n");
	FILE* fout = fopen((path + "graph.bin").c_str(), "wb");
	fwrite(&n1, sizeof(int), 1, fout);
	fwrite(&n2, sizeof(int), 1, fout);
	fwrite(&m, sizeof(int), 1, fout);
	//int num_weight = weight_ID.size();
	//fwrite(&num_weight, sizeof(int), 1, fout);
	fwrite(&max_degree_v1, sizeof(int), 1, fout);
	fwrite(&max_degree_v2, sizeof(int), 1, fout);
	fwrite(&degree_v1[0], sizeof(int), n1, fout);
	fwrite(&degree_v2[0], sizeof(int), n2, fout);
	
	// add edges
	fwrite(&valid_edges[0], sizeof(Edge), m, fout);
	//fwrite(&weight_ID[0], sizeof(int), m, fout);
	for (int i = 0; i < n1; i++) {
		fwrite(&neighbor_v1[i][0], sizeof(weighted_neighbor_node), degree_v1[i], fout);
	}
	for (int i = 0; i < n2; i++) {
		fwrite(&neighbor_v2[i][0], sizeof(weighted_neighbor_node), degree_v2[i], fout);
	}
	fclose(fout);
	printf("Created binary file, n1 = %d, n2 = %d, m = %d\n", n1, n2, m);
	//delete[] adj;  delete[] dat;
	//delete[] con; delete[] deg; delete[] oid; delete[] nid; delete[] nbr;
}

