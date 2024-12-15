// ʹ��ʱ��ʹ��Ӣ�Ļ���ƴ������������޷�������ʾ��

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <wchar.h>
// #include <locale.h>
// #include <windows.h>

// ������Ʒ��Ϣ�ṹ
typedef struct goods {
    char code[20];        // ��Ʒ����
    char name[50];        // ��Ʒ����
    char category[30];    // ��Ʒ���
    char unit[10];        // ������λ
    float price;             // ����
    int min_stock;           // ��Ϳ��
    int max_stock;           // ��߿��
} Goods;

// ��������Ϣ�ṹ
typedef struct stock {
    char goods_code[20];  // ��Ʒ����
    int quantity;            // �������
    char location[30];    // ��λ
    char batch_no[20];    // ���κ�
    char expire_date[20]; // ��Ч��
} Stock;

// ���������¼�ṹ
typedef struct record {
    char record_id[20];   // ��¼���
    char goods_code[20];  // ��Ʒ����
    int quantity;            // ����
    int type;                // ���ͣ�1����⣬2�����⣩
    char operator[50];    // ����Ա
    char date[20];        // ����ʱ��
    char note[100];       // ��ע
} Record;

// �ļ�·������
#define GOODS_FILE "goods.txt"
#define STOCK_FILE "stock.txt"
#define RECORD_FILE "record.txt"
#define CATEGORY_FILE "categories.txt"
#define QC_FILE "quality_check.txt"
#define LOCATION_FILE "locations.txt"

// ��������
void menu();
void goods_menu();
void stock_menu();
void record_menu();
void statistics_menu();

void add_goods();
void view_goods();
void delete_goods();
void modify_goods();
void classify_goods();

void add_category();
void view_categories();
void delete_category();

void add_stock();
void view_stock();

void record_entry();
void view_records();
void record_exit();

void stock_analysis(); // ����̵�
// void allocate_stock(); // ��λ����
// void quality_check(); // �ʼ��¼
void inventory_warning(); // ���Ԥ��
void turnover_rate_analysis(); // ��Ʒ��ת��ͳ��

// ������
int main() {
    // SetConsoleOutputCP(65001); // ֧���������
    // setlocale(LC_ALL, ""); // ֧��������ʾ
    int choice;
    do {
        menu();
        printf("������ѡ�");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: goods_menu(); break;
        case 2: stock_menu(); break;
        case 3: record_menu(); break;
        case 4: statistics_menu(); break;
        case 5: printf("�˳�ϵͳ\n"); break;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 5);
    return 0;
}

// ���˵�
void menu() {
    printf("\n=========== �ֿ����ϵͳ ===========\n");
    printf("1. ��Ʒ����\n");
    printf("2. ������\n");
    printf("3. ������¼����\n");
    printf("4. ͳ�Ʒ���\n");
    printf("5. �˳�ϵͳ\n");
    printf("===================================\n");
}

// ��Ʒ����˵�
void goods_menu() {
    int choice;
    do {
        printf("\n--- ��Ʒ���� ---\n");
        printf("1. �����Ʒ\n");
        printf("2. �鿴��Ʒ\n");
        printf("3. ɾ����Ʒ\n");
        printf("4. �޸���Ʒ��Ϣ\n");
        printf("5. �������\n");
        printf("6. �������˵�\n");
        printf("��ѡ��");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: add_goods(); break;
        case 2: view_goods(); break;
        case 3: delete_goods(); break;
        case 4: modify_goods(); break;
        case 5: classify_goods(); break;
        case 6: return;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 6);
}

// ������˵�
void stock_menu() {
    int choice;
    do {
        printf("\n--- ������ ---\n");
        printf("1. ���Ǽ�\n");
        printf("2. �鿴���\n");
        printf("3. �������˵�\n");
        printf("��ѡ��");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: add_stock(); break;
        case 2: view_stock(); break;
        case 3: return;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 3);
}

