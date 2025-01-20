#include "graph.h"
#include <queue>
#include <iostream>

void graph::init(int u[], int v[], int w[])
{
    // �����ڽӱ���ʾͼ
    for (int i = 0; i < edge_num; ++i) {
        // ���� u -> v �ı�
        Edge* newEdge1 = new Edge{ v[i], w[i], nullptr };
        newEdge1->next = vertex[u[i]].head;
        vertex[u[i]].head = newEdge1;

        // ���� v -> u �ıߣ��ԳƱߣ�
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
    // ��dijkstra�㷨�����·��
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq; // ��С�� (distanceance, vertex)
    for (int i = 0; i < vertex_num; i++) {
        vertex[i].distance = INT_MAX;  // ��ʼ������Ϊ�����
        vertex[i].sure = 0;        // ȷ�����·���ı��
        vertex[i].prev = -1;       // ��¼ǰ���ڵ�
    }

    vertex[s].distance = 0;  // Դ��ľ���Ϊ0
    pq.push({ 0, s });      // ��Դ��������ȶ���

    while (!pq.empty()) {
        int u = pq.top().second;  // ȡ��������С�Ķ���
        pq.pop();

        if (vertex[u].sure) continue; // �����ǰ������·����ȷ��������

        vertex[u].sure = 1;  // ��Ǹýڵ��Ѵ���

        // ������ǰ�ڵ�������ڽӱ�
        for (Edge* e = vertex[u].head; e != nullptr; e = e->next) {
            int v = e->adj;
            int weight = e->weight;

            // �����ڽӵ�ľ���
            if (vertex[u].distance + weight < vertex[v].distance) {
                vertex[v].distance = vertex[u].distance + weight;
                vertex[v].prev = u;  // ��¼ǰ���ڵ�
                pq.push({ vertex[v].distance, v });  // �����º�Ľڵ�������
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