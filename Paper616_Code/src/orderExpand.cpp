#include "orderingExpand.h"
#include "algorithm"
#include <map>

using namespace std;

void vulnerable_ordering(BiGraph &g, int alpha, int beta)
{
    cout << "vulnerable ordering..." << endl;
    const int N1 = g.num_v1;
    const int N2 = g.num_v2;

    std::vector<int> deg_v1;
    deg_v1.resize(N1);
    std::vector<int> deg_v2;
    deg_v2.resize(N2);
    int md = -1, md_v1 = -1, md_v2 = -1;

    for (int i = 0; i < N1;i++)
    {
        deg_v1[i] = g.degree_v1[i] - alpha;
        md = max(md, deg_v1[i]);
        md_v1 = max(md, deg_v1[i]);
    }
        
    for (int i = 0; i < N2;i++)
    {
        deg_v2[i] = g.degree_v2[i] - beta;
        md = max(md, deg_v2[i]);
        md_v2 = max(md, deg_v2[i]);
    }

    std::vector<int> bin_v1;
    bin_v1.resize(N1 + 1);
    std::vector<int> bin_v2;
    bin_v2.resize(N2 + 1);
    fill_n(bin_v1.begin(), bin_v1.size(), 0);
    fill_n(bin_v2.begin(), bin_v2.size(), 0);

    for (int i = 0; i < N1;i++)
    {
        int tmp = deg_v1[i];
        ++bin_v1[tmp];
    }
        
    for (int i = 0; i < N2;i++)
    {
        int tmp = deg_v2[i];
        ++bin_v2[tmp];    //此时bin[i]表示度数为i的点有多少个
    }
        
    
    int start = 0;
    for(int i = 0;i<=md_v1;++i){    //此时bin[i]表示度数为i的第一个点
        int num = bin_v1[i];
        bin_v1[i] = start;
        start += num;
    }
    
    start = 0;
    for(int i = 0;i<=md_v2;++i){    
        int num = bin_v2[i];
        bin_v2[i] = start;
        start += num;
    }

    std::vector<int> pos_v1, pos_v2;
    std::vector<int> vert_v1, vert_v2;
    pos_v1.resize(N1);
    pos_v2.resize(N2);
    vert_v1.resize(N1);
    vert_v2.resize(N2);

    for(int i = 0;i<N1;++i){
        int v = i;
        pos_v1[v] = bin_v1[deg_v1[v]];
        vert_v1[pos_v1[v]] = v;
        ++bin_v1[deg_v1[v]];
    }

    for(int i = 0;i<N2;++i){
        int v = i;
        pos_v2[v] = bin_v2[deg_v2[v]];
        vert_v2[pos_v2[v]] = v;
        ++bin_v2[deg_v2[v]];
    }

    //此时bin[i]表示度数为i+1的第一个点，所以后面要进行左移操作
    for(int i = md_v1;i > 0;--i){
        bin_v1[i] = bin_v1[i-1];
    }
    bin_v1[0] = 0;
    bin_v1[md_v1 + 1] = N1;

    for(int i = md_v2;i > 0;--i){
        bin_v2[i] = bin_v2[i-1];
    }
    bin_v2[0] = 0;
    bin_v2[md_v2 + 1] = N2;

    int level = 1;
    g.vulnerable_order_v1.resize(N1);
    g.vulnerable_order_v2.resize(N2);
    fill_n(g.vulnerable_order_v1.begin(), g.vulnerable_order_v1.size(), 0);
    fill_n(g.vulnerable_order_v2.begin(), g.vulnerable_order_v2.size(), 0);

    std::vector<int> vertex_to_be_deleted_v1, vertex_to_be_deleted_v2;
    while(vert_v1.size()>0||vert_v2.size()>0)
    {
        int min_b1 = -1, min_b2 = -1, min_b = -1;
        for (int i = 0; i <= md_v1+1;i++)
            if(bin_v1[i]>0)
            {
                min_b1 = i-1;
                break;
            }
        for (int i = 0; i <= md_v2+1;i++)
            if(bin_v2[i]>0)
            {
                min_b2 = i-1;
                break;
            }
        
        min_b = min(min_b1, min_b2);
        if(min_b<0)
        {
            if(min_b1==-1&&min_b2==-1)
                break;
            else
                min_b=max(min_b1, min_b2);
        }
        if(min_b==min_b1)
        {
            if((bin_v1[min_b1]==bin_v1[min_b1+1]))
                continue;
            for (int i = bin_v1[min_b1]; i < bin_v1[min_b1 + 1];i++)
            {
                //cout <<"vert_v1[i]= "<< vert_v1[i] << endl;
                vertex_to_be_deleted_v1.push_back(vert_v1[i]);
                g.vulnerable_order_v1[vert_v1[i]] = level;
            }
        
            int v1_deleted_size = vertex_to_be_deleted_v1.size();
            while(v1_deleted_size--)
            {
                int i = bin_v1[min_b1];
                vert_v1.erase(vert_v1.begin() + i);
            }
            v1_deleted_size = vertex_to_be_deleted_v1.size();
            for (int i = 0; i <= md_v1;i++)
            {
                bin_v1[i+1] -= v1_deleted_size;
            }
            bin_v1[min_b1] = 0;
            fill_n(pos_v1.begin(), pos_v1.size(), -1);
            for (int i = 0; i < vert_v1.size();i++)
                pos_v1[vert_v1[i]] = i;
        }
        if(min_b==min_b2)
        {
            if(bin_v2[min_b2]==bin_v2[min_b2+1])
                continue;
            for (int i = bin_v2[min_b2]; i < bin_v2[min_b2 + 1];i++)
            {
                //cout <<" vert_v2[i]= "<< vert_v2[i] << endl;
                vertex_to_be_deleted_v2.push_back(vert_v2[i]);
                g.vulnerable_order_v2[vert_v2[i]] = level;
            }

            int v2_deleted_size = vertex_to_be_deleted_v2.size();
            while(v2_deleted_size--)
            {
                int i = bin_v2[min_b2];
                vert_v2.erase(vert_v2.begin() + i);
            }
            v2_deleted_size = vertex_to_be_deleted_v2.size();
            for (int i = min_b2; i <= md_v2;i++)
            {
                bin_v2[i+1] -= v2_deleted_size;
            }
            bin_v2[min_b2] = 0;
            fill_n(pos_v2.begin(), pos_v2.size(), -1);
            for (int i = 0; i < vert_v2.size();i++)
                pos_v2[vert_v2[i]] = i;
        }

        /////update its incident vertex's degree
        if(min_b==min_b1)
        {
            int v1_deleted_size = vertex_to_be_deleted_v1.size();
            for (int i = 0; i < v1_deleted_size; i++)
            {
                int v = vertex_to_be_deleted_v1[i];
                vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v1[v];
			    int ss = tmp_neigh_.size();
			    for (int k = 0; k < ss; k++)
                {
                    int u = tmp_neigh_[k].vertex;
                    if(g.vulnerable_order_v2[u]!=0)
                        continue;
                    int w = vert_v2[bin_v2[deg_v2[u]]];
                    if(u!=w){
                        swap(vert_v2[pos_v2[u]],vert_v2[pos_v2[w]]);
                        swap(pos_v2[u],pos_v2[w]);
                    }
                    ++bin_v2[deg_v2[u]];
                    --deg_v2[u];
                    if(deg_v2[u]<0)
                    {
                        deg_v2[u] = 0;
                        --bin_v2[deg_v2[u]];
                    }
                }
            }
            cout << vertex_to_be_deleted_v1.size() << endl;
            vertex_to_be_deleted_v1.clear();
        }
        if(min_b2==min_b)
        {
            int v2_deleted_size = vertex_to_be_deleted_v2.size();
            for (int i = 0; i < vertex_to_be_deleted_v2.size(); i++)
            {
                int v = vertex_to_be_deleted_v2[i];
                vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v2[v];
			    int ss = tmp_neigh_.size();
			    for (int k = 0; k < ss; k++)
                {
                    int u = tmp_neigh_[k].vertex;
                    if(g.vulnerable_order_v1[u]!=0)
                        continue;
                    int w = vert_v1[bin_v1[deg_v1[u]]];
                    if(u!=w){
                        swap(vert_v1[pos_v1[u]],vert_v1[pos_v1[w]]);
                        swap(pos_v1[u],pos_v1[w]);
                    }
                    ++bin_v1[deg_v1[u]];
                    --deg_v1[u];
                    if(deg_v1[u]<0)
                    {
                        deg_v1[u] = 0;
                        --bin_v1[deg_v1[u]];
                    }
                    
                }
            }
            
            cout << vertex_to_be_deleted_v2.size() << endl;
            vertex_to_be_deleted_v2.clear();
        }
        level++;
        // cout << "Terminate..." << endl;
    }


    // for (int i = 0; i < N1;i++)
    //     cout << g.vulnerable_order_v1[i] << " ";
    // cout << endl;
    // for (int i = 0; i < N2;i++)
    //     cout << g.vulnerable_order_v2[i] << " ";
    // cout << endl;

    return;
}

