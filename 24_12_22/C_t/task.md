## 1

#### 题目描述

设计一个C语言程序，要求实现以下功能：

1. 动态创建一个单链表，存储用户输入的正整数。
2. 输出链表中的所有节点数据，格式为`值1->值2->...->值n->NULL`.
3. 接收用户输入的一个新的正整数和一个插入位置，将该值插入到链表的指定位置。
4. 输出插入后的链表中所有节点数据。

#### 输入描述

1. 第一行输入正整数n，范围为[1, 50]，表示用户输入的数据个数。
2. 第二行输入用户输入的所有数据。
3. 第三行输入一个新数据和插入位置

#### 输出描述

1. 第一行输出创建的链表中所有节点数据。
2. 第二行输出插入后的链表中所有节点数据。

#### 输入示例

```
5
1 2 3 4 5
10 3
```

#### 输出示例

```
1->2->3->4->5->NULL
1->2->10->3->4->5->NULL
```

#### 提示

插入操作需要考虑三种情况：插入到链表头部、中间或尾部。

##### main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main() {
    int n;
    scanf("%d", &n);

    struct Node* head = createList(n);
    printList(head);

    int value, pos;
    scanf("%d%d", &value, &pos);

    head = insertList(head, value, pos);
    printList(head);

    return 0;
}
```

##### func.h

```c
struct Node {
    int data;
    struct Node* next;
};

struct Node* createList(int n);
void printList(struct Node* head);
struct Node* insertList(struct Node* head, int value, int pos);
```

#### func.c

```c
#include <stdio.h>
#include <stdlib.h>
#include "func.h"

// 创建一个单链表
struct Node* createList(int n) {
    struct Node* head = NULL;
    struct Node* tail = NULL;
    
    for (int i = 0; i < n; i++) {
        int value;
        scanf("%d", &value);

        // 创建一个新节点
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = value;
        newNode->next = NULL;

        if (head == NULL) {
            // 如果链表为空，新节点作为头节点
            head = newNode;
        } else {
            // 否则，将新节点链接到链表尾部
            tail->next = newNode;
        }
        tail = newNode; // 更新尾节点
    }

    return head;
}

// 输出链表中的所有节点数据
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf("->");
        }
        current = current->next;
    }
    printf("->NULL\n");
}

// 在链表中插入一个新节点
struct Node* insertList(struct Node* head, int value, int pos) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (pos == 1) {
        // 插入到头部
        newNode->next = head;
        head = newNode;
        return head;
    }

    struct Node* current = head;
    for (int i = 1; i < pos - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        // 如果位置无效，插入到链表尾部
        free(newNode); // 防止内存泄漏
        return head;
    }

    // 插入到中间或尾部
    newNode->next = current->next;
    current->next = newNode;

    return head;
}
```

## 2

#### 问题描述

要求设计一个C语言函数，实现删除链表中所有重复元素的节点，使每个元素只出现一次，返回无重复元素的链表（不改变原始节点相对顺序，且重复元素按从左至右顺序保留第一个）

#### 输入描述

1. 第一行输入链表节点个数`n`，范围为[2, 50]
2. 第二行输入所有链表节点数据，范围为[1, 100]

#### 输出描述

1. 第一行输出原始链表
2. 第二行输出无重复元素的链表

#### 输入示例

```
5
10 20 10 30 30
```

#### 输出示例

```
10->20->10->30->30->NULL
10->20->30->NULL
```

##### main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main() {
    struct Node *head = NULL, *tail = NULL, *newNode;
    int n, value;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &value);

        newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = value;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    struct Node* current = head;
    while (current != NULL) {
        printf("%d->", current->data);
        current = current->next;
    }
    printf("NULL\n");

 
    deleteDuplicates(head);

    current = head;
    while (current != NULL) {
        printf("%d->", current->data);
        current = current->next;
    }
    printf("NULL\n");

    return 0;
}
```

##### func.h

```c
struct Node {
    int data;
    struct Node* next;
};

void deleteDuplicates(struct Node* head); 
```

#### func.c

```c
#include <stdio.h>
#include <stdlib.h>
#include "func.h"

// 删除链表中所有重复的节点
void deleteDuplicates(struct Node* head) {
    if (head == NULL) return;

    struct Node* outer = head; // 外层循环指针，遍历链表
    while (outer != NULL && outer->next != NULL) {
        struct Node* inner = outer;
        while (inner->next != NULL) {
            if (outer->data == inner->next->data) {
                // 删除重复节点
                struct Node* temp = inner->next;
                inner->next = inner->next->next;
                free(temp); // 释放重复节点的内存
            } else {
                inner = inner->next;
            }
        }
        outer = outer->next;
    }
}
```

## 3

#### 题目描述

##### **反转链表的指定区间**

给定一个单链表和两个整数 `m` 和 `n`，其中 `1 <= m <= n <= 链表长度`，你需要将链表中从位置 `m` 到位置 `n` 之间的节点反转，并返回链表的头节点。

#### 输入输出

