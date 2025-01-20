#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define USER_FILE "users.txt" // 用户信息保存的文件
#define BOOK_FILE "books.txt" // 书籍信息保存的文件

// 函数声明
void registerUser(); // 用户注册函数
int loginUser(int isAdmin); // 用户登录函数，isAdmin 表示是否为管理员登录
void queryBooks(); // 查询书籍信息
void borrowBook(); // 借阅书籍
void returnBook(); // 归还书籍
void manageBooks(); // 管理书籍
void manageUsers(); // 管理用户

int main() {
    int choice;
    while (1) {
        // 主菜单
        printf("\n图书借阅和管理系统\n");
        printf("1. 用户注册\n");
        printf("2. 用户登录\n");
        printf("3. 管理员登录\n");
        printf("4. 退出\n");
        printf("请选择: ");
        scanf("%d", &choice);
        getchar(); // 清除缓冲区换行符

        switch (choice) {
        case 1:
            registerUser(); // 用户注册
            break;
        case 2:
            if (loginUser(0)) { // 普通用户登录成功后
                int userChoice;
                while (1) {
                    printf("\n用户功能\n");
                    printf("1. 查询书籍\n");
                    printf("2. 借阅书籍\n");
                    printf("3. 归还书籍\n");
                    printf("4. 退出登录\n");
                    printf("请选择: ");
                    scanf("%d", &userChoice);
                    getchar();

                    if (userChoice == 1) {
                        queryBooks(); // 查询书籍
                    }
                    else if (userChoice == 2) {
                        borrowBook(); // 借阅书籍
                    }
                    else if (userChoice == 3) {
                        returnBook(); // 归还书籍
                    }
                    else {
                        break; // 退出登录
                    }
                }
            }
            break;
        case 3:
            if (loginUser(1)) { // 管理员登录成功后
                int adminChoice;
                while (1) {
                    printf("\n管理员功能\n");
                    printf("1. 管理书籍\n");
                    printf("2. 管理用户\n");
                    printf("3. 退出登录\n");
                    printf("请选择: ");
                    scanf("%d", &adminChoice);
                    getchar();

                    if (adminChoice == 1) {
                        manageBooks(); // 管理书籍
                    }
                    else if (adminChoice == 2) {
                        manageUsers(); // 管理用户
                    }
                    else {
                        break; // 退出登录
                    }
                }
            }
            break;
        case 4:
            exit(0); // 退出程序
        default:
            printf("无效选择，请重试。\n");
        }
    }
    return 0;
}

// 用户注册函数
void registerUser() {
    char username[MAX_LINE], password[MAX_LINE];
    printf("请输入用户名: ");
    fgets(username, MAX_LINE, stdin);
    username[strcspn(username, "\n")] = 0; // 去除换行符

    printf("请输入密码: ");
    fgets(password, MAX_LINE, stdin);
    password[strcspn(password, "\n")] = 0; // 去除换行符

    // 将用户信息写入文件
    FILE* file = fopen(USER_FILE, "a");
    if (file == NULL) {
        perror("无法打开用户文件");
        return;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("注册成功！\n");
}

// 用户登录函数
int loginUser(int isAdmin) {
    char username[MAX_LINE], password[MAX_LINE], fileUser[MAX_LINE], filePass[MAX_LINE];
    printf("请输入用户名: ");
    fgets(username, MAX_LINE, stdin);
    username[strcspn(username, "\n")] = 0; // 去除换行符

    printf("请输入密码: ");
    fgets(password, MAX_LINE, stdin);
    password[strcspn(password, "\n")] = 0; // 去除换行符

    // 打开用户文件进行验证
    FILE* file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("无法打开用户文件");
        return 0;
    }

    // 遍历用户文件查找匹配的用户名和密码
    while (fscanf(file, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            fclose(file);
            if (isAdmin && strcmp(username, "admin") != 0) {
                // 检查是否为管理员登录
                printf("非管理员账户无法登录。\n");
                return 0;
            }
            printf("登录成功！\n");
            return 1;
        }
    }
    fclose(file);
    printf("用户名或密码错误。\n");
    return 0;
}

// 查询书籍信息
void queryBooks() {
    char line[MAX_LINE];
    FILE* file = fopen(BOOK_FILE, "r");
    if (file == NULL) {
        perror("无法打开书籍文件");
        return;
    }

    printf("\n书籍列表:\n");
    while (fgets(line, MAX_LINE, file)) {
        printf("%s", line); // 输出每本书的信息
    }
    fclose(file);
}

