#include "utility.h"
#include "abcoreDecom.h"
using namespace std;


void compute_a_b_core(BiGraph& g, int alpha, int beta) {
	vector<vid_t> left_vertices_to_be_peeled;
	vector<vid_t> right_vertices_to_be_peeled;
	bool stop = true;
	for (vid_t u = 0; u < g.num_v1; u++) {
		if (!g.left_delete[u]) {
			if (g.degree_v1[u] < alpha) {
				left_vertices_to_be_peeled.push_back(u);
			}
		}
	}
	for (vid_t v = 0; v < g.num_v2; v++) {
		if (!g.right_delete[v]) {
			if (g.degree_v2[v] < beta) {
				right_vertices_to_be_peeled.push_back(v);
			}
		}
	}
	while (!left_vertices_to_be_peeled.empty() || !right_vertices_to_be_peeled.empty()) {
		// peel left
		int oo_ = left_vertices_to_be_peeled.size();
		for (int j = 0; j < oo_; j++) {
			vid_t u = left_vertices_to_be_peeled[j];
			if (g.left_delete[u]) continue;
			vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v1[u];
			int ss = tmp_neigh_.size();
			for (int k = 0; k < ss; k++) {
				vid_t v = tmp_neigh_[k].vertex;
				if (g.right_delete[v]) continue;
				int dd_ = --g.degree_v2[v];
				if (dd_ == 0) {
					// core part
					g.right_delete[v] = true;
				}
				if (dd_ == beta - 1) {
					right_vertices_to_be_peeled.push_back(v);
				}
			}
			g.degree_v1[u] = 0;
			g.left_delete[u] = true;
		}
		left_vertices_to_be_peeled.clear();
		// peel right
		oo_ = right_vertices_to_be_peeled.size();
		for (int j = 0; j < oo_; j++) {
			vid_t v = right_vertices_to_be_peeled[j];
			if (g.right_delete[v]) continue;
			vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v2[v];
			int ss = tmp_neigh_.size();
			for (int k = 0; k < ss; k++) {
				vid_t u = tmp_neigh_[k].vertex;
				if (g.left_delete[u]) continue;
				int dd_ = --g.degree_v1[u];
				if (dd_ == 0) {
					g.left_delete[u] = true;
				}
				if (dd_ == alpha - 1) {
					left_vertices_to_be_peeled.push_back(u);
				}
			}
			g.degree_v2[v] = 0;
			g.right_delete[v] = true;
		}
		right_vertices_to_be_peeled.clear();
	}
	int community_left_num=0, community_right_num=0;
	g.community_left_set.clear();
	g.community_right_set.clear();
	for (vid_t u = 0; u < g.num_v1; u++) {
		if (!g.left_delete[u]) {
			cout << u << " ";
			community_left_num++;
			g.community_left_set.push_back(u);
		}
    }
    cout << endl;
    for (vid_t u = 0; u < g.num_v2; u++) {
		if (!g.right_delete[u]) {
			cout << u << " ";
			community_right_num++;
			g.community_right_set.push_back(u);
		}
	}
	g.community_left_set.resize(community_left_num);
	g.community_right_set.resize(community_right_num);
	cout << endl;
}