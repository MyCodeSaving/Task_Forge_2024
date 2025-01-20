#pragma once
#include <cstdio>
#include <vector>
//#include <utility>
#define NUM_VERTEX 10
#define NUM_EDGE 15

class graph {
public:
	int vertex_num, edge_num;
	struct Edge {
		int adj;	// �ڽӵ�
		int weight;
		struct Edge* next;
	};
	struct Vertex {
		int id;
		struct Edge* head;	// �����Ӧ���ڽӱ�����ͷ
		int visited;	// �Ƿ񱻷��ʣ�������DFS��
		// ��������dijkstra�㷨����
		int distance;
		int sure;
		int prev;
	};
	struct Path {
		std::vector<Vertex> path;
		int distance = -1;
	};
	struct Vertex vertex[NUM_VERTEX];

	graph() = default;
	graph(int _vertex, int _edge) {
		vertex_num = _vertex;
		edge_num = _edge;
		for (int i = 0;i < vertex_num;i++) {
			vertex[i].id = i;
			vertex[i].visited = 0;
			vertex[i].head = NULL;
		}
	}
	~graph() = default;
	void init(int u[], int v[], int w[]);
	struct Path find_path(int s, int d);	// �������������·��
};