#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define USER_FILE "users.txt" // �û���Ϣ������ļ�
#define BOOK_FILE "books.txt" // �鼮��Ϣ������ļ�

// ��������
void registerUser(); // �û�ע�ắ��
int loginUser(int isAdmin); // �û���¼������isAdmin ��ʾ�Ƿ�Ϊ����Ա��¼
void queryBooks(); // ��ѯ�鼮��Ϣ
void borrowBook(); // �����鼮
void returnBook(); // �黹�鼮
void manageBooks(); // �����鼮
void manageUsers(); // �����û�

int main() {
    int choice;
    while (1) {
        // ���˵�
        printf("\nͼ����ĺ͹���ϵͳ\n");
        printf("1. �û�ע��\n");
        printf("2. �û���¼\n");
        printf("3. ����Ա��¼\n");
        printf("4. �˳�\n");
        printf("��ѡ��: ");
        scanf("%d", &choice);
        getchar(); // ������������з�

        switch (choice) {
        case 1:
            registerUser(); // �û�ע��
            break;
        case 2:
            if (loginUser(0)) { // ��ͨ�û���¼�ɹ���
                int userChoice;
                while (1) {
                    printf("\n�û�����\n");
                    printf("1. ��ѯ�鼮\n");
                    printf("2. �����鼮\n");
                    printf("3. �黹�鼮\n");
                    printf("4. �˳���¼\n");
                    printf("��ѡ��: ");
                    scanf("%d", &userChoice);
                    getchar();

                    if (userChoice == 1) {
                        queryBooks(); // ��ѯ�鼮
                    }
                    else if (userChoice == 2) {
                        borrowBook(); // �����鼮
                    }
                    else if (userChoice == 3) {
                        returnBook(); // �黹�鼮
                    }
                    else {
                        break; // �˳���¼
                    }
                }
            }
            break;
        case 3:
            if (loginUser(1)) { // ����Ա��¼�ɹ���
                int adminChoice;
                while (1) {
                    printf("\n����Ա����\n");
                    printf("1. �����鼮\n");
                    printf("2. �����û�\n");
                    printf("3. �˳���¼\n");
                    printf("��ѡ��: ");
                    scanf("%d", &adminChoice);
                    getchar();

                    if (adminChoice == 1) {
                        manageBooks(); // �����鼮
                    }
                    else if (adminChoice == 2) {
                        manageUsers(); // �����û�
                    }
                    else {
                        break; // �˳���¼
                    }
                }
            }
            break;
        case 4:
            exit(0); // �˳�����
        default:
            printf("��Чѡ�������ԡ�\n");
        }
    }
    return 0;
}

// �û�ע�ắ��
void registerUser() {
    char username[MAX_LINE], password[MAX_LINE];
    printf("�������û���: ");
    fgets(username, MAX_LINE, stdin);
    username[strcspn(username, "\n")] = 0; // ȥ�����з�

    printf("����������: ");
    fgets(password, MAX_LINE, stdin);
    password[strcspn(password, "\n")] = 0; // ȥ�����з�

    // ���û���Ϣд���ļ�
    FILE* file = fopen(USER_FILE, "a");
    if (file == NULL) {
        perror("�޷����û��ļ�");
        return;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("ע��ɹ���\n");
}

// �û���¼����
int loginUser(int isAdmin) {
    char username[MAX_LINE], password[MAX_LINE], fileUser[MAX_LINE], filePass[MAX_LINE];
    printf("�������û���: ");
    fgets(username, MAX_LINE, stdin);
    username[strcspn(username, "\n")] = 0; // ȥ�����з�

    printf("����������: ");
    fgets(password, MAX_LINE, stdin);
    password[strcspn(password, "\n")] = 0; // ȥ�����з�

    // ���û��ļ�������֤
    FILE* file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("�޷����û��ļ�");
        return 0;
    }

    // �����û��ļ�����ƥ����û���������
    while (fscanf(file, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            fclose(file);
            if (isAdmin && strcmp(username, "admin") != 0) {
                // ����Ƿ�Ϊ����Ա��¼
                printf("�ǹ���Ա�˻��޷���¼��\n");
                return 0;
            }
            printf("��¼�ɹ���\n");
            return 1;
        }
    }
    fclose(file);
    printf("�û������������\n");
    return 0;
}

// ��ѯ�鼮��Ϣ
void queryBooks() {
    char line[MAX_LINE];
    FILE* file = fopen(BOOK_FILE, "r");
    if (file == NULL) {
        perror("�޷����鼮�ļ�");
        return;
    }

    printf("\n�鼮�б�:\n");
    while (fgets(line, MAX_LINE, file)) {
        printf("%s", line); // ���ÿ�������Ϣ
    }
    fclose(file);
}