void new_vulnerable_ordering(BiGraph &g, int alpha, int beta)
{
    cout << "vulnerable ordering..." << endl;
    const int N1 = g.num_v1;
    const int N2 = g.num_v2;

    std::vector<int> deg_v1;
    deg_v1.resize(N1);
    std::vector<int> deg_v2;
    deg_v2.resize(N2);
    int md = -1, md_v1 = -1, md_v2 = -1;

    for (int i = 0; i < N1;i++)
    {
        deg_v1[i] = g.degree_v1[i] - alpha;
        md = max(md, deg_v1[i]);
        md_v1 = max(md, deg_v1[i]);
    }
        
    for (int i = 0; i < N2;i++)
    {
        deg_v2[i] = g.degree_v2[i] - beta;
        md = max(md, deg_v2[i]);
        md_v2 = max(md, deg_v2[i]);
    }

    std::vector<int> bin_v1;
    bin_v1.resize(N1 + 1);
    std::vector<int> bin_v2;
    bin_v2.resize(N2 + 1);
    fill_n(bin_v1.begin(), bin_v1.size(), 0);
    fill_n(bin_v2.begin(), bin_v2.size(), 0);

    for (int i = 0; i < N1;i++)
    {
        int tmp = deg_v1[i];
        ++bin_v1[tmp];
    }
        
    for (int i = 0; i < N2;i++)
    {
        int tmp = deg_v2[i];
        ++bin_v2[tmp];    //此时bin[i]表示度数为i的点有多少个
    }
        
    
    int start = 0;
    for(int i = 0;i<=md_v1;++i){    //此时bin[i]表示度数为i的第一个点
        int num = bin_v1[i];
        bin_v1[i] = start;
        start += num;
    }
    
    start = 0;
    for(int i = 0;i<=md_v2;++i){    
        int num = bin_v2[i];
        bin_v2[i] = start;
        start += num;
    }

    std::vector<int> pos_v1, pos_v2;
    std::vector<int> vert_v1, vert_v2;
    pos_v1.resize(N1);
    pos_v2.resize(N2);
    vert_v1.resize(N1);
    vert_v2.resize(N2);

    for(int i = 0;i<N1;++i){
        int v = i;
        pos_v1[v] = bin_v1[deg_v1[v]];
        vert_v1[pos_v1[v]] = v;
        ++bin_v1[deg_v1[v]];
    }

    for(int i = 0;i<N2;++i){
        int v = i;
        pos_v2[v] = bin_v2[deg_v2[v]];
        vert_v2[pos_v2[v]] = v;
        ++bin_v2[deg_v2[v]];
    }

    //此时bin[i]表示度数为i+1的第一个点，所以后面要进行左移操作
    for(int i = md_v1;i > 0;--i){
        bin_v1[i] = bin_v1[i-1];
    }
    bin_v1[0] = 0;
    bin_v1[md_v1 + 1] = N1;

    for(int i = md_v2;i > 0;--i){
        bin_v2[i] = bin_v2[i-1];
    }
    bin_v2[0] = 0;
    bin_v2[md_v2 + 1] = N2;

    int level = 1;
    g.vulnerable_order_v1.resize(N1);
    g.vulnerable_order_v2.resize(N2);
    fill_n(g.vulnerable_order_v1.begin(), g.vulnerable_order_v1.size(), 0);
    fill_n(g.vulnerable_order_v2.begin(), g.vulnerable_order_v2.size(), 0);

    std::vector<int> vertex_to_be_deleted_v1, vertex_to_be_deleted_v2;
    while(vert_v1.size()>0||vert_v2.size()>0)
    {
        int min_b1 = -1, min_b2 = -1, min_b = -1;
        for (int i = 0; i <= md_v1+1;i++)
            if(bin_v1[i]>0)
            {
                min_b1 = i-1;
                break;
            }
        for (int i = 0; i <= md_v2+1;i++)
            if(bin_v2[i]>0)
            {
                min_b2 = i-1;
                break;
            }
        
        min_b = min(min_b1, min_b2);
        if(min_b<0)
        {
            if(min_b1==-1&&min_b2==-1)
                break;
            else
                min_b=max(min_b1, min_b2);
        }
        if(min_b==min_b1)
        {
            if((bin_v1[min_b1]==bin_v1[min_b1+1]))
                continue;
            for (int i = bin_v1[min_b1]; i < bin_v1[min_b1 + 1];i++)
            {
                //cout <<"vert_v1[i]= "<< vert_v1[i] << endl;
                vertex_to_be_deleted_v1.push_back(vert_v1[i]);
                g.vulnerable_order_v1[vert_v1[i]] = level;
            }
        
            int v1_deleted_size = vertex_to_be_deleted_v1.size();
            while(v1_deleted_size--)
            {
                int i = bin_v1[min_b1];
                vert_v1.erase(vert_v1.begin() + i);
            }
            v1_deleted_size = vertex_to_be_deleted_v1.size();
            for (int i = 0; i <= md_v1;i++)
            {
                bin_v1[i+1] -= v1_deleted_size;
            }
            bin_v1[min_b1] = 0;
            fill_n(pos_v1.begin(), pos_v1.size(), -1);
            for (int i = 0; i < vert_v1.size();i++)
                pos_v1[vert_v1[i]] = i;
        }
        if(min_b==min_b2)
        {
            if(bin_v2[min_b2]==bin_v2[min_b2+1])
                continue;
            for (int i = bin_v2[min_b2]; i < bin_v2[min_b2 + 1];i++)
            {
                //cout <<" vert_v2[i]= "<< vert_v2[i] << endl;
                vertex_to_be_deleted_v2.push_back(vert_v2[i]);
                g.vulnerable_order_v2[vert_v2[i]] = level;
            }

            int v2_deleted_size = vertex_to_be_deleted_v2.size();
            while(v2_deleted_size--)
            {
                int i = bin_v2[min_b2];
                vert_v2.erase(vert_v2.begin() + i);
            }
            v2_deleted_size = vertex_to_be_deleted_v2.size();
            for (int i = min_b2; i <= md_v2;i++)
            {
                bin_v2[i+1] -= v2_deleted_size;
            }
            bin_v2[min_b2] = 0;
            fill_n(pos_v2.begin(), pos_v2.size(), -1);
            for (int i = 0; i < vert_v2.size();i++)
                pos_v2[vert_v2[i]] = i;
        }

        /////update its incident vertex's degree
        if(min_b==min_b1)
        {
            int v1_deleted_size = vertex_to_be_deleted_v1.size();
            for (int i = 0; i < v1_deleted_size; i++)
            {
                int v = vertex_to_be_deleted_v1[i];
                vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v1[v];
			    int ss = tmp_neigh_.size();
			    for (int k = 0; k < ss; k++)
                {
                    int u = tmp_neigh_[k].vertex;
                    if(g.vulnerable_order_v2[u]!=0)
                        continue;
                    int w = vert_v2[bin_v2[deg_v2[u]]];
                    if(u!=w){
                        swap(vert_v2[pos_v2[u]],vert_v2[pos_v2[w]]);
                        swap(pos_v2[u],pos_v2[w]);
                    }
                    ++bin_v2[deg_v2[u]];
                    --deg_v2[u];
                    if(deg_v2[u]<0)
                    {
                        deg_v2[u] = 0;
                        --bin_v2[deg_v2[u]];
                    }
                }
            }
            cout << vertex_to_be_deleted_v1.size() << endl;
            vertex_to_be_deleted_v1.clear();
        }
        if(min_b2==min_b)
        {
            int v2_deleted_size = vertex_to_be_deleted_v2.size();
            for (int i = 0; i < vertex_to_be_deleted_v2.size(); i++)
            {
                int v = vertex_to_be_deleted_v2[i];
                vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v2[v];
			    int ss = tmp_neigh_.size();
			    for (int k = 0; k < ss; k++)
                {
                    int u = tmp_neigh_[k].vertex;
                    if(g.vulnerable_order_v1[u]!=0)
                        continue;
                    int w = vert_v1[bin_v1[deg_v1[u]]];
                    if(u!=w){
                        swap(vert_v1[pos_v1[u]],vert_v1[pos_v1[w]]);
                        swap(pos_v1[u],pos_v1[w]);
                    }
                    ++bin_v1[deg_v1[u]];
                    --deg_v1[u];
                    if(deg_v1[u]<0)
                    {
                        deg_v1[u] = 0;
                        --bin_v1[deg_v1[u]];
                    }
                    
                }
            }
            
            cout << vertex_to_be_deleted_v2.size() << endl;
            vertex_to_be_deleted_v2.clear();
        }
        level++;
    }
    return;
}