// ������¼����˵�
void record_menu() {
    int choice;
    do {
        printf("\n--- ������¼���� ---\n");
        printf("1. ��Ӽ�¼\n");
        printf("2. �鿴��¼\n");
        printf("3. �������˵�\n");
        printf("��ѡ��");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: record_entry(); break;
        case 2: view_records(); break;
        case 3: record_exit(); break;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 3);
}

// ͳ�Ʒ����˵�
void statistics_menu() {
    int choice;
    do {
        printf("\n--- ͳ�Ʒ��� ---\n");
        printf("1. ����̵�\n");
        printf("2. ���Ԥ��\n");
        printf("3. ��Ʒ��ת��ͳ��\n");
        printf("4. �������˵�\n");
        printf("��ѡ��");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: stock_analysis(); break;
        case 2: inventory_warning(); break;
        case 3: turnover_rate_analysis(); break;
        case 4: return;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 4);
}

// �����Ʒ
void add_goods() {
    FILE* file = fopen("goods.txt", "a");
    if (file == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }
    Goods g;
    printf("������Ʒ���룺");
    scanf_s("%s", g.code ,20 );
    printf("������Ʒ���ƣ�");
    scanf_s("%s", g.name, 20);
    printf("������Ʒ���");
    scanf_s("%s", g.category, 20);
    printf("���������λ��");
    scanf_s("%s", g.unit, 20);
    printf("���뵥�ۣ�");
    scanf_s("%f", &g.price);
    printf("������Ϳ�棺");
    scanf_s("%d", &g.min_stock);
    printf("������߿�棺");
    scanf_s("%d", &g.max_stock);
    fprintf(file, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    fclose(file);
    printf("��Ʒ��ӳɹ���\n");
}

// �鿴��Ʒ
void view_goods() {
    FILE* file = fopen("goods.txt", "r");
    if (file == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }
    Goods g;
    printf("\n��Ʒ��Ϣ��\n");
    printf("����\t����\t���\t��λ\t����\t��Ϳ��\t��߿��\n");
    while (fscanf_s(file, "%s %s %s %s %f %d %d", g.code, 20, g.name, 20, g.category, 20, g.unit, 20, &g.price, &g.min_stock, &g.max_stock) != EOF) {
        printf("%s\t%s\t%s\t%s\t%.2f\t%d\t%d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
}

// ɾ����Ʒ
void delete_goods() {
    FILE* file = fopen(GOODS_FILE, "r");
    FILE* temp = fopen("temp_goods.txt", "w");
    if (!file || !temp) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }
    Goods g;
    char delete_code[20];
    printf("������Ҫɾ������Ʒ���룺");
    scanf_s("%s", delete_code, 20);
    int found = 0;

    while (fscanf_s(file, "%s %s %s %s %f %d %d", g.code, 20, g.name, 20, g.category, 20, g.unit, 20, &g.price, &g.min_stock, &g.max_stock) != EOF) {
        // printf("%s\n", g.code);
        // printf("%s\n", delete_code);
        if (strcmp(g.code, delete_code) == 0) {
            found = 1;
            continue; // ����Ҫɾ���ļ�¼
        }
        fprintf(temp, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
    fclose(temp);

    if (found) {
        remove(GOODS_FILE); // ɾ��ԭ�ļ�
        rename("temp_goods.txt", GOODS_FILE); // ��������ʱ�ļ�
        printf("��Ʒɾ���ɹ���\n");
    }
    else {
        remove("temp_goods.txt");
        printf("δ�ҵ���Ӧ����Ʒ���롣\n");
    }
}

// �޸���Ʒ
void modify_goods() {
    FILE* file = fopen(GOODS_FILE, "r");
    FILE* temp = fopen("temp_goods.txt", "w");
    if (!file || !temp) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }
    Goods g;
    char modify_code[20];
    printf("������Ҫ�޸ĵ���Ʒ���룺");
    scanf_s("%s", modify_code, 20);

    int found = 0;

    // ��ȡ�ļ��е���Ʒ��Ϣ������Ҫ�޸ĵ���Ʒ
    while (fscanf_s(file, "%s %s %s %s %f %d %d", g.code, 20, g.name, 20, g.category, 20, g.unit, 20, &g.price, &g.min_stock, &g.max_stock) != EOF) {
        // printf("%sb\n", g.code);
        // printf("%sb\n", modify_code);
        // printf("%d\n", strcmp(g.code, modify_code));        
        // printf("%d\n", strcmp(g.code, "001"));
        // printf("%d\n", strcmp(modify_code, "001"));
        // for(int i=0;i<20;i++){
        //     printf("%c\n", g.code[i]);
        //     printf("%c\n", modify_code[i]);
        // }
        if (strcmp(g.code, modify_code) == 0) {  // ʹ�� strcmp �Ƚ���Ʒ����
            found = 1;
            printf("�����µ���Ʒ��Ϣ��\n");
            printf("��Ʒ���ƣ�");
            scanf_s("%s", g.name, 20);
            printf("��Ʒ���");
            scanf_s("%s", g.category, 20);
            printf("������λ��");
            scanf_s("%s", g.unit, 20);
            printf("���ۣ�");
            scanf_s("%f", &g.price);
            printf("��Ϳ�棺");
            scanf_s("%d", &g.min_stock);
            printf("��߿�棺");
            scanf_s("%d", &g.max_stock);
        }
        // ���޸ĺ�Ļ�δ�޸ĵ���Ʒ��Ϣд����ʱ�ļ�
        fprintf(temp, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
    fclose(temp);

    if (found) {
        remove(GOODS_FILE);  // ɾ��ԭ�ļ�
        rename("temp_goods.txt", GOODS_FILE);  // ��������ʱ�ļ�
        printf("��Ʒ��Ϣ�޸ĳɹ���\n");
    }
    else {
        remove("temp_goods.txt");  // ɾ����ʱ�ļ�
        printf("δ�ҵ���Ӧ����Ʒ���롣\n");
    }
}

// ��Ʒ������ϵ����
void classify_goods() {
    int choice;
    do {
        printf("\n--- ��Ʒ������� ---\n");
        printf("1. ��ӷ���\n");
        printf("2. �鿴����\n");
        printf("3. ɾ������\n");
        printf("4. �������˵�\n");
        printf("��ѡ��");
        scanf_s("%d", &choice, 20);

        switch (choice) {
        case 1: add_category(); break;
        case 2: view_categories(); break;
        case 3: delete_category(); break;
        case 4: return;
        default: printf("��Чѡ�������ѡ��\n");
        }
    } while (choice != 4);
}

// ��ӷ���
void add_category() {
    FILE* file = fopen(CATEGORY_FILE, "a");
    if (!file) {
        printf("�����ļ���ʧ�ܣ�\n");
        return;
    }
    char category[50];
    printf("����������ƣ�");
    scanf_s("%s", category, 20);
    fprintf(file, "%s\n", category);
    fclose(file);
    printf("������ӳɹ���\n");
}

// �鿴����
void view_categories() {
    FILE* file = fopen(CATEGORY_FILE, "r");
    if (!file) {
        printf("�����ļ���ʧ�ܣ�\n");
        return;
    }
    char category[50];
    printf("\n��ǰ�����б�\n");
    while (fscanf_s(file, "%s", category , 20 ) != EOF) {
        printf("- %s\n", category);
    }
    fclose(file);
}

// ɾ������
void delete_category() {
    FILE* file = fopen(CATEGORY_FILE, "r");
    FILE* temp = fopen("temp_categories.txt", "w");
    if (!file || !temp) {
        printf("�����ļ���ʧ�ܣ�\n");
        return;
    }
    char category[50], delete_category[50];
    int found = 0;
    printf("����Ҫɾ���ķ������ƣ�");
    scanf_s("%s", delete_category, 20);

    while (fscanf_s(file, "%s", category, 20) != EOF) {
        if (strcmp(category, delete_category) == 0) {
            found = 1;
            continue;
        }
        fprintf(temp, "%s\n", category);
    }
    fclose(file);
    fclose(temp);

    if (found) {
        remove(CATEGORY_FILE);
        rename("temp_categories.txt", CATEGORY_FILE);
        printf("����ɾ���ɹ���\n");
    }
    else {
        remove("temp_categories.txt");
        printf("δ�ҵ���Ӧ�ķ��ࡣ\n");
    }
}

// ��ӿ��
void add_stock() {
    FILE* file = fopen(STOCK_FILE, "a+");
    if (file == NULL) {
        printf("����ļ���ʧ�ܣ�\n");
        return;
    }

    Stock s;
    printf("������Ʒ���룺");
    scanf_s("%s", s.goods_code, 20);
    printf("������������");
    scanf_s("%d", &s.quantity, 20);
    printf("�����λ��");
    scanf_s("%s", s.location, 20);
    printf("�������κţ�");
    scanf_s("%s", s.batch_no, 20);
    printf("������Ч�ڣ�");
    scanf_s("%s", s.expire_date, 20);

    // �������Ϣд���ļ�
    fprintf(file, "%s %d %s %s %s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);
    fclose(file);
    printf("�����ӳɹ���\n");
}

// �鿴���
void view_stock() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (file == NULL) {
        printf("����ļ���ʧ�ܣ�\n");
        return;
    }

    Stock s;
    printf("\n�����Ϣ��\n");
    printf("��Ʒ����\t����\t��λ\t���κ�\t��Ч��\n");

    // ��ȡ����ʾ�����Ϣ
    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20 ) != EOF) {
        printf("%s\t%d\t%s\t%s\t%s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);
    }

    fclose(file);
}

// ����¼
void record_entry() {
    FILE* file = fopen(RECORD_FILE, "a");
    if (!file) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }
    Record r;
    printf("�����¼��ţ�");
    scanf_s("%s", r.record_id, 20);
    printf("������Ʒ���룺");
    scanf_s("%s", r.goods_code, 20);
    printf("�������������");
    scanf_s("%d", &r.quantity);
    r.type = 1; // �������
    printf("�������Ա��");
    scanf_s("%s", r.operator,20);
    printf("�������ʱ�䣺");
    scanf_s("%s", r.date, 20);
    printf("���뱸ע��");
    scanf_s("%s", r.note, 20);
    fprintf(file, "%s %s %d %d %s %s %s\n", r.record_id, r.goods_code, r.quantity, r.type, r.operator, r.date, r.note);
    fclose(file);
    printf("����¼��ӳɹ���\n");
}

// �鿴������¼
void view_records() {
    FILE* file = fopen(RECORD_FILE, "r");  // �򿪼�¼�ļ�
    if (file == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        return;
    }

    Record r;  // ����һ�� Record �ṹ�壬���ڴ洢ÿһ����¼
    printf("\n������¼��\n");
    printf("���\t��Ʒ����\t����\t����\t����Ա\tʱ��\t��ע\n");

    // ���ļ��ж�ȡ��¼����ӡ
    while (fscanf_s(file, "%s %s %d %d %s %s %[^\n]", r.record_id, 20, r.goods_code, 20, &r.quantity,
        &r.type, r.operator, 20, r.date, 20, r.note ,20 ) != EOF) {
        // ���ݼ�¼�����ͣ�������⣩��ӡ��ͬ��ֵ
        printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\n", r.record_id, r.goods_code, r.quantity,
            r.type == 1 ? "���" : "����", r.operator, r.date, r.note);
    }

    fclose(file);  // �ر��ļ�
}

// �˳�������¼����
void record_exit() {
    printf("�˳�������¼�����������˵�...\n");
    // �������˵�����������
    return;
}

// // ��λ����
// void allocate_stock() {
//     FILE *file = fopen(LOCATION_FILE, "r+");
//     if (!file) {
//         printf("��λ�ļ���ʧ�ܣ�\n");
//         return;
//     }
//     char goods_code[20], location[20];
//     int occupied;

//     printf("������Ʒ���룺");
//     scanf_s("%s", goods_code);

//     // ��������δռ�ÿ�λ
//     int found = 0;
//     long pos;
//     while ((pos = ftell(file)) >= 0 && fscanf_s(file, "%s %d", location, &occupied) != EOF) {
//         if (!occupied) {
//             found = 1;
//             printf("Ϊ��Ʒ [%s] �����λ��%s\n", goods_code, location);

//             // ��ǿ�λΪ��ռ��
//             fseek(file, pos, SEEK_SET);
//             fprintf(file, "%s %d\n", location, 1);
//             break;
//         }
//     }

//     if (!found) {
//         printf("δ�ҵ����ÿ�λ��\n");
//     }

//     fclose(file);
// }

// // �ʼ��¼
// void quality_check() {
//     FILE *file = fopen(QC_FILE, "a");
//     if (!file) {
//         printf("�ʼ��¼�ļ���ʧ�ܣ�\n");
//         return;
//     }
//     char goods_code[20], qc_status[10], note[100];
//     printf("������Ʒ���룺");
//     scanf_s("%s", goods_code);
//     printf("�����ʼ�״̬���ϸ�/���ϸ񣩣�");
//     scanf_s("%s", qc_status);
//     printf("�����ʼ챸ע��");
//     scanf_s("%s", note);
//     fprintf(file, "%s %s %s\n", goods_code, qc_status, note);
//     fclose(file);
//     printf("�ʼ��¼��ӳɹ���\n");
// }

// ���Ԥ��
void inventory_warning() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (!file) {
        printf("����ļ���ʧ�ܣ�\n");
        return;
    }
    Stock s;
    printf("\n���Ԥ����Ϣ��\n");
    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20) != EOF) {
        if (s.quantity < 10) { // ʾ����ֵ
            printf("��Ʒ [%s] ��治�㣺%d\n", s.goods_code, s.quantity);
        }
        else if (s.quantity > 100) { // ʾ����ֵ
            printf("��Ʒ [%s] ��������%d\n", s.goods_code, s.quantity);
        }
    }
    fclose(file);
}

