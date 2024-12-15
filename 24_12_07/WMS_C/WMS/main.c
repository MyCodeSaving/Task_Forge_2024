// 使用时请使用英文或汉语拼音，否则可能无法正常显示。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <wchar.h>
// #include <locale.h>
// #include <windows.h>

// 定义商品信息结构
typedef struct goods {
    char code[20];        // 商品编码
    char name[50];        // 商品名称
    char category[30];    // 商品类别
    char unit[10];        // 计量单位
    float price;             // 单价
    int min_stock;           // 最低库存
    int max_stock;           // 最高库存
} Goods;

// 定义库存信息结构
typedef struct stock {
    char goods_code[20];  // 商品编码
    int quantity;            // 库存数量
    char location[30];    // 库位
    char batch_no[20];    // 批次号
    char expire_date[20]; // 有效期
} Stock;

// 定义出入库记录结构
typedef struct record {
    char record_id[20];   // 记录编号
    char goods_code[20];  // 商品编码
    int quantity;            // 数量
    int type;                // 类型（1：入库，2：出库）
    char operator[50];    // 操作员
    char date[20];        // 操作时间
    char note[100];       // 备注
} Record;

// 文件路径定义
#define GOODS_FILE "goods.txt"
#define STOCK_FILE "stock.txt"
#define RECORD_FILE "record.txt"
#define CATEGORY_FILE "categories.txt"
#define QC_FILE "quality_check.txt"
#define LOCATION_FILE "locations.txt"

// 函数声明
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

void stock_analysis(); // 库存盘点
// void allocate_stock(); // 库位分配
// void quality_check(); // 质检记录
void inventory_warning(); // 库存预警
void turnover_rate_analysis(); // 商品周转率统计

// 主函数
int main() {
    // SetConsoleOutputCP(65001); // 支持中文输出
    // setlocale(LC_ALL, ""); // 支持中文显示
    int choice;
    do {
        menu();
        printf("请输入选项：");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: goods_menu(); break;
        case 2: stock_menu(); break;
        case 3: record_menu(); break;
        case 4: statistics_menu(); break;
        case 5: printf("退出系统\n"); break;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 5);
    return 0;
}

// 主菜单
void menu() {
    printf("\n=========== 仓库管理系统 ===========\n");
    printf("1. 商品管理\n");
    printf("2. 库存管理\n");
    printf("3. 出入库记录管理\n");
    printf("4. 统计分析\n");
    printf("5. 退出系统\n");
    printf("===================================\n");
}

// 商品管理菜单
void goods_menu() {
    int choice;
    do {
        printf("\n--- 商品管理 ---\n");
        printf("1. 添加商品\n");
        printf("2. 查看商品\n");
        printf("3. 删除商品\n");
        printf("4. 修改商品信息\n");
        printf("5. 分类管理\n");
        printf("6. 返回主菜单\n");
        printf("请选择：");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: add_goods(); break;
        case 2: view_goods(); break;
        case 3: delete_goods(); break;
        case 4: modify_goods(); break;
        case 5: classify_goods(); break;
        case 6: return;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 6);
}

// 库存管理菜单
void stock_menu() {
    int choice;
    do {
        printf("\n--- 库存管理 ---\n");
        printf("1. 入库登记\n");
        printf("2. 查看库存\n");
        printf("3. 返回主菜单\n");
        printf("请选择：");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: add_stock(); break;
        case 2: view_stock(); break;
        case 3: return;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 3);
}

// 出入库记录管理菜单
void record_menu() {
    int choice;
    do {
        printf("\n--- 出入库记录管理 ---\n");
        printf("1. 添加记录\n");
        printf("2. 查看记录\n");
        printf("3. 返回主菜单\n");
        printf("请选择：");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: record_entry(); break;
        case 2: view_records(); break;
        case 3: record_exit(); break;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 3);
}

// 统计分析菜单
void statistics_menu() {
    int choice;
    do {
        printf("\n--- 统计分析 ---\n");
        printf("1. 库存盘点\n");
        printf("2. 库存预警\n");
        printf("3. 商品周转率统计\n");
        printf("4. 返回主菜单\n");
        printf("请选择：");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: stock_analysis(); break;
        case 2: inventory_warning(); break;
        case 3: turnover_rate_analysis(); break;
        case 4: return;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 4);
}

