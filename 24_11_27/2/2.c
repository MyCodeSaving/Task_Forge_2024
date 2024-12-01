// Last modified: 24/11/29 20:00
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

// 暴力计算锤子中心点为 (x0, y0) 时的总价值
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

void test() {
    // 测试用例 1
    printf("Test 1:\n");
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

    // 计算结果
    maxValue = -2147483648;
    resultCount = 0;
    for (int x = 1; x <= n; x++) {
        for (int y = 1; y <= m; y++) {
            int currentValue = calculate_value(x, y);
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

    // 检查结果
    printf("MaxValue: %d\n", maxValue);
    printf("ResultCount: %d\n", resultCount);
    printf("Results:\n");
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }
    if (maxValue == 29)
        printf("MaxValue passed.\n");
    if (resultCount == 1)
        printf("ResultCount passed.\n");
    if (results[0][0] == 3 && results[0][1] == 2)
        printf("Result passed.\n");

    // 测试用例 2
    printf("Test 2:\n");
    n = 4; m = 4; d = 2;
    int test2[MAX_N][MAX_M] = {
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            value[i][j] = test2[i][j];
        }
    }

    // 计算结果
    maxValue = -2147483648;
    resultCount = 0;
    for (int x = 1; x <= n; x++) {
        for (int y = 1; y <= m; y++) {
            int currentValue = calculate_value(x, y);
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

    // 检查结果
    printf("MaxValue: %d\n", maxValue);
    printf("ResultCount: %d\n", resultCount);
    printf("Results:\n");
    for (int i = 0; i < resultCount; i++) {
        printf("%d %d\n", results[i][0], results[i][1]);
    }
    if (maxValue == 11)
        printf("MaxValue passed.\n");
    if (resultCount == 4)
        printf("ResultCount passed.\n");
}

int main() {
    // scanf("%d %d %d", &n, &m, &d);
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < m; j++) {
    //         scanf("%d", &value[i][j]);
    //     }
    // }

    // for (int x = 1; x <= n; x++) {
    //     for (int y = 1; y <= m; y++) {
    //         int currentValue = calculate_value(x, y);
    //         if (currentValue > maxValue) {
    //             // 如果找到新的最大值，更新最大值并清空结果列表
    //             maxValue = currentValue;
    //             resultCount = 0;
    //             results[resultCount][0] = x;
    //             results[resultCount][1] = y;
    //             resultCount++;
    //         } else if (currentValue == maxValue) {
    //             // 如果当前值等于最大值，添加到结果列表
    //             results[resultCount][0] = x;
    //             results[resultCount][1] = y;
    //             resultCount++;
    //         }
    //     }
    // }

    // printf("%d %d\n", maxValue, resultCount);
    // for (int i = 0; i < resultCount; i++) {
    //     printf("%d %d\n", results[i][0], results[i][1]);
    // }
    test();
    return 0;
}