// ��Ʒ��ת��ͳ��
void turnover_rate_analysis() {
    FILE* file = fopen(RECORD_FILE, "r");
    if (!file) {
        printf("������¼�ļ���ʧ�ܣ�\n");
        return;
    }
    Record r;
    int entry_count = 0, exit_count = 0;
    printf("\n��Ʒ��ת��ͳ�ƣ�\n");

    while (fscanf_s(file, "%s %s %d %d %s %s %[^\n]", r.record_id, 20, r.goods_code, 20, &r.quantity, &r.type, r.operator, 20, r.date, 20, r.note, 20 ) != EOF) {
        if (r.type == 1) {
            entry_count += r.quantity;
        }
        else if (r.type == 2) {
            exit_count += r.quantity;
        }
    }
    fclose(file);

    if (entry_count > 0) {
        float turnover_rate = (float)exit_count / entry_count * 100;
        printf("����⣺%d���ܳ��⣺%d����ת�ʣ�%.2f%%\n", entry_count, exit_count, turnover_rate);
    }
    else {
        printf("û���㹻���ݼ�����ת�ʡ�\n");
    }
}

// ����̵�
void stock_analysis() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (!file) {
        printf("����ļ���ʧ�ܣ�\n");
        return;
    }
    Stock s;
    int total_stock = 0;

    printf("\n--- ����̵� ---\n");
    printf("��Ʒ����\t����\t��λ\t���κ�\t��Ч��\n");

    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20) != EOF) {
        total_stock += s.quantity;
        printf("%s\t%d\t%s\t%s\t%s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);

        // ���������
        if (s.quantity < 10) {
            printf("���棺��Ʒ [%s] ��治�㣨������%d����\n", s.goods_code, s.quantity);
        }
        else if (s.quantity > 100) {
            printf("��ʾ����Ʒ [%s] ��泬����������%d����\n", s.goods_code, s.quantity);
        }
    }

    printf("\n���������%d\n", total_stock);
    fclose(file);
}
