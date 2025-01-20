### 题目描述：

在一个王国中，每年都有一位神秘的日历大师，他负责记录每一天的具体信息。你作为这位日历大师的助手，需要帮助他计算一年中每一天的排名。

每年，王国的居民都会根据不同的日期庆祝各类节日，但为了避免搞错每个节日是哪一天，日历大师希望知道某个特定日期在这一年中是第几天。例如，如果某个日期是1月15日，那么它是这一年的第15天。

##### 示例1：

输入：

```
2010 10 1
```

输出:

```
10/1 is the 274th day in 2010
```

##### 示例2：

输入：

```
2022 12 31
```

输出:

```
12/31 is the 365th day in 2022
```

###### 提示

输入分别为年 月 日

注意闰月的存在。
润月的规则是: 年份能被4或者400整除但不能被100整除。

注意: 主函数已经定义完成，只完成day_of_year.h和day_of_year.c即可。
请根据主函数中的提示，自行构建结构体。

```c
#include <stdio.h>
#include "day_of_year.h"


int main() {
    struct Date date;
    
  
    scanf("%d %d %d", &date.year, &date.month, &date.day);

    // 调用函数计算第几天
    int days = calculate_day_of_year(date);

    // 输出结果
    printf("%d/%d is the %dth day in %d\n", date.month, date.day, days, date.year);
    
    return 0;
}
```

##### 参考代码

```
#ifndef DAY_OF_YEAR_H
#define DAY_OF_YEAR_H

// 定义日期结构体
struct Date {
    int year;
    int month;
    int day;
};

// 声明计算函数
int calculate_day_of_year(struct Date date);

#endif // DAY_OF_YEAR_H
```

```
#include "day_of_year.h"

// 判断是否是闰年
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 每月的天数
int days_in_month[] = { 
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 
};

// 计算第几天
int calculate_day_of_year(struct Date date) {
    int total_days = 0;

    // 如果是闰年，2月有29天
    if (is_leap_year(date.year)) {
        days_in_month[1] = 29;
    } else {
        days_in_month[1] = 28;
    }

    // 累加当前月份之前的天数
    for (int i = 0; i < date.month - 1; i++) {
        total_days += days_in_month[i];
    }

    // 加上当前月的天数
    total_days += date.day;

    return total_days;
}
```

### 题目描述：

你的学校需要建立一个小型学生信息管理系统，该系统需要处理学生的基本信息，包括姓名、年龄、学号和成绩（成绩保留一位小数）。现在你需要输入n个学生的信息，并打印成绩最高的学生信息。

#### 示例

##### 输入

```
3
张三
20
1001
85.5
李四
22
1002
90.0
王五
21
1003
88.0

```

##### 输入描述

学生数量
第一个学生的名字
第一个学生的年龄
第一个学生的学号
第一个学生的成绩
第二个学生的名字
.....(以此类推)

##### 输出

```
成绩最高的学生信息： 
姓名: 李四
年龄: 22
学号: 1002
成绩: 90.0

```

#### 提示

- 定义一个名为 Student 的结构体，用来存储以下学生信息：
  - 姓名（`name`，最大长度 50 字符）
  - 年龄（`age`，整数类型）
  - 学号（`id`，整数类型）
  - 成绩（`score`，浮点型）
- 编写一个函数 `printStudentInfo`，接受一个 `Student` 类型的结构体变量，输出该学生的所有信息。
- 编写一个函数 `getTopStudent`，接受一个学生数组和学生数量作为参数，返回成绩最高的学生信息（如果有多个成绩相同的最高分，返回第一个）。

##### 给出的 main 函数

```c
#include <stdio.h>
#include "student.h"

int main() {

    // 创建学生数据
    int n;
    scanf("%d", &n);
    Student students[n];
    // 输入学生信息
    for (int i = 0; i < n; i++) {
        getchar(); // 清除输入缓冲区中的换行符
        
        // 输入学生姓名
        scanf("%[^\n]%*c", students[i].name);

        // 输入年龄
        scanf("%d", &students[i].age);

        // 输入学号
        scanf("%d", &students[i].id);

        // 输入成绩
        scanf("%f", &students[i].score);
    }

    // 获取成绩最高的学生
    Student topStudent = getTopStudent(students, n);

    // 输出成绩最高的学生信息
    printf("成绩最高的学生信息：\n");
    printStudentInfo(topStudent);

    return 0;
}
```

##### 参考代码

```
#ifndef STUDENT_H
#define STUDENT_H

// 定义 Student 结构体
typedef struct {
    char name[51];  // 学生姓名，最大50字符 + '\0'
    int age;        // 学生年龄
    int id;         // 学生学号
    float score;    // 学生成绩
} Student;

// 声明函数
void printStudentInfo(Student student);
Student getTopStudent(Student students[], int n);

#endif // STUDENT_H
```

```
#include <stdio.h>
#include "student.h"

// 输出学生信息
void printStudentInfo(Student student) {
    printf("姓名: %s\n", student.name);
    printf("年龄: %d\n", student.age);
    printf("学号: %d\n", student.id);
    printf("成绩: %.1f\n", student.score);
}

// 获取成绩最高的学生
Student getTopStudent(Student students[], int n) {
    Student topStudent = students[0]; // 假设第一个学生成绩最高

    for (int i = 1; i < n; i++) {
        if (students[i].score > topStudent.score) {
            topStudent = students[i]; // 更新最高成绩学生
        }
    }

    return topStudent;
}
```

