#ifndef __BIGRAPH_H
#define __BIGRAPH_H
#define _CRT_SECURE_NO_WARNINGS 1

#include "utility.h"
using namespace std;

struct node_core
{
    int vertex;
    int core;
};

struct weighted_neighbor_node
{
    vid_t vertex;
    int weight;
};

struct node_with_layer
{
    int vertex;
    int layer;  ////0:upper layer, 1: lower layer
};

struct follower_t
{
    int vertex_u;
    int vertex_v;
};



struct Edge
{
    vid_t u;
    vid_t v;
    int w; // weight order
    float weight;
};

class BiGraph
{
public:
    /* data */
    BiGraph(string dir, bool bin);
    BiGraph();
    ~BiGraph(){}

    num_t getV1Num(vid_t u) {return num_v1;}
    num_t getV2Num(vid_t u) {return num_v2;}
    num_t getV1Degree(vid_t u)  {return degree_v1[u];}
    num_t getV2Degree(vid_t u) { return degree_v2[u]; }
    
    std::vector<weighted_neighbor_node>& getV1Neighbors(vid_t u) { return neighbor_v1[u]; }
    std::vector<weighted_neighbor_node> & getV2Neighbors(vid_t u)   {return neighbor_v2[u];}

    num_t num_v1;
    num_t num_v2;
    num_t num_edges;
    int alpha_max_;
	int beta_max_;
    int v1_max_degree;
	int v2_max_degree;

    vector<Edge> edges; //Edge ID
	vector<Edge> update_edges;

    std::vector<std::vector<weighted_neighbor_node>> neighbor_v1;
	std::vector<std::vector<weighted_neighbor_node>> neighbor_v2;

    std::vector<int> degree_v1;
	std::vector<int> degree_v2;

	std::vector<num_t> core_v1;
    std::vector<num_t> core_v2;

    std::vector<bool> left_delete;
    std::vector<bool> right_delete;

    std::vector<bool> left_marked;
    std::vector<bool> right_marked;

    std::vector<bool> tmp_left_delete;
    std::vector<bool> tmp_right_delete;
    std::vector<bool> tmp_edges;

    std::vector<num_t> community_left_set;
    std::vector<num_t> community_right_set;

    std::vector<num_t> vulnerable_order_v1;
    std::vector<num_t> vulnerable_order_v2;

    std::vector<std::vector<vid_t>> follower_right_neighbor_v1;
	std::vector<std::vector<vid_t>> follower_left_neighbor_v1;
    std::vector<std::vector<vid_t>> inner_neighbor_v1;

    std::vector<std::vector<vid_t>> follower_right_neighbor_v2;
	std::vector<std::vector<vid_t>> follower_left_neighbor_v2;
    std::vector<std::vector<vid_t>> inner_neighbor_v2;

    std::vector<num_t> larger_community_left_set;
    std::vector<num_t> larger_community_right_set;
    std::vector<std::vector<vid_t>> larger_community_neighbor_v1;
    std::vector<std::vector<vid_t>> larger_community_neighbor_v2;

    std::vector<num_t> exposed_v1;
    std::vector<num_t> exposed_v2;
    std::vector<num_t> influence_v1;
    std::vector<num_t> influence_v2;
    std::vector<num_t> steady_v1;
    std::vector<num_t> steady_v2;

    std::vector<follower_t> followers_count_v1;
    std::vector<follower_t> followers_count_v2;

    void init(unsigned int num_v1, unsigned int num_v2, unsigned int n_deges);
};

#endif