// 借阅书籍
void borrowBook() {
    char bookName[MAX_LINE], line[MAX_LINE], tempFile[] = "temp.txt";
    int found = 0;

    printf("请输入要借阅的书名: ");
    fgets(bookName, MAX_LINE, stdin);
    bookName[strcspn(bookName, "\n")] = 0; // 去除换行符

    FILE* file = fopen(BOOK_FILE, "r");
    FILE* temp = fopen(tempFile, "w");
    if (file == NULL || temp == NULL) {
        perror("文件操作失败");
        return;
    }

    // 查找指定书籍并更新状态
    while (fgets(line, MAX_LINE, file)) {
        char name[MAX_LINE], status[MAX_LINE];
        sscanf(line, "%[^|]|%s", name, status);
        if (strcmp(name, bookName) == 0 && strcmp(status, "[可借]") == 0) {
            fprintf(temp, "%s|[已借]\n", name); // 更新为已借状态
            printf("书籍 \"%s\" 借阅成功！\n", bookName);
            found = 1;
        }
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("未找到指定书籍或书籍已被借出。\n");
        remove(tempFile); // 删除临时文件
    }
    else {
        remove(BOOK_FILE); // 删除原文件
        rename(tempFile, BOOK_FILE); // 替换为更新后的文件
    }
}


// 归还书籍
void returnBook() {
    char bookName[MAX_LINE], line[MAX_LINE], tempFile[] = "temp.txt";
    int found = 0;

    printf("请输入要归还的书名: ");
    fgets(bookName, MAX_LINE, stdin);
    bookName[strcspn(bookName, "\n")] = 0; // 去除换行符

    FILE* file = fopen(BOOK_FILE, "r");
    FILE* temp = fopen(tempFile, "w");
    if (file == NULL || temp == NULL) {
        perror("文件操作失败");
        return;
    }

    // 查找指定书籍并更新状态
    while (fgets(line, MAX_LINE, file)) {
        char name[MAX_LINE], status[MAX_LINE];
        sscanf(line, "%[^|]|%s", name, status);
        if (strcmp(name, bookName) == 0 && strcmp(status, "[已借]") == 0) {
            fprintf(temp, "%s|[可借]\n", name); // 更新为可借状态
            printf("书籍 \"%s\" 归还成功！\n", bookName);
            found = 1;
        }
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("未找到指定书籍或书籍未被借出。\n");
        remove(tempFile); // 删除临时文件
    }
    else {
        remove(BOOK_FILE); // 删除原文件
        rename(tempFile, BOOK_FILE); // 替换为更新后的文件
    }
}


// 管理书籍
void manageBooks() {
    int choice;
    char bookName[MAX_LINE];

    printf("\n1. 添加书籍\n");
    printf("2. 删除书籍\n");
    printf("请选择: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("请输入书名: ");
        fgets(bookName, MAX_LINE, stdin);
        bookName[strcspn(bookName, "\n")] = 0; // 去除换行符

        // 添加书籍到文件
        FILE* file = fopen(BOOK_FILE, "a");
        if (file == NULL) {
            perror("无法打开书籍文件");
            return;
        }
        fprintf(file, "%s|[可借]\n", bookName);
        fclose(file);
        printf("书籍添加成功！\n");
    }
    else if (choice == 2) {
        char line[MAX_LINE], tempFile[] = "temp.txt";
        int found = 0;

        printf("请输入要删除的书名: ");
        fgets(bookName, MAX_LINE, stdin);
        bookName[strcspn(bookName, "\n")] = 0; // 去除换行符

        FILE* file = fopen(BOOK_FILE, "r");
        FILE* temp = fopen(tempFile, "w");
        if (file == NULL || temp == NULL) {
            perror("文件操作失败");
            return;
        }

        // 查找并删除指定书籍
        while (fgets(line, MAX_LINE, file)) {
            char name[MAX_LINE], status[MAX_LINE];
            sscanf(line, "%[^|]|%s", name, status);
            if (strcmp(name, bookName) != 0) {
                fprintf(temp, "%s", line);
            }
            else {
                found = 1;
            }
        }

        fclose(file);
        fclose(temp);

        if (!found) {
            printf("未找到指定书籍。\n");
            remove(tempFile); // 删除临时文件
        }
        else {
            remove(BOOK_FILE); // 删除原文件
            rename(tempFile, BOOK_FILE); // 替换为更新后的文件
            printf("书籍删除成功！\n");
        }
    }
    else {
        printf("无效选择\n");
    }
}


// 管理用户
void manageUsers() {
    char line[MAX_LINE];
    printf("\n用户列表:\n");
    FILE* file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("无法打开用户文件");
        return;
    }

    // 输出所有用户信息
    while (fgets(line, MAX_LINE, file)) {
        printf("%s", line);
    }
    fclose(file);
}