void transformed_network(BiGraph &g)
{
    int rrnn = g.num_v1;
    int llnn = g.num_v2;

    ////transformed networks:
    g.follower_left_neighbor_v1.resize(rrnn);
    g.follower_left_neighbor_v2.resize(llnn);
    g.follower_right_neighbor_v1.resize(rrnn);
    g.follower_right_neighbor_v2.resize(llnn);
    g.inner_neighbor_v1.resize(rrnn);
    g.inner_neighbor_v2.resize(llnn);
    for (int u = 0; u < rrnn;u++)
    {
        vector<weighted_neighbor_node>& tmp_neigh_ = g.neighbor_v1[u];
		int ss = tmp_neigh_.size();
		for (int k = 0; k < ss; k++)
        {
            vid_t v = tmp_neigh_[k].vertex;
            if(g.vulnerable_order_v1[u] < g.vulnerable_order_v2[v]){
                
                g.follower_right_neighbor_v1[u].push_back(v);
                
                g.follower_left_neighbor_v2[v].push_back(u);

            }
            else if(g.vulnerable_order_v1[u] > g.vulnerable_order_v2[v]){

                g.follower_left_neighbor_v1[u].push_back(v);
                g.follower_right_neighbor_v2[v].push_back(u);
            }
            else{
                g.inner_neighbor_v1[u].push_back(v);
                g.inner_neighbor_v2[v].push_back(u);
            }
        }
    }
}

