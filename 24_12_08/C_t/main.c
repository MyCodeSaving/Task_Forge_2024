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
