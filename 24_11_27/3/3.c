#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int size;       // 节点的大小（以 2 的幂次表示）
    int start;      // 起始地址
    int id;         // 应用程序 ID（0 表示空闲）
} Node;

Node *nodes;        // 动态分配的节点数组
int node_count = 1; // 当前节点总数，初始只有一个节点
int total_size;     // 内存总大小
int capacity;       // 当前动态数组容量

void initialize(int n) {
    total_size = 1 << n; // 总大小为 2^n
    capacity = 2 * total_size; // 初始容量足够大
    nodes = (Node *)malloc(capacity * sizeof(Node));
    nodes[0].size = total_size;
    nodes[0].start = 0;
    nodes[0].id = 0;
}

// 查询当前内存状态
void query() {
    printf("%d\n", node_count);
    for (int i = 0; i < node_count; i++) {
        printf("%d ", nodes[i].id);
    }
    printf("\n");
}

// 动态扩展节点数组
void expand_nodes() {
    capacity *= 2;
    nodes = (Node *)realloc(nodes, capacity * sizeof(Node));
}

// 分裂节点
void split_node(int index, int required_size) {
    while (nodes[index].size > required_size) {
        int new_size = nodes[index].size / 2;

        // 动态扩展节点数组（如果需要）
        if (node_count + 1 >= capacity) {
            expand_nodes();
        }

        // 创建一个新节点
        for (int i = node_count; i > index + 1; i--) {
            nodes[i] = nodes[i - 1];
        }
        nodes[index + 1].size = new_size;
        nodes[index + 1].start = nodes[index].start + new_size;
        nodes[index + 1].id = 0;

        // 更新原节点大小
        nodes[index].size = new_size;
        node_count++;
    }
}

// 分配内存
void allocate(int id, int m) {
    int required_size = 1;
    while (required_size < m) {
        required_size *= 2;
    }

    for (int i = 0; i < node_count; i++) {
        if (nodes[i].id == 0 && nodes[i].size >= required_size) {
            split_node(i, required_size);
            nodes[i].id = id;
            return;
        }
    }
}

// 释放内存
void cleanup() {
    free(nodes);
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    initialize(n);

    char operation[10];
    for (int i = 0; i < q; i++) {
        scanf("%s", operation);
        if (operation[0] == 'Q') {
            query();
        } else if (operation[0] == 'A') {
            int id, m;
            scanf("%d %d", &id, &m);
            allocate(id, m);
        }
    }

    cleanup();
    return 0;
}