void expanding(BiGraph &g, vid_t q, int as, int bs, int layer)
{
    int up_layer_num_v1 = 0, up_layer_num_v2 = 0;
    int same_layer_num_v1 = 0, same_layer_num_v2 = 0;
    int down_layer_num_v1 = 0, down_layer_num_v2 = 0;

    queue<pair<vid_t,int>> Q;   //second int indicate its layer
    Q.push(make_pair(q,layer));
    if(layer==0)
    {
        g.larger_community_left_set.push_back(q);
    }
    else
        g.larger_community_right_set.push_back(q);
    
    int cur_as = 0, cur_bs = 0;
    while(!Q.empty())
    {
        int top = Q.front().first;
        int tmp_layer = Q.front().second;
        Q.pop();
        if(tmp_layer == 0)
        {
            std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v1[top];
            int ss = tmp_neigh_.size();
            cur_bs += ss;
            for (int k = 0; k < ss;k++)
            {
                vid_t u = tmp_neigh_[k];
                Q.push(make_pair(u, 0));

                g.larger_community_right_set.push_back(u);
                g.larger_community_neighbor_v1[top].push_back(u);
            }
        }
        if(tmp_layer == 1)
        {
            std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v2[top];
            int ss = tmp_neigh_.size();
            cur_as += ss;
            for (int k = 0; k < ss;k++)
            {
                vid_t u = tmp_neigh_[k];
                Q.push(make_pair(u, 0));

                g.larger_community_left_set.push_back(u);
                g.larger_community_neighbor_v2[top].push_back(u);
            }
        }
        
    }
    if(cur_as<as&&cur_bs<bs)
    {
        Q.push(make_pair(q,layer));
        while(!Q.empty())
        {
            int top = Q.front().first;
            int tmp_layer = Q.front().second;
            Q.pop();
            if(tmp_layer == 0)
            {
                std::vector<vid_t> &tmp_neigh_ = g.inner_neighbor_v1[top];
                int ss = tmp_neigh_.size();
                cur_bs += ss;
                for (int k = 0; k < ss;k++)
                {
                    vid_t u = tmp_neigh_[k];
                    Q.push(make_pair(u, 0));

                    g.larger_community_right_set.push_back(u);
                    g.larger_community_neighbor_v1[top].push_back(u);
                }
            }
            if(tmp_layer == 1)
            {
                std::vector<vid_t> &tmp_neigh_ = g.inner_neighbor_v2[top];
                int ss = tmp_neigh_.size();
                cur_as += ss;
                for (int k = 0; k < ss;k++)
                {
                    vid_t u = tmp_neigh_[k];
                    Q.push(make_pair(u, 0));

                    g.larger_community_left_set.push_back(u);
                    g.larger_community_neighbor_v2[top].push_back(u);
                }
            }
        
        }
    }
    if(cur_as<as&&cur_bs<bs)
    {
        Q.push(make_pair(q,layer));
        while(!Q.empty())
        {
            int top = Q.front().first;
            int tmp_layer = Q.front().second;
            Q.pop();
            if(tmp_layer == 0)
            {
                std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v1[top];
                int ss = tmp_neigh_.size();
                cur_bs += ss;
                for (int k = 0; k < ss;k++)
                {
                    vid_t u = tmp_neigh_[k];
                    Q.push(make_pair(u, 0));

                    g.larger_community_right_set.push_back(u);
                    g.larger_community_neighbor_v1[top].push_back(u);
                }
            }
            if(tmp_layer == 1)
            {
                std::vector<vid_t> &tmp_neigh_ = g.follower_right_neighbor_v2[top];
                int ss = tmp_neigh_.size();
                cur_as += ss;
                for (int k = 0; k < ss;k++)
                {
                    vid_t u = tmp_neigh_[k];
                    Q.push(make_pair(u, 0));

                    g.larger_community_left_set.push_back(u);
                    g.larger_community_neighbor_v2[top].push_back(u);
                }
            }
        
        }
    }
}