// 添加商品
void add_goods() {
    FILE* file = fopen("goods.txt", "a");
    if (file == NULL) {
        printf("文件打开失败！\n");
        return;
    }
    Goods g;
    printf("输入商品编码：");
    scanf_s("%s", g.code ,20 );
    printf("输入商品名称：");
    scanf_s("%s", g.name, 20);
    printf("输入商品类别：");
    scanf_s("%s", g.category, 20);
    printf("输入计量单位：");
    scanf_s("%s", g.unit, 20);
    printf("输入单价：");
    scanf_s("%f", &g.price);
    printf("输入最低库存：");
    scanf_s("%d", &g.min_stock);
    printf("输入最高库存：");
    scanf_s("%d", &g.max_stock);
    fprintf(file, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    fclose(file);
    printf("商品添加成功！\n");
}

// 查看商品
void view_goods() {
    FILE* file = fopen("goods.txt", "r");
    if (file == NULL) {
        printf("文件打开失败！\n");
        return;
    }
    Goods g;
    printf("\n商品信息：\n");
    printf("编码\t名称\t类别\t单位\t单价\t最低库存\t最高库存\n");
    while (fscanf_s(file, "%s %s %s %s %f %d %d", g.code, 20, g.name, 20, g.category, 20, g.unit, 20, &g.price, &g.min_stock, &g.max_stock) != EOF) {
        printf("%s\t%s\t%s\t%s\t%.2f\t%d\t%d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
}

// 删除商品
void delete_goods() {
    FILE* file = fopen(GOODS_FILE, "r");
    FILE* temp = fopen("temp_goods.txt", "w");
    if (!file || !temp) {
        printf("文件打开失败！\n");
        return;
    }
    Goods g;
    char delete_code[20];
    printf("请输入要删除的商品编码：");
    scanf_s("%s", delete_code, 20);
    int found = 0;

    while (fscanf_s(file, "%s %s %s %s %f %d %d", g.code, 20, g.name, 20, g.category, 20, g.unit, 20, &g.price, &g.min_stock, &g.max_stock) != EOF) {
        // printf("%s\n", g.code);
        // printf("%s\n", delete_code);
        if (strcmp(g.code, delete_code) == 0) {
            found = 1;
            continue; // 跳过要删除的记录
        }
        fprintf(temp, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
    fclose(temp);

    if (found) {
        remove(GOODS_FILE); // 删除原文件
        rename("temp_goods.txt", GOODS_FILE); // 重命名临时文件
        printf("商品删除成功！\n");
    }
    else {
        remove("temp_goods.txt");
        printf("未找到对应的商品编码。\n");
    }
}

// 修改商品
void modify_goods() {
    FILE* file = fopen(GOODS_FILE, "r");
    FILE* temp = fopen("temp_goods.txt", "w");
    if (!file || !temp) {
        printf("文件打开失败！\n");
        return;
    }
    Goods g;
    char modify_code[20];
    printf("请输入要修改的商品编码：");
    scanf_s("%s", modify_code, 20);

    int found = 0;

    // 读取文件中的商品信息并查找要修改的商品
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
        if (strcmp(g.code, modify_code) == 0) {  // 使用 strcmp 比较商品编码
            found = 1;
            printf("输入新的商品信息：\n");
            printf("商品名称：");
            scanf_s("%s", g.name, 20);
            printf("商品类别：");
            scanf_s("%s", g.category, 20);
            printf("计量单位：");
            scanf_s("%s", g.unit, 20);
            printf("单价：");
            scanf_s("%f", &g.price);
            printf("最低库存：");
            scanf_s("%d", &g.min_stock);
            printf("最高库存：");
            scanf_s("%d", &g.max_stock);
        }
        // 将修改后的或未修改的商品信息写入临时文件
        fprintf(temp, "%s %s %s %s %.2f %d %d\n", g.code, g.name, g.category, g.unit, g.price, g.min_stock, g.max_stock);
    }
    fclose(file);
    fclose(temp);

    if (found) {
        remove(GOODS_FILE);  // 删除原文件
        rename("temp_goods.txt", GOODS_FILE);  // 重命名临时文件
        printf("商品信息修改成功！\n");
    }
    else {
        remove("temp_goods.txt");  // 删除临时文件
        printf("未找到对应的商品编码。\n");
    }
}

// 商品分类体系管理
void classify_goods() {
    int choice;
    do {
        printf("\n--- 商品分类管理 ---\n");
        printf("1. 添加分类\n");
        printf("2. 查看分类\n");
        printf("3. 删除分类\n");
        printf("4. 返回主菜单\n");
        printf("请选择：");
        scanf_s("%d", &choice, 20);

        switch (choice) {
        case 1: add_category(); break;
        case 2: view_categories(); break;
        case 3: delete_category(); break;
        case 4: return;
        default: printf("无效选项，请重新选择。\n");
        }
    } while (choice != 4);
}

// 添加分类
void add_category() {
    FILE* file = fopen(CATEGORY_FILE, "a");
    if (!file) {
        printf("分类文件打开失败！\n");
        return;
    }
    char category[50];
    printf("输入分类名称：");
    scanf_s("%s", category, 20);
    fprintf(file, "%s\n", category);
    fclose(file);
    printf("分类添加成功！\n");
}

// 查看分类
void view_categories() {
    FILE* file = fopen(CATEGORY_FILE, "r");
    if (!file) {
        printf("分类文件打开失败！\n");
        return;
    }
    char category[50];
    printf("\n当前分类列表：\n");
    while (fscanf_s(file, "%s", category , 20 ) != EOF) {
        printf("- %s\n", category);
    }
    fclose(file);
}

// 删除分类
void delete_category() {
    FILE* file = fopen(CATEGORY_FILE, "r");
    FILE* temp = fopen("temp_categories.txt", "w");
    if (!file || !temp) {
        printf("分类文件打开失败！\n");
        return;
    }
    char category[50], delete_category[50];
    int found = 0;
    printf("输入要删除的分类名称：");
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
        printf("分类删除成功！\n");
    }
    else {
        remove("temp_categories.txt");
        printf("未找到对应的分类。\n");
    }
}

// 添加库存
void add_stock() {
    FILE* file = fopen(STOCK_FILE, "a+");
    if (file == NULL) {
        printf("库存文件打开失败！\n");
        return;
    }

    Stock s;
    printf("输入商品编码：");
    scanf_s("%s", s.goods_code, 20);
    printf("输入库存数量：");
    scanf_s("%d", &s.quantity, 20);
    printf("输入库位：");
    scanf_s("%s", s.location, 20);
    printf("输入批次号：");
    scanf_s("%s", s.batch_no, 20);
    printf("输入有效期：");
    scanf_s("%s", s.expire_date, 20);

    // 将库存信息写入文件
    fprintf(file, "%s %d %s %s %s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);
    fclose(file);
    printf("库存添加成功！\n");
}

// 查看库存
void view_stock() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (file == NULL) {
        printf("库存文件打开失败！\n");
        return;
    }

    Stock s;
    printf("\n库存信息：\n");
    printf("商品编码\t数量\t库位\t批次号\t有效期\n");

    // 读取并显示库存信息
    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20 ) != EOF) {
        printf("%s\t%d\t%s\t%s\t%s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);
    }

    fclose(file);
}

// 入库记录
void record_entry() {
    FILE* file = fopen(RECORD_FILE, "a");
    if (!file) {
        printf("文件打开失败！\n");
        return;
    }
    Record r;
    printf("输入记录编号：");
    scanf_s("%s", r.record_id, 20);
    printf("输入商品编码：");
    scanf_s("%s", r.goods_code, 20);
    printf("输入入库数量：");
    scanf_s("%d", &r.quantity);
    r.type = 1; // 入库类型
    printf("输入操作员：");
    scanf_s("%s", r.operator,20);
    printf("输入操作时间：");
    scanf_s("%s", r.date, 20);
    printf("输入备注：");
    scanf_s("%s", r.note, 20);
    fprintf(file, "%s %s %d %d %s %s %s\n", r.record_id, r.goods_code, r.quantity, r.type, r.operator, r.date, r.note);
    fclose(file);
    printf("入库记录添加成功！\n");
}

// 查看出入库记录
void view_records() {
    FILE* file = fopen(RECORD_FILE, "r");  // 打开记录文件
    if (file == NULL) {
        printf("文件打开失败！\n");
        return;
    }

    Record r;  // 声明一个 Record 结构体，用于存储每一条记录
    printf("\n出入库记录：\n");
    printf("编号\t商品编码\t数量\t类型\t操作员\t时间\t备注\n");

    // 从文件中读取记录并打印
    while (fscanf_s(file, "%s %s %d %d %s %s %[^\n]", r.record_id, 20, r.goods_code, 20, &r.quantity,
        &r.type, r.operator, 20, r.date, 20, r.note ,20 ) != EOF) {
        // 根据记录的类型（入库或出库）打印不同的值
        printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\n", r.record_id, r.goods_code, r.quantity,
            r.type == 1 ? "入库" : "出库", r.operator, r.date, r.note);
    }

    fclose(file);  // 关闭文件
}

// 退出出入库记录管理
void record_exit() {
    printf("退出出入库记录管理，返回主菜单...\n");
    // 返回主菜单或其他操作
    return;
}

// // 库位分配
// void allocate_stock() {
//     FILE *file = fopen(LOCATION_FILE, "r+");
//     if (!file) {
//         printf("库位文件打开失败！\n");
//         return;
//     }
//     char goods_code[20], location[20];
//     int occupied;

//     printf("输入商品编码：");
//     scanf_s("%s", goods_code);

//     // 遍历查找未占用库位
//     int found = 0;
//     long pos;
//     while ((pos = ftell(file)) >= 0 && fscanf_s(file, "%s %d", location, &occupied) != EOF) {
//         if (!occupied) {
//             found = 1;
//             printf("为商品 [%s] 分配库位：%s\n", goods_code, location);

//             // 标记库位为已占用
//             fseek(file, pos, SEEK_SET);
//             fprintf(file, "%s %d\n", location, 1);
//             break;
//         }
//     }

//     if (!found) {
//         printf("未找到可用库位！\n");
//     }

//     fclose(file);
// }

// // 质检记录
// void quality_check() {
//     FILE *file = fopen(QC_FILE, "a");
//     if (!file) {
//         printf("质检记录文件打开失败！\n");
//         return;
//     }
//     char goods_code[20], qc_status[10], note[100];
//     printf("输入商品编码：");
//     scanf_s("%s", goods_code);
//     printf("输入质检状态（合格/不合格）：");
//     scanf_s("%s", qc_status);
//     printf("输入质检备注：");
//     scanf_s("%s", note);
//     fprintf(file, "%s %s %s\n", goods_code, qc_status, note);
//     fclose(file);
//     printf("质检记录添加成功！\n");
// }

// 库存预警
void inventory_warning() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (!file) {
        printf("库存文件打开失败！\n");
        return;
    }
    Stock s;
    printf("\n库存预警信息：\n");
    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20) != EOF) {
        if (s.quantity < 10) { // 示例阈值
            printf("商品 [%s] 库存不足：%d\n", s.goods_code, s.quantity);
        }
        else if (s.quantity > 100) { // 示例阈值
            printf("商品 [%s] 库存过量：%d\n", s.goods_code, s.quantity);
        }
    }
    fclose(file);
}

