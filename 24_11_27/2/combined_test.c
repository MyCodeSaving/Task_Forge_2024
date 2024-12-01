#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_N 100
#define MAX_M 100

int n, m, d;                   // 地图大小 n x m 和锤子范围 d
int value[MAX_N][MAX_M];       // 保存地图上的地鼠价值
int maxValue = -2147483648;    // 保存最大价值，初始化为最小整数
int results[MAX_N * MAX_M][2]; // 保存所有能达到最大价值的中心点
int resultCount = 0;           // 最大价值的中心点数量

// 随机生成二维数组
void generate_random_array(int n_, int m_) {
    srand(time(NULL));
    // 随机生成数组的每个元素
    for (int i = 0; i < n_; i++) {
        for (int j = 0; j < m_; j++) {
            // 生成min_value到max_value之间的随机数
            value[i][j] = (rand() % MAX_N )* (rand() % 2 == 0 ? 1 : -1);
        }
    }
}

// 打印二维数组value
void print_array(int n_, int m_) {
    for (int i = 0; i < n_; i++) {
        for (int j = 0; j < m_; j++) {
            printf("%d ", value[i][j]);
        }
        printf("\n");
    }
}

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
    for(int i = 0; i < 10; i++){
        int maxValue_tmp;
        int resultCount_tmp;
        int results_tmp[MAX_N * MAX_M][2];

        printf("Test %d:\n", i+1);
        n = rand() % MAX_N;
        m = rand() % MAX_M;
        printf("n = %d, m = %d\n", n, m);
        generate_random_array(n, m);

        // printf("Method 1:\n");
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
        maxValue_tmp = maxValue;
        resultCount_tmp = resultCount;
        for (int i = 0; i < resultCount; i++) {
            results_tmp[i][0] = results[i][0];
            results_tmp[i][1] = results[i][1];
        }

        // printf("Method 2:\n");
        maxValue = -2147483648;
        resultCount = 0;
        sliding_window();

        if (maxValue_tmp != maxValue || resultCount_tmp != resultCount){
            printf("Error!\n");
            print_array(n, m);
            printf("Method 1:\n");
            printf("MaxValue: %d\n", maxValue_tmp);
            printf("ResultCount: %d\n", resultCount_tmp);
            printf("Results:\n");
            for (int i = 0; i < resultCount_tmp; i++) {
                printf("%d %d\n", results_tmp[i][0], results_tmp[i][1]);
            }
            printf("Method 2:\n");
            printf("MaxValue: %d\n", maxValue);
            printf("ResultCount: %d\n", resultCount);
            printf("Results:\n");
            for (int i = 0; i < resultCount; i++) {
                printf("%d %d\n", results[i][0], results[i][1]);
            }
            return;
        }
        printf("Passed!\n");
    }
}

int main() {
    test();
    return 0;
}