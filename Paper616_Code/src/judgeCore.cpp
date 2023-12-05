#include "judgeCore.h"

using namespace std;

std::vector<std::vector<int>> enum_left_index;
std::vector<std::vector<int>> enum_right_index;
std::vector<int> tmp_left_index;
std::vector<int> tmp_right_index;
std::vector<int> res_left_index;
std::vector<int> res_right_index;
std::vector<int> a;

int global_i;
int left_num_vertices, right_num_vertices;
int left_times, right_times;



void generateCombinations_left(int k,int as){
	if(k==as+1){  
		for(int i=1; i<=as; i++){
			//cout<<a[i]<<" ";
            enum_left_index[global_i].push_back(a[i]);
        }
        global_i++;
        //cout<<endl; 
		return;
	} 
	
	for(int i=a[k-1]+1; i<=left_num_vertices; i++){  
		a[k]=i;   
		generateCombinations_left(k+1,as);    
	}
}

void generateCombinations_right(int k,int bs){
	if(k==bs+1){  
		for(int i=1; i<=bs; i++){
			//cout<<a[i]<<" ";
            enum_right_index[global_i].push_back(a[i]);
        }
        global_i++;
        //cout<<endl; 
		return;
	} 
	
	for(int i=a[k-1]+1; i<=right_num_vertices; i++){  
		a[k]=i;   
		generateCombinations_right(k+1,bs);    
	}
}



float is_abCore(BiGraph& g, int alpha, int beta)
{
    g.tmp_left_delete.resize(g.num_v1);
    fill_n(g.tmp_left_delete.begin(), g.num_v1, 1);
    for (int i = 0; i < tmp_left_index.size(); i++)
        g.tmp_left_delete[tmp_left_index[i] - 1] = 0;

    g.tmp_right_delete.resize(g.num_v2);
    fill_n(g.tmp_right_delete.begin(), g.num_v2, 1);
    for (int i = 0; i < tmp_right_index.size(); i++)
        g.tmp_right_delete[tmp_right_index[i] - 1] = 0;

    for (int i = 0; i < tmp_left_index.size(); i++)
    {
        int left_index = tmp_left_index[i] - 1;
        int degree_difference = g.neighbor_v1[left_index].size() - alpha;
        if(degree_difference < 0)
            return 0;
        int tmp_delete_vertice_num = 0;
        for (int j = 0; j < g.neighbor_v1[left_index].size(); j++)
        {
            weighted_neighbor_node tmp_u = g.neighbor_v1[left_index][j];
            if(g.tmp_right_delete[tmp_u.vertex])
                tmp_delete_vertice_num++;
            //cout << tmp_u.vertex << " " << tmp_u.weight << endl;
            if(degree_difference < tmp_delete_vertice_num)
                return 0;
        }
    }
        
    
    return 1;
}

void find_largest_a_b_core(BiGraph& g, int alpha, int beta)
{
    float max_weight = 0;
    for (int i = 0;i < left_times;i++)
        for (int j = 0;j < right_times;j++)
        {
            tmp_left_index.clear();
            tmp_right_index.clear();
            tmp_left_index = enum_left_index[i];
            tmp_right_index = enum_right_index[j];
            float tmp_cohesive = is_abCore(g, alpha, beta);
            if(tmp_cohesive>0)
            {
                res_left_index = tmp_left_index;
                res_right_index = tmp_right_index;
            }
        }
    for (int i = 0; i < res_left_index.size(); i++)
        cout << res_left_index[i] << " ";
    cout << endl;
    for (int i = 0; i < res_right_index.size(); i++)
        cout << res_right_index[i] << " ";
    cout << endl;
    return;
}

void enumCombinations(BiGraph& g, int as, int bs, int alpha, int beta)
{
    cout << "enumerate left vertices ..." << endl;
    left_num_vertices = g.community_left_set.size();
    left_times = std::tgamma(left_num_vertices + 1) / (std::tgamma(as + 1) * std::tgamma(left_num_vertices - as + 1));
    enum_left_index.resize(left_times);
    a.resize(left_num_vertices);
    global_i = 0;
    generateCombinations_left(1, as);
    
    cout << "enumerate right vertices ..." << endl;
    right_num_vertices = g.community_right_set.size();
    right_times = std::tgamma(right_num_vertices + 1) / (std::tgamma(bs + 1) * std::tgamma(right_num_vertices - bs + 1));
    enum_right_index.resize(right_times);
    global_i = 0;
    a.resize(right_num_vertices);
    generateCombinations_right(1, bs);
    cout << "enumerating end..." << endl;

    find_largest_a_b_core(g, alpha, beta);

}