- **第一行输入**：链表的数值，使用空格分隔。
- **第二行输入**：两个整数 `m` 和 `n`，使用空格分隔，代表链表反转的区间。
- **输出**：反转后的链表结果，输出为一行数值，用空格分隔。

##### **示例 1:**

输入:

```
1 2 3 4 5
2 4

```

输出：

```
1 4 3 2 5

```

##### **示例 2:**

输入:

```
1 2 3 4 5
3 5

```

输出：

```
1 2 5 4 3

```

#### 提示：

- 假设输入的数据是有效的，且链表的长度至少为 `n`。

#### 解题思路：

1. **找到链表的第 `m-1` 个节点**，以便反转过程中的连接操作。
2. **反转链表的 `m` 到 `n` 区间**，可以通过逐步调整节点的 `next` 指针来实现。
3. **维护一个指向 `m-1` 节点的指针**，它在反转操作完成后需要指向反转后的 `m` 节点。
4. **反转后，连接起始节点**，最后将原链表尾部连接好。

#### main.c

```c
#include <stdio.h>
#include <stdlib.h>

// 链表节点结构
struct Node {
    int data;
    struct Node* next;
};

// 创建链表
struct Node* createList(int arr[], int size) {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    for (int i = 0; i < size; i++) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = arr[i];
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}

// 打印链表
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("\n");
}

// 反转链表的指定区间
struct Node* reverseBetween(struct Node* head, int m, int n) {
    if (head == NULL || m == n) return head;

    struct Node dummy;
    dummy.next = head;
    struct Node* prev = &dummy;

    // 找到 m-1 的前一个节点
    for (int i = 1; i < m; i++) {
        prev = prev->next;
    }

    struct Node* start = prev->next; // m 节点
    struct Node* then = start->next; // m+1 节点

    // 逐步反转 m 到 n 的节点
    for (int i = 0; i < n - m; i++) {
        start->next = then->next;
        then->next = prev->next;
        prev->next = then;
        then = start->next;
    }

    return dummy.next;
}

int main() {
    // 输入链表
    int arr[50], size = 0;
    while (scanf("%d", &arr[size]) == 1) {
        size++;
        if (getchar() == '\n') break;
    }

    // 输入 m 和 n
    int m, n;
    scanf("%d %d", &m, &n);

    // 创建链表
    struct Node* head = createList(arr, size);

    // // 反转前链表输出
    // printList(head);

    // 反转链表指定区间
    head = reverseBetween(head, m, n);

    // 反转后链表输出
    printList(head);

    return 0;
}
```

## 4

#### 题目描述

##### **合并两个有序链表**

给定两个有序链表 `L1` 和 `L2`，你需要将这两个链表合并成一个有序链表。其中，输入链表节点的值是整数，且链表中的节点值是升序排列的；输出链表也应该是升序的。

#### 输入输出

- **第一行输入**：第一个链表的数值，使用空格分隔。
- **第二行输入**：第二个链表的数值，使用空格分隔。
- **输出**：合并后的链表结果，输出为一行数值，用空格分隔。

##### **示例 1:**

输入:

```
1 2 3
1 3 4
```

输出：

```
1 1 2 3 4 4
```

##### **示例 2:**

输入:

```
1
2
```

输出：

```
1 2
```

#### 解题思路：

可以通过使用两个指针，分别指向两个链表的头节点，比较它们的值，并将较小的节点加入到新链表中。遍历两个链表，直到遍历完其中一个链表。之后，直接将剩余链表的部分接到新链表的末尾。

#### main.c

```c
#include <stdio.h>
#include <stdlib.h>

// 链表节点结构
struct Node {
    int data;
    struct Node* next;
};

// 创建链表
struct Node* createList(int arr[], int size) {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    for (int i = 0; i < size; i++) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = arr[i];
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}

// 打印链表
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("\n");
}

// 合并两个有序链表
struct Node* mergeLists(struct Node* L1, struct Node* L2) {
    struct Node dummy;
    struct Node* tail = &dummy;
    dummy.next = NULL;

    // 遍历两个链表并比较节点值
    while (L1 != NULL && L2 != NULL) {
        if (L1->data < L2->data) {
            tail->next = L1;
            L1 = L1->next;
        } else {
            tail->next = L2;
            L2 = L2->next;
        }
        tail = tail->next;
    }

    // 如果链表L1或L2还有剩余部分，直接连接到结果链表的末尾
    if (L1 != NULL) {
        tail->next = L1;
    } else {
        tail->next = L2;
    }

    return dummy.next;
}

int main() {
    // 输入第一个链表
    int arr1[50], size1 = 0;
    while (scanf("%d", &arr1[size1]) == 1) {
        size1++;
        if (getchar() == '\n') break;
    }

    // 输入第二个链表
    int arr2[50], size2 = 0;
    while (scanf("%d", &arr2[size2]) == 1) {
        size2++;
        if (getchar() == '\n') break;
    }

    // 创建两个链表
    struct Node* L1 = createList(arr1, size1);
    struct Node* L2 = createList(arr2, size2);

    // 合并链表
    struct Node* merged = mergeLists(L1, L2);

    // 打印合并后的链表
    printList(merged);

    return 0;
}
```

