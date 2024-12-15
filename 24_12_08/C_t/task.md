##### Description

现在给定两条字符串，如“abcdabc”和"abc"，要求字符串"abc"在"abcdabc"的最大字符串长度。最大字符串长度指的是"abc"在"abcdabc"连续出现的长度，当匹配的次数大于等于1的时候，不要求"abc"在后面也要保持完整。如"abcab"和“abc”的最大字符串长度为5。
字符串长度不大于10000。

##### Input

输入的第一个数字 *N* 代表测试样例数目，接下来有 2∗*N* 行字符串。
每两行字符串的第一个字符串为待匹配的字符串，第二个字符串为要匹配的字符串。

##### Output

题意要求的最大字符串匹配长度。

##### Sample Input

```
2
abcdabc
abc
abcdabcd
abcd
```

##### Sample Output

```
3
8
```

##### 参考逻辑（伪代码）

```c
// 暴力算法
int func(char* s, char* p) {
    int ans = 0;
    for (int i=0; i<l(s); i++) {
        int max_l = 0;
        int cur_l = 0;
        int j = 0;
        判断满不满足第一组符合；
        while (s[i+j+cur_l]==p[cur_l]) {
            cur_l++;
            if (cur_l>=l(p)) {
                cur_l = 0;
                j += cur_l;
            }
        }
        max_l = j+cur_l;
        ans = max(max_l,ans);
    }
}
```

##### 参考代码

```c
#include <stdio.h>
#include <string.h>

int l(const char *s) {
    return strlen(s);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int func(char* s, char* p) {
    int ans = 0;
    int s_len = l(s);
    int p_len = l(p);

    for (int i = 0; i < s_len; i++) {
        int max_l = 0;
        int cur_l = 0;
        int j = 0;

        while (i + j + cur_l < s_len && cur_l < p_len && s[i + j + cur_l] == p[cur_l]) {
            cur_l++;
            if (cur_l >= p_len) {
                cur_l = 0;
                j += p_len; 
            }
        }

        max_l =  j + cur_l; 

        if(max_l < l(p)) {
            max_l = 0; // 排除第一组都没有匹配的
        }
        ans = max(max_l, ans); 
    }
    return ans;
}

int main() {
    int n;
    scanf("%d", &n); 

    for (int i = 0; i < n; i++) {
        char s[10001], p[10001];
        scanf("%s", s); 
        scanf("%s", p); 

        printf("%d\n", func(s, p));
    }

    return 0;
}
```

##### **核心逻辑**

 `func`

- 输入：两个字符串 `s`（待匹配）和 `p`（模式）。
- 逻辑：
  1. 遍历 `s` 的每个起始位置 `i`。
  2. 尝试从当前位置匹配 p：
     - 使用变量 `cur_l` 表示当前匹配了 `p` 的前几个字符。
     - 如果 `cur_l` 达到 `p` 的长度（即 `p` 完整匹配），将其重置为0，并跳过完整匹配部分。
  3. 将当前匹配的总长度 `j + cur_l` 更新为 `max_l`。
  4. 若 `max_l` 小于 `p` 的长度（即没有匹配到任何完整 `p`），将 `max_l` 重置为0。
  5. 用 `ans` 保存所有起始位置中找到的最大匹配长度。