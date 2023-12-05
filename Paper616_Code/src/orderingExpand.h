#include "utility.h"
#include "biGraph.h"
using namespace std;

void vulnerable_ordering(BiGraph &g, int alpha, int beta);
void new_vulnerable_ordering(BiGraph &g, int alpha, int beta);
void transformed_network(BiGraph &g);
void expanding(BiGraph &g, vid_t q, int as, int bs, int layer);