#include <stdio.h>
#include <string.h>

#define MAXN 110

char grid[MAXN][MAXN] = {0};
int R, C;

// 判断上顶点左侧格子坐标为 (x, y) 的正方形是否合法
int is_valid_square(int x, int y, int half_size) {
    if (half_size == 0) {
        return 0;
    }
    // 判断正方形是否超出边界
    if (y - half_size + 1 < 1 || y + half_size > R) {
        return 0;
    }

    for (int i = 1; i <= half_size; i++) {
        if (grid[y + i][x + i - 1] != '\\' || grid[y - i + 1][x + i - 1] != '/' ||
            grid[y + i][x + 2*half_size - i] != '/' || grid[y - i + 1][x + 2*half_size - i] != '\\') {
            return 0;
        }
    }
    return 1;
}

int count_squares() {
    int count = 0;

    // 枚举所有可能的正方形大小
    for (int size = 1; size <= R && size <= C; size++) { // size 为对角线长度
        if (size % 2 != 0) {
            continue;
        }
        for (int i = 1; i + size/2 <= R; i++) {
            for (int j = 1; j + size -1 <= C; j++) {
                if (is_valid_square(i, j, size/2)) {
                    count++;
                }
            }
        }
    }

    return count;
}

int main() {
    scanf("%d %d", &R, &C);

    for (int i = 1; i <= R; i++) {
        scanf("%s", grid[i] + 1); // 从 1 开始存储，方便处理边界
    }

    int result = count_squares();

    printf("%d\n", result);

    return 0;
}
