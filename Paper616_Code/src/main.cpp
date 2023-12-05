#include "utility.h"
#include "e_to_bin.h"
#include "abcoreDecom.h"
#include "biGraph.h"
#include "judgeCore.h"
#include "orderingExpand.h"
#include "followerPeel.h"

using namespace std;

bool BIN,SBA,OEA,FPA,EPF;
string dataset, filename;

int main(int argc, char* argv[])
{
    
    BIN = false;
    dataset = "D:\\Code\\C\\ICDE24_StaticCode\\Datasets\\";
    filename = "example.txt";
    int q, alpha, beta, as, bs; // input parameters
    if (BIN)
    {
        save_graph_to_bin(dataset, filename);
    }
    SBA = false;
    if(SBA) // Baseline algorithm for static bipartite graphs
    {
        cout<<dataset<<endl;
        BiGraph g(dataset,true);
        g.init(g.num_v1, g.num_v2, g.num_edges);
        alpha = 2;
        beta = 2;
        as = 2;
        bs = 2;
        compute_a_b_core(g, alpha, beta);
        enumCombinations(g, as, bs, alpha, beta);
    }
    OEA = false;
    if(OEA) // ordering-based expanding algorithm
    {
        BiGraph g(dataset,true);
        g.init(g.num_v1, g.num_v2, g.num_edges);
        alpha = 2;
        beta = 2;
        as = 2;
        bs = 2;
        vid_t q = 0;
        int layer = 0;  //0:upper layer, 1: lower layer
        compute_a_b_core(g, alpha, beta);
        vulnerable_ordering(g, alpha, beta);
        transformed_network(g);
        expanding(g,q,as,bs,layer);
    }
    FPA = true;
    if(FPA)
    {
        BiGraph g(dataset,true);
        g.init(g.num_v1, g.num_v2, g.num_edges);
        alpha = 2;
        beta = 2;
        as = 2;
        bs = 2;
        vid_t q = 0;
        int layer = 0;  //0:upper layer, 1: lower layer
        vulnerable_ordering(g, alpha, beta);
        transformed_network(g);
        vertex_division(g);
        follower_computation(g);
        dp_peeling(g,q,as,bs,layer);
    }
    EPF = false;
    if(EPF)
    {
        BiGraph g(dataset,true);
        g.init(g.num_v1, g.num_v2, g.num_edges);
        alpha = 2;
        beta = 2;
        as = 2;
        bs = 2;
        vid_t q = 0;
        int layer = 0;  //0:upper layer, 1: lower layer
        compute_a_b_core(g, alpha, beta);
        vulnerable_ordering(g, alpha, beta);
        transformed_network(g);
        expanding(g,q,as,bs,layer);
        new_vulnerable_ordering(g, alpha, beta);
        vertex_division(g);
        follower_computation(g);
        dp_peeling(g,q,as,bs,layer);
    }
    return 0;
}