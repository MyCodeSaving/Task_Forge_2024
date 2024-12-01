// Last modified: 24/11/29 19:00
// Author: lyingloong

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
        if (grid[x + i - 1][y + i] != '\\' || grid[x + i - 1][y - i + 1] != '/' ||
            grid[x + 2*half_size - i][y + i] != '/' || grid[x + 2*half_size - i][y - i + 1] != '\\') {
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
            for (int j = 1; j + size/2 <= C; j++) {
                if (is_valid_square(i, j, size/2)) {
                    count++;
                }
            }
        }
    }

    return count;
}

void run_test() {
    // 定义测试用例
    struct TestCase {
        int R, C;                   // 网格大小
        char grid[MAXN][MAXN];      // 网格内容
        int expected;               // 期望结果
    };

    struct TestCase tests[] = {
        // 测试用例1：无正方形
        {
            3, 3,
            {
                "/\\ ",
                " \\/",
                "   "
            },
            0
        },
        // 测试用例2：一个简单正方形
        {
            4, 4,
            {
                " /\\ ",
                "/  \\",
                "\\  /",
                " \\/ "
            },
            1
        },
        // 测试用例3：多个嵌套正方形
        {
            5, 5,
            {
                " /\\  ",
                "/\\/\\ ",
                "\\/\\/ ",
                " \\/  ",
                "     "
            },
            5
        },
        // 测试用例4：更复杂的情况
        {
            6, 6,
            {
                "      ",
                " /\\/\\ ",
                "/\\/\\/\\",
                "\\    /",
                " \\/\\/ ",
                "      "
            },
            2
        },
        // 测试用例5：无有效字符
        {
            3, 3,
            {
                "   ",
                "   ",
                "   "
            },
            0
        }
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    // 遍历测试用例
    for (int t = 0; t < num_tests; t++) {
        printf("Test %d: \n", t + 1);
        struct TestCase test = tests[t];

        // 设置 R 和 C
        R = test.R;
        C = test.C;

        // 复制网格
        memset(grid, 0, sizeof(grid));
        for (int i = 0; i < R; i++) {
            strcpy(grid[i+1] + 1, test.grid[i]);
            printf("%s\n", grid[i+1] + 1);
        }

        // 调用函数
        int result = count_squares();

        // 验证结果
        if (result == test.expected) {
            printf("PASSED (Result: %d)\n", result);
        } else {
            printf("FAILED (Expected: %d, Got: %d)\n", test.expected, result);
        }
    }
}

int main() {
    scanf("%d %d", &R, &C);

    for (int i = 1; i <= R; i++) {
        scanf("%s", grid[i] + 1); // 从 1 开始存储，方便处理边界
    }

    int result = count_squares();

    printf("%d\n", result);

    // run_test();
    return 0;
}
