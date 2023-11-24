#include "utility.h"
#include "biGraph.h"
using namespace std;

void generateCombinations_left(int n, int m, vector<int>& combination, int start);
void generateCombinations_right(int n, int m, vector<int>& combination, int start);
void enumCombinations(BiGraph& g, int as, int bs, int alpha, int beta);
void find_largest_a_b_core(BiGraph& g, int alpha, int beta);
float is_abCore(BiGraph &g, int alpha, int beta);