// �����鼮
void borrowBook() {
    char bookName[MAX_LINE], line[MAX_LINE], tempFile[] = "temp.txt";
    int found = 0;

    printf("������Ҫ���ĵ�����: ");
    fgets(bookName, MAX_LINE, stdin);
    bookName[strcspn(bookName, "\n")] = 0; // ȥ�����з�

    FILE* file = fopen(BOOK_FILE, "r");
    FILE* temp = fopen(tempFile, "w");
    if (file == NULL || temp == NULL) {
        perror("�ļ�����ʧ��");
        return;
    }

    // ����ָ���鼮������״̬
    while (fgets(line, MAX_LINE, file)) {
        char name[MAX_LINE], status[MAX_LINE];
        sscanf(line, "%[^|]|%s", name, status);
        if (strcmp(name, bookName) == 0 && strcmp(status, "[�ɽ�]") == 0) {
            fprintf(temp, "%s|[�ѽ�]\n", name); // ����Ϊ�ѽ�״̬
            printf("�鼮 \"%s\" ���ĳɹ���\n", bookName);
            found = 1;
        }
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("δ�ҵ�ָ���鼮���鼮�ѱ������\n");
        remove(tempFile); // ɾ����ʱ�ļ�
    }
    else {
        remove(BOOK_FILE); // ɾ��ԭ�ļ�
        rename(tempFile, BOOK_FILE); // �滻Ϊ���º���ļ�
    }
}


// �黹�鼮
void returnBook() {
    char bookName[MAX_LINE], line[MAX_LINE], tempFile[] = "temp.txt";
    int found = 0;

    printf("������Ҫ�黹������: ");
    fgets(bookName, MAX_LINE, stdin);
    bookName[strcspn(bookName, "\n")] = 0; // ȥ�����з�

    FILE* file = fopen(BOOK_FILE, "r");
    FILE* temp = fopen(tempFile, "w");
    if (file == NULL || temp == NULL) {
        perror("�ļ�����ʧ��");
        return;
    }

    // ����ָ���鼮������״̬
    while (fgets(line, MAX_LINE, file)) {
        char name[MAX_LINE], status[MAX_LINE];
        sscanf(line, "%[^|]|%s", name, status);
        if (strcmp(name, bookName) == 0 && strcmp(status, "[�ѽ�]") == 0) {
            fprintf(temp, "%s|[�ɽ�]\n", name); // ����Ϊ�ɽ�״̬
            printf("�鼮 \"%s\" �黹�ɹ���\n", bookName);
            found = 1;
        }
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("δ�ҵ�ָ���鼮���鼮δ�������\n");
        remove(tempFile); // ɾ����ʱ�ļ�
    }
    else {
        remove(BOOK_FILE); // ɾ��ԭ�ļ�
        rename(tempFile, BOOK_FILE); // �滻Ϊ���º���ļ�
    }
}


// �����鼮
void manageBooks() {
    int choice;
    char bookName[MAX_LINE];

    printf("\n1. ����鼮\n");
    printf("2. ɾ���鼮\n");
    printf("��ѡ��: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("����������: ");
        fgets(bookName, MAX_LINE, stdin);
        bookName[strcspn(bookName, "\n")] = 0; // ȥ�����з�

        // ����鼮���ļ�
        FILE* file = fopen(BOOK_FILE, "a");
        if (file == NULL) {
            perror("�޷����鼮�ļ�");
            return;
        }
        fprintf(file, "%s|[�ɽ�]\n", bookName);
        fclose(file);
        printf("�鼮��ӳɹ���\n");
    }
    else if (choice == 2) {
        char line[MAX_LINE], tempFile[] = "temp.txt";
        int found = 0;

        printf("������Ҫɾ��������: ");
        fgets(bookName, MAX_LINE, stdin);
        bookName[strcspn(bookName, "\n")] = 0; // ȥ�����з�

        FILE* file = fopen(BOOK_FILE, "r");
        FILE* temp = fopen(tempFile, "w");
        if (file == NULL || temp == NULL) {
            perror("�ļ�����ʧ��");
            return;
        }

        // ���Ҳ�ɾ��ָ���鼮
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
            printf("δ�ҵ�ָ���鼮��\n");
            remove(tempFile); // ɾ����ʱ�ļ�
        }
        else {
            remove(BOOK_FILE); // ɾ��ԭ�ļ�
            rename(tempFile, BOOK_FILE); // �滻Ϊ���º���ļ�
            printf("�鼮ɾ���ɹ���\n");
        }
    }
    else {
        printf("��Чѡ��\n");
    }
}


// �����û�
void manageUsers() {
    char line[MAX_LINE];
    printf("\n�û��б�:\n");
    FILE* file = fopen(USER_FILE, "r");
    if (file == NULL) {
        perror("�޷����û��ļ�");
        return;
    }

    // ��������û���Ϣ
    while (fgets(line, MAX_LINE, file)) {
        printf("%s", line);
    }
    fclose(file);
}

