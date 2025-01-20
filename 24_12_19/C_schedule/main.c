#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // Windows 平台的头文件
#include <locale.h>

#define MAX_EVENTS 100
#define MAX_DESC 256

typedef struct {
    char date[11];   // 格式: YYYY-MM-DD
    char time[6];    // 格式: HH:MM
    char desc[MAX_DESC];
    int reminder_time; // 提醒时间（-1表示无需提醒）
} Event;

Event events[MAX_EVENTS];
int event_count = 0;
int reminder_running = 0; // 提醒服务状态

// 添加日程
void add_event() {
    if (event_count >= MAX_EVENTS) {
        printf("日程已满，无法添加新日程！\n");
        return;
    }
    printf("请输入日期(格式 YYYY-MM-DD): ");
    scanf("%s", events[event_count].date);
    printf("请输入时间(格式 HH:MM): ");
    scanf("%s", events[event_count].time);
    printf("请输入日程描述: ");
    getchar();  // 清除缓冲区换行符
    fgets(events[event_count].desc, MAX_DESC, stdin);
    events[event_count].desc[strcspn(events[event_count].desc, "\n")] = 0;  // 去掉换行符
    printf("请输入提醒时间(int)(-1 表示无需提醒): ");
    scanf("%d", &events[event_count].reminder_time);
    while ( events[event_count].reminder_time > ((events[event_count].time[3]-'0')*10 + (events[event_count].time[4]-'0')) ) {
        printf("提醒时间必须位于日程当日！\n");
        printf("请输入新提醒时间(int)(-1 表示无需提醒): ");
        scanf("%d", &events[event_count].reminder_time);
    };
    event_count++;
    printf("日程添加成功！\n");
}

// 删除日程
void delete_event() {
    if (event_count == 0) {
        printf("当前无任何日程！\n");
        return;
    }
    int index;
    printf("请输入要删除的日程编号(1-%d): ", event_count);
    scanf("%d", &index);
    if (index < 1 || index > event_count) {
        printf("编号无效！\n");
        return;
    }
    for (int i = index - 1; i < event_count - 1; i++) {
        events[i] = events[i + 1];
    }
    event_count--;
    printf("日程删除成功！\n");
}

// 修改日程
void modify_event() {
    if (event_count == 0) {
        printf("当前无任何日程！\n");
        return;
    }
    int index;
    printf("请输入要修改的日程编号(1-%d): ", event_count);
    scanf("%d", &index);
    if (index < 1 || index > event_count) {
        printf("编号无效！\n");
        return;
    }
    index--; // 转为数组索引
    printf("当前日程: 日期: %s 时间: %s 描述: %s 提醒时间：%d分钟前\n", 
            events[index].date, events[index].time, events[index].desc, events[index].reminder_time);
    printf("请输入新日期(格式 YYYY-MM-DD): ");
    scanf("%s", events[index].date);
    printf("请输入新时间(格式 HH:MM): ");
    scanf("%s", events[index].time);
    printf("请输入新日程描述: ");
    getchar();  // 清除缓冲区换行符
    fgets(events[index].desc, MAX_DESC, stdin);
    events[index].desc[strcspn(events[index].desc, "\n")] = 0;  // 去掉换行符
    printf("请输入新提醒时间(int)(-1 表示无需提醒): ");
    scanf("%d", &events[index].reminder_time);
    while ( events[index].reminder_time > ((events[index].time[3]-'0')*10 + (events[index].time[4]-'0')) ) {
        printf("提醒时间必须位于日程当日！\n");
        printf("请输入新提醒时间(int)(-1 表示无需提醒): ");
        scanf("%d", &events[index].reminder_time);
    };
    printf("日程修改成功！\n");
}

// 查看日程
void view_events() {
    if (event_count == 0) {
        printf("当前无任何日程！\n");
        return;
    }
    printf("当前日程列表:\n");
    for (int i = 0; i < event_count; i++) {
        printf("%d. 日期: %s 时间: %s 描述: %s 提醒时间: %d 分钟前\n", 
                i + 1, events[i].date, events[i].time, events[i].desc, events[i].reminder_time);
    }
}

// 保存到文件
void save_to_file() {
    FILE *file = fopen("schedule.txt", "w");
    if (!file) {
        printf("无法保存到文件！\n");
        return;
    }
    for (int i = 0; i < event_count; i++) {
        fprintf(file, "%s %s %s %d\n", events[i].date, events[i].time, events[i].desc, events[i].reminder_time);
    }
    fclose(file);
    printf("日程保存成功！\n");
}

