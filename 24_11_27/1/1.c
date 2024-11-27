#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取一个点的邻点的坐标偏移
const int dx[6] = {1, -1, 0, 0, 0, 0};
const int dy[6] = {0, 0, 1, -1, 0, 0};
const int dz[6] = {0, 0, 0, 0, 1, -1};

// 检查某个坐标是否在地图范围内
int is_valid(int x, int y, int z, int n) {
    return x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n;
}

// 将 3D 坐标转换为 1D 索引
int idx_3d_to_1d(int x, int y, int z, int n) {
    return x * n * n + y * n + z;
}

// 计算某个点的周围地雷数量
int count_mines(char *map, int x, int y, int z, int n) {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        int nz = z + dz[i];
        if (is_valid(nx, ny, nz, n) && map[idx_3d_to_1d(nx, ny, nz, n)] == '*') {
            count++;
        }
    }
    return count;
}

int main() {
    int n;
    scanf("%d", &n);

    // 动态分配一维数组保存地图
    char *map = malloc(n * n * n * sizeof(char));
    scanf("%s", map);

    // 找到 '?' 位置
    int qx = -1, qy = -1, qz = -1;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                if (map[idx_3d_to_1d(x, y, z, n)] == '?') {
                    qx = x;
                    qy = y;
                    qz = z;
                }
            }
        }
    }

    if (qx == -1) {
        printf("invalid\n");
        free(map);
        return 0;
    }

    // 假设 '?' 不是地雷，验证所有数字是否正确
    map[idx_3d_to_1d(qx, qy, qz, n)] = '0';
    int valid_no_mines = 1;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                if (qx == x && qy == y && qz == z) {
                    continue;
                }
                char cell = map[idx_3d_to_1d(x, y, z, n)];
                if (cell >= '0' && cell <= '6') {
                    int expected = cell - '0';
                    int actual = count_mines(map, x, y, z, n);
                    if (actual != expected) {
                        valid_no_mines = 0;
                    }
                }
            }
        }
    }
    if (valid_no_mines) {
        printf("valid\n%d\n", count_mines(map, qx, qy, qz, n));
        free(map);
        return 0;
    }

    // 假设 '?' 是地雷，验证所有数字是否正确
    map[idx_3d_to_1d(qx, qy, qz, n)] = '*';
    int valid_mines = 1;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                char cell = map[idx_3d_to_1d(x, y, z, n)];
                if (cell >= '0' && cell <= '6') {
                    int expected = cell - '0';
                    int actual = count_mines(map, x, y, z, n);
                    if (actual != expected) {
                        valid_mines = 0;
                    }
                }
            }
        }
    }

    if (valid_mines) {
        printf("valid\n*\n");
        free(map);
        return 0;
    } else {
        printf("invalid\n");
        // 输出不匹配的点
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n; y++) {
                for (int z = 0; z < n; z++) {
                    char cell = map[idx_3d_to_1d(x, y, z, n)];
                    if (cell >= '0' && cell <= '6') {
                        int expected = cell - '0';
                        int actual = count_mines(map, x, y, z, n);
                        if (actual != expected) {
                            printf("%d %d %d\n", x + 1, y + 1, z + 1);
                        }
                    }
                }
            }
        }
    }

    free(map);
    return 0;
}
