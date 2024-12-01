// Last modified: 24/11/29 20:15
// Author: lyingloong

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100
#define MAX_M 100

int n, m, d;                   // 地图大小 n x m 和锤子范围 d
int value[MAX_N][MAX_M];       // 保存地图上的地鼠价值
int maxValue = -2147483648;    // 保存最大价值，初始化为最小整数
int results[MAX_N * MAX_M][2]; // 保存所有能达到最大价值的中心点
int resultCount = 0;           // 最大价值的中心点数量

// 检查 (x, y) 是否在地图范围内
int is_valid(int x, int y) {
    return x >= 1 && x <= n && y >= 1 && y <= m;
}

// 计算锤子中心点为 (x0, y0) 时的总价值（优化前，使用滑动窗口计算）
int calculate_value(int x0, int y0) {
    int sum = 0;
    for (int dx = -d; dx <= d; dx++) {
        for (int dy = -d; dy <= d; dy++) {
            if (abs(dx) + abs(dy) <= d) { // 满足菱形范围条件
                int x = x0 + dx;
                int y = y0 + dy;
                if (is_valid(x, y)) {
                    sum += value[x - 1][y - 1];
                }
            }
        }
    }
    return sum;
}

// 滑动窗口优化：利用前一个结果，只计算增量部分
void sliding_window() {
    int window[MAX_N][MAX_M] = {0};

    // 计算第一个窗口（第1行，第1列的锤子范围）
    for (int x = 1; x <= n; x++) {
        for (int y = 1; y <= m; y++) {
            window[x][y] = calculate_value(x, y);
        }
    }

    // 逐个检查每个中心点的锤子范围，更新结果
    for (int x = 1; x <= n; x++) {
        for (int y = 1; y <= m; y++) {
            int currentValue = window[x][y];
            if (currentValue > maxValue) {
                maxValue = currentValue;
                resultCount = 0;
                results[resultCount][0] = x;
                results[resultCount][1] = y;
                resultCount++;
            } else if (currentValue == maxValue) {
                results[resultCount][0] = x;
                results[resultCount][1] = y;
                resultCount++;
            }
        }
    }
}

void test() {
    // 测试用例1
    maxValue = -2147483648;
    resultCount = 0; 
    printf("Test case 1:\n");
    n = 3; m = 3; d = 1;
    int test1[MAX_N][MAX_M] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test1[i][j];
        }
    }
    sliding_window();

    // 输出结果
    // 29 1
    // 3 2
    printf("%d ", maxValue);
    printf("%d\n", resultCount);
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }

    // 测试用例2
    maxValue = -2147483648;
    resultCount = 0; 
    printf("Test case 2:\n");
    n = 5; m = 5; d = 2;
    int test2[MAX_N][MAX_M] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test2[i][j];
        }
    }
    sliding_window();

    // 输出结果
    printf("%d ", maxValue);
    printf("%d\n", resultCount);
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }

    // 测试用例3
    maxValue = -2147483648;
    resultCount = 0; 
    printf("Test case 3:\n");
    n = 5; m = 5; d = 3;
    int test3[MAX_N][MAX_M] = {
        {-1, -2, -3, -4, -5},
        {-6, -7, -8, -9, -10},
        {-11, -12, -13, -14, -15},
        {-16, -17, -18, -19, -20},
        {-21, -22, -23, -24, -25}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test3[i][j];
        }
    }
    sliding_window();

    // 输出结果
    printf("%d ", maxValue);
    printf("%d\n", resultCount);
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }

    // 测试用例4
    maxValue = -2147483648;
    resultCount = 0; 
    printf("Test case 4:\n");
    n = 4; m = 6; d = 1;
    int test4[MAX_N][MAX_M] = {
        {-3, 4, 5, 4, 3, 2},
        {1, 6, 1, 7, 5, 2},
        {9, 9, 0, 0, 8, -3},
        {8, 8, 4, 4, -3, 2}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test4[i][j];
        }
    }
    sliding_window();

    // 输出结果
    // 32 1
    // 3 2
    printf("%d ", maxValue);
    printf("%d\n", resultCount);
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }

    // 测试用例5
    maxValue = -2147483648;
    resultCount = 0; 
    printf("Test case 5:\n");
    n = 4; m = 6; d = 2;
    int test5[MAX_N][MAX_M] = {
        {-3, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test5[i][j];
        }
    }
    sliding_window();

    // 输出结果
    // 0 18
    printf("%d ", maxValue);
    printf("%d\n", resultCount);
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }
}

int main() {
    // scanf("%d %d %d", &n, &m, &d);
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < m; j++) {
    //         scanf("%d", &value[i][j]);
    //     }
    // }

    // sliding_window();

    // // 输出结果
    // printf("%d ", maxValue);
    // printf("%d\n", resultCount);
    // for (int i = 0; i < resultCount; i++) {
    //     printf("%d %d\n", results[i][0], results[i][1]);
    // }

    test();

    return 0;
}
