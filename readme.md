# main.cpp

- e_to_bin.cpp：用于将txt文件转换成graph.bin文件，存储了图的基本信息

  - n1,n2: number of vertices in the upper and lower layer

  - m: total number of valid edges (删除了重复边，并将节点起始编号设置为0)

  - max_degree_v1: 上层节点最大度

  - max_degree_v2: 下层节点最大度

  - degree_v1: vector 上层各节点的度数

  - degree_v2: vector 下层各节点的度数

  - valid_edges: vector 所有有效边

  - neighbor_v1：上层节点的邻接矩阵

  - neighbor_v2：下层节点的邻接矩阵

    

- BiGraph g(dataset,true); //将dataset load进来
- Baseline algorithm SBA
  - as,bs：是上下层节点个数限制