// 从文件加载
void load_from_file() {
    FILE *file = fopen("schedule.txt", "r");
    if (!file) {
        printf("无法读取文件，可能文件不存在。\n");
        return;
    }
    event_count = 0;
    while (fscanf(file, "%s %s %s %d\n", 
                  events[event_count].date, 
                  events[event_count].time, 
                  events[event_count].desc,
                  &events[event_count].reminder_time) != EOF) {
        // 打印调试信息，检查字段读取的情况
        printf("读取：日期=%s, 时间=%s, 描述=%s, 提醒时间=%d\n", 
           events[event_count].date, 
           events[event_count].time, 
           events[event_count].desc, 
           events[event_count].reminder_time);
        event_count++;
    }
    fclose(file);
    printf("日程加载成功！\n");
}

// 提醒功能
void check_reminders() {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char current_date[11];
    char current_time_str[6];

    // 获取当前日期和时间字符串
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", current_time);
    strftime(current_time_str, sizeof(current_time_str), "%H:%M", current_time);

    // 遍历日程列表，检查是否有匹配的日程
    for (int i = 0; i < event_count; i++) {
        if (events[i].reminder_time == -1) {
            continue; // 无需提醒
        }
        Event event_tmp = events[i];
        int remind_time_min = (event_tmp.time[3]-'0')*10 + (event_tmp.time[4]-'0');
        int remind_time_hour = (event_tmp.time[0]-'0')*10 + (event_tmp.time[1]-'0');
        if (remind_time_min < event_tmp.reminder_time) {
            remind_time_hour -= (event_tmp.reminder_time / 60);
            remind_time_min += (event_tmp.reminder_time / 60) *60 - event_tmp.reminder_time;
            if (remind_time_min < 0) {
                remind_time_hour--;
                remind_time_min += 60;
            }
        } else {
            remind_time_min -= event_tmp.reminder_time;
        }
        // 更新提醒时间
        event_tmp.time[0] = remind_time_hour / 10 + '0';
        event_tmp.time[1] = remind_time_hour % 10 + '0';
        event_tmp.time[3] = remind_time_min / 10 + '0';
        event_tmp.time[4] = remind_time_min % 10 + '0';
        // 打印调试信息，检查提醒时间计算的情况
        // printf("检查提醒时间：日期=%s, 时间=%s, 描述=%s, 提醒时间=%d\n", 
        //    event_tmp.date, 
        //    event_tmp.time, 
        //    event_tmp.desc, 
        //    event_tmp.reminder_time);
        if (strcmp(event_tmp.date, current_date) == 0 &&
            strcmp(event_tmp.time, current_time_str) == 0) {
            printf("\n提醒！\n距离日程: %s 还有%d分钟!\n\n",
                   event_tmp.desc, event_tmp.reminder_time);
            event_tmp.reminder_time = -1; // 防止重复提醒
        }
    }
}

// 线程函数
DWORD WINAPI reminder_thread(LPVOID lpParam) {
    while (reminder_running) {
        check_reminders();
        Sleep(30000); // 检查周期 ms
    }
    return 0;
}

// 启动提醒服务
void start_reminder_service() {
    if (reminder_running) {
        printf("提醒服务已经运行！\n");
        return;
    }
    reminder_running = 1;
    HANDLE thread_handle = CreateThread(
        NULL,        // 默认安全属性
        0,           // 默认栈大小
        reminder_thread, // 线程函数
        NULL,        // 线程参数
        0,           // 默认创建标志
        NULL         // 不需要线程 ID
    );

    if (thread_handle) {
        printf("提醒服务已启动！\n");
    } else {
        printf("无法启动提醒服务线程！\n");
        reminder_running = 0;
    }
}

// 停止提醒服务
void stop_reminder_service() {
    if (!reminder_running) {
        printf("提醒服务未运行！\n");
        return;
    }
    reminder_running = 0;
    printf("提醒服务已停止！\n");
}

// 显示菜单
void display_menu() {
    printf("\n日程管理系统\n");
    printf("1. 添加日程\n");
    printf("2. 删除日程\n");
    printf("3. 修改日程\n");
    printf("4. 查看日程\n");
    printf("5. 保存日程\n");
    printf("6. 加载日程\n");
    printf("7. 启动日程提醒\n");
    printf("8. 停止日程提醒\n");
    printf("0. 退出 \n");
    printf("请选择操作: ");
}

int main() {
    SetConsoleOutputCP(65001); // 支持中文输出
    setlocale(LC_ALL, ""); // 支持中文显示
    int choice;
    load_from_file(); // 启动时尝试加载日程
    do {
        // Sleep(100); // 延时 ms
        // system("cls");
        display_menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: add_event(); break;
            case 2: delete_event(); break;
            case 3: modify_event(); break;
            case 4: view_events(); break;
            case 5: save_to_file(); break;
            case 6: load_from_file(); break;
            case 7: start_reminder_service(); break;
            case 8: stop_reminder_service(); break;
            case 0: stop_reminder_service(); printf("退出系统。\n"); break;
            default: printf("无效选择，请重试！\n");
        }
    } while (choice != 0);
    return 0;
}