// 商品周转率统计
void turnover_rate_analysis() {
    FILE* file = fopen(RECORD_FILE, "r");
    if (!file) {
        printf("出入库记录文件打开失败！\n");
        return;
    }
    Record r;
    int entry_count = 0, exit_count = 0;
    printf("\n商品周转率统计：\n");

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
        printf("总入库：%d，总出库：%d，周转率：%.2f%%\n", entry_count, exit_count, turnover_rate);
    }
    else {
        printf("没有足够数据计算周转率。\n");
    }
}

// 库存盘点
void stock_analysis() {
    FILE* file = fopen(STOCK_FILE, "r");
    if (!file) {
        printf("库存文件打开失败！\n");
        return;
    }
    Stock s;
    int total_stock = 0;

    printf("\n--- 库存盘点 ---\n");
    printf("商品编码\t数量\t库位\t批次号\t有效期\n");

    while (fscanf_s(file, "%s %d %s %s %s", s.goods_code, 20, &s.quantity, s.location, 20, s.batch_no, 20, s.expire_date ,20) != EOF) {
        total_stock += s.quantity;
        printf("%s\t%d\t%s\t%s\t%s\n", s.goods_code, s.quantity, s.location, s.batch_no, s.expire_date);

        // 检测问题库存
        if (s.quantity < 10) {
            printf("警告：商品 [%s] 库存不足（数量：%d）！\n", s.goods_code, s.quantity);
        }
        else if (s.quantity > 100) {
            printf("提示：商品 [%s] 库存超量（数量：%d）！\n", s.goods_code, s.quantity);
        }
    }

    printf("\n库存总量：%d\n", total_stock);
    fclose(file);
}
