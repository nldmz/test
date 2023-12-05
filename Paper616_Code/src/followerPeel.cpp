#include "followerPeel.h"
#include "algorithm"
#include <map>



using namespace std;

void vertex_division(BiGraph &g)
{
    int min_order = INF;
    int n1 = g.num_v1;
    int n2 = g.num_v2;
    queue<pair<vid_t,int>> Q;   //second int indicate its layer

    for (int i = 0; i < n1;i++)
    {
        if(g.vulnerable_order_v1[i]<min_order)
            min_order = g.vulnerable_order_v1[i];
    }
    for (int i = 0; i < n2;i++)
    {
        if(g.vulnerable_order_v2[i]<min_order)
            min_order = g.vulnerable_order_v2[i];
    }
    g.left_marked.resize(n1);
    g.right_marked.resize(n2);
    fill_n(g.left_marked.begin(), n1, 0);
    fill_n(g.right_marked.begin(), n2, 0);
    /////exposed vertex
    for (int i = 0; i < n1;i++)
    {
        if(g.vulnerable_order_v1[i]==min_order){
            g.exposed_v1.push_back(i);
            g.left_marked[i] = true;
            Q.push(make_pair(i, 0));
        }
    }
    for (int i = 0; i < n2;i++)
    {
        if(g.vulnerable_order_v2[i]==min_order){
            g.exposed_v2.push_back(i);
            g.right_marked[i] = true;
            Q.push(make_pair(i, 1));
        }
    }

    /////////////Influenced Vertex
    while(!Q.empty())
    {
        int top = Q.front().first;
        int tmp_layer = Q.front().second;
        Q.pop();
        if(tmp_layer==0)
        {
            std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v1[top];
            int ss = tmp_neigh_.size();
            for (int k = 0; k < ss;k++)
            {
                vid_t u = tmp_neigh_[k];
                g.influence_v2.push_back(u);
                g.right_marked[u] = true;
            }
        }
        if(tmp_layer==1)
        {
            std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v2[top];
            int ss = tmp_neigh_.size();
            for (int k = 0; k < ss;k++)
            {
                vid_t u = tmp_neigh_[k];
                g.influence_v1.push_back(u);
                g.left_marked[u] = true;
            }
        }
    }

    ////////////Steady Vertex
    for (int i = 0; i < n1;i++)
        if(!g.left_marked[i])
            g.steady_v1.push_back(i);
    for (int i = 0; i < n2;i++)
        if(!g.right_marked[i])
            g.steady_v2.push_back(i);

    cout << "vertex division..." << endl;
    return;
}
void follower_computation(BiGraph &g)
{
    /////init followers
    int n1 = g.num_v1;
    int n2 = g.num_v2;
    g.followers_count_v1.resize(n1);
    g.followers_count_v2.resize(n2);
    /////exposed vertex
    int exp_num_v1 = g.exposed_v1.size();
    int exp_num_v2 = g.exposed_v2.size();
    for (int i = 0; i < exp_num_v1;i++)
    {
        int u = g.exposed_v1[i];
        int fu = g.inner_neighbor_v2[u].size();
        int fv = g.inner_neighbor_v1[u].size();
        follower_t fo = {fu+1,fv};
        g.followers_count_v1[u] = fo;
    }
    for (int i = 0; i < exp_num_v2;i++)
    {
        int u = g.exposed_v2[i];
        int fu = g.inner_neighbor_v2[u].size();
        int fv = g.inner_neighbor_v1[u].size();
        follower_t fo = {fu,fv+1};
        g.followers_count_v1[u] = fo;
    }
        /////////Steady vertex
    int std_num_v1 = g.steady_v1.size();
    int std_num_v2 = g.steady_v2.size();
    for (int i = 0; i < std_num_v1;i++)
    {
        int u = g.steady_v1[i];
        follower_t fo = {1,0};
        g.followers_count_v1[u] = fo;
    }
    for (int i = 0; i < std_num_v2;i++)
    {
        int u = g.steady_v2[i];
        follower_t fo = {0,1};
        g.followers_count_v1[u] = fo;
    }

    /////Influenced Vertex
    int inf_num_v1 = g.influence_v1.size();
    int inf_num_v2 = g.influence_v2.size();
    for (int i = 0; i < inf_num_v1;i++)
    {
        int u = g.influence_v1[i];
        vector<weighted_neighbor_node>& tmp_neigh_ = g.follower_left_neighbor_v1[u];
		int ss = tmp_neigh_.size();
        int fu, fv;
        for (int k = 0; k < ss;k++)
        {
            vid_t u = tmp_neigh_[k].vertex;
            if (std::find(g.steady_v2.begin(), g.steady_v2.end(), u) != g.steady_v2.end())
            {
                fu = g.followers_count_v2[u].vertex_u;
                fv = g.followers_count_v2[u].vertex_v;
            }
            follower_t fo = {fu+1,fv};
            g.followers_count_v1[u] = fo;
        }
    }

    for (int i = 0; i < inf_num_v2;i++)
    {
        int u = g.influence_v2[i];
        vector<weighted_neighbor_node>& tmp_neigh_ = g.follower_left_neighbor_v2[u];
		int ss = tmp_neigh_.size();
        int fu, fv;
        for (int k = 0; k < ss;k++)
        {
            vid_t u = tmp_neigh_[k].vertex;
            if (std::find(g.steady_v1.begin(), g.steady_v1.end(), u) != g.steady_v1.end())
            {
                fu = g.followers_count_v1[u].vertex_u;
                fv = g.followers_count_v1[u].vertex_v;
            }
            follower_t fo = {fu,fv+1};
            g.followers_count_v2[u] = fo;
        }
    }

    cout << "follower computation..." << endl;
}
void dp_peeling(BiGraph &g, vid_t q, int as, int bs, int layer)
{
    int dfu, dfv;
    dfu = g.num_v1 - as;
    dfv = g.num_v2 - bs;
    std::vector<std::vector<vid_t>> dp;
    std::vector<std::vector<vid_t>> path;
    int n1 = g.num_v1;
    int n2 = g.num_v2;
    dp.resize(n1+n2);
    path.resize(n1 + n2);
    for (int i = 0; i < n1 + n2;i++)
    {
        dp[i][0] = 0;
        path[i][0] = 0;
    }
    for (int i = 0; i < n1 + n2;i++)
    {
        for (int j = 1; j < dfu;j++){
            dp[i][j] = -INF;
            path[i][j] = 0;
        }
    }
    for (int i=1; i<=n1+n2; i++){
		for (int j=1; j<=dfu; j++){
			if (j>=g.followers_count_v2[i].vertex_v && dp[i-1][j] < dp[i-1][j-g.followers_count_v2[i].vertex_v]+g.followers_count_v2[i].vertex_u) 
			{
                dp[i - 1][j] < dp[i - 1][j - g.followers_count_v2[i].vertex_v] + g.followers_count_v2[i].vertex_u;
                path[i][j] = 1;
			}
			else dp[i][j]=dp[i-1][j];
		}
	}
	
	if (dp[n1+n2][dfu]==dfv) cout<<"yes"<<endl; else cout<<"no"<<endl;
	int i = n1+n2-1;
	int j = dfu;
	while(i>0 && j>0)
	{
		if(path[i][j]==1)
		{
			cout << g.followers_count_v2[i].vertex_u << endl;
			j = j - g.followers_count_v2[i].vertex_v;
		}
		i = i - 1;
	}
}