#include "utility.h"
#include "biGraph.h"
using namespace std;



void vertex_division(BiGraph &g);
void follower_computation(BiGraph &g);
void dp_peeling(BiGraph &g, vid_t q, int as, int bs, int layer);