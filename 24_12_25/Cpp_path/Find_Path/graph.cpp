#include "graph.h"
#include <queue>
#include <iostream>

void graph::init(int u[], int v[], int w[])
{
    // 采用邻接表法表示图
    for (int i = 0; i < edge_num; ++i) {
        // 插入 u -> v 的边
        Edge* newEdge1 = new Edge{ v[i], w[i], nullptr };
        newEdge1->next = vertex[u[i]].head;
        vertex[u[i]].head = newEdge1;

        // 插入 v -> u 的边（对称边）
        Edge* newEdge2 = new Edge{ u[i], w[i], nullptr };
        newEdge2->next = vertex[v[i]].head;
        vertex[v[i]].head = newEdge2;
    }
}

graph::Path graph::find_path(int s, int d) {
    if (s == -1 || d == -1) {
        std::cout << "Input Error" << std::endl;
        Path p;
        return p;
    }
    // 由dijkstra算法求最短路径
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq; // 最小堆 (distanceance, vertex)
    for (int i = 0; i < vertex_num; i++) {
        vertex[i].distance = INT_MAX;  // 初始距离设为无穷大
        vertex[i].sure = 0;        // 确定最短路径的标记
        vertex[i].prev = -1;       // 记录前驱节点
    }

    vertex[s].distance = 0;  // 源点的距离为0
    pq.push({ 0, s });      // 将源点加入优先队列

    while (!pq.empty()) {
        int u = pq.top().second;  // 取出距离最小的顶点
        pq.pop();

        if (vertex[u].sure) continue; // 如果当前点的最短路径已确定，跳过

        vertex[u].sure = 1;  // 标记该节点已处理

        // 遍历当前节点的所有邻接边
        for (Edge* e = vertex[u].head; e != nullptr; e = e->next) {
            int v = e->adj;
            int weight = e->weight;

            // 更新邻接点的距离
            if (vertex[u].distance + weight < vertex[v].distance) {
                vertex[v].distance = vertex[u].distance + weight;
                vertex[v].prev = u;  // 记录前驱节点
                pq.push({ vertex[v].distance, v });  // 将更新后的节点加入队列
            }
        }
    }
    Path p;
    std::vector<Vertex> path;
    for (int i = d;i != -1;i = vertex[i].prev) {
        path.push_back(vertex[i]);
    }
    std::reverse(path.begin(), path.end());
    p.path = path;
    p.distance = vertex[d].distance;
    return p;
}