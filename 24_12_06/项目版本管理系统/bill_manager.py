import datetime
import json
import os
import sys


def resource_path(relative_path):
    """获取资源文件的路径，兼容打包后的情况"""
    try:
        # PyInstaller会将资源打包到 _MEIPASS 文件夹
        base_path = sys._MEIPASS
    except AttributeError:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

# 数据存储文件路径
DATA_FILE = resource_path("resources/bills_data.json")
print(DATA_FILE)

# 初始化数据
data = {
    "incomes": [],
    "expenses": [],
    "monthly_budget": None
}


def load_data():
    """加载本地文件数据"""
    global data
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, "r", encoding="utf-8") as file:
            data = json.load(file)


def save_data():
    """保存数据到本地文件"""
    with open(DATA_FILE, "w", encoding="utf-8") as file:
        json.dump(data, file, ensure_ascii=False, indent=4)


def is_valid_date(date_str):
    """验证日期格式是否正确"""
    try:
        datetime.datetime.strptime(date_str, "%Y-%m-%d")
        return True
    except ValueError:
        return False


def add_income():
    """记录收入"""
    print("\n请输入收入信息：")
    date = input("日期（YYYY-MM-DD）：")
    if not is_valid_date(date):
        print("日期格式错误，请重新输入！")
        return

    try:
        amount = float(input("金额："))
        if amount <= 0:
            print("收入金额必须为正数！")
            return
    except ValueError:
        print("金额输入错误，请输入数字！")
        return

    category = input("类别（如工资、奖金等）：")
    remark = input("备注：")
    data["incomes"].append({"date": date, "amount": amount, "category": category, "remark": remark})
    save_data()
    print("收入已成功记录！")


def add_expense():
    """记录支出"""
    print("\n请输入支出信息：")
    date = input("日期（YYYY-MM-DD）：")
    if not is_valid_date(date):
        print("日期格式错误，请重新输入！")
        return

    try:
        amount = float(input("金额："))
        if amount <= 0:
            print("支出金额必须为正数！")
            return
    except ValueError:
        print("金额输入错误，请输入数字！")
        return

    category = input("类别（如餐饮、交通、购物等）：")
    remark = input("备注：")
    data["expenses"].append({"date": date, "amount": amount, "category": category, "remark": remark})
    save_data()
    print("支出已成功记录！")


def show_bills():
    """展示所有账单"""
    print("\n所有收入记录：")
    for income in data["incomes"]:
        print(f"日期：{income['date']}，金额：{income['amount']}，类别：{income['category']}，备注：{income['remark']}")

    print("\n所有支出记录：")
    for expense in data["expenses"]:
        print(f"日期：{expense['date']}，金额：{expense['amount']}，类别：{expense['category']}，备注：{expense['remark']}")


def query_bills():
    """查询账单"""
    print("\n查询条件：")
    print("1. 按日期查询")
    print("2. 按日期范围查询")
    print("3. 按类别查询")
    choice = input("请输入查询条件序号：")

    if choice == "1":
        date = input("请输入日期（YYYY-MM-DD）：")
        if not is_valid_date(date):
            print("日期格式错误！")
            return
        print("\n查询结果：")
        print("收入记录：")
        for income in data["incomes"]:
            if income["date"] == date:
                print(income)
        print("支出记录：")
        for expense in data["expenses"]:
            if expense["date"] == date:
                print(expense)

    elif choice == "2":
        start_date = input("请输入起始日期（YYYY-MM-DD）：")
        end_date = input("请输入结束日期（YYYY-MM-DD）：")
        if not (is_valid_date(start_date) and is_valid_date(end_date)):
            print("日期格式错误！")
            return
        print("\n查询结果：")
        print("收入记录：")
        for income in data["incomes"]:
            if start_date <= income["date"] <= end_date:
                print(income)
        print("支出记录：")
        for expense in data["expenses"]:
            if start_date <= expense["date"] <= end_date:
                print(expense)

    elif choice == "3":
        category = input("请输入类别：")
        print("\n查询结果：")
        print("收入记录：")
        for income in data["incomes"]:
            if income["category"] == category:
                print(income)
        print("支出记录：")
        for expense in data["expenses"]:
            if expense["category"] == category:
                print(expense)
    else:
        print("无效的查询条件！")


def set_budget():
    """设置月度预算"""
    try:
        budget = float(input("请输入每月预算金额："))
        if budget <= 0:
            print("预算金额必须为正数！")
            return
        data["monthly_budget"] = budget
        save_data()
        print("预算已设置成功！")
    except ValueError:
        print("金额输入错误，请输入数字！")


def monthly_report():
    """生成月度统计报告"""
    if data["monthly_budget"] is None:
        print("请先设置月度预算！")
        return
    total_income = sum(income["amount"] for income in data["incomes"])
    total_expense = sum(expense["amount"] for expense in data["expenses"])
    remaining_budget = data["monthly_budget"] - total_expense
    print("\n月度统计报告：")
    print(f"总收入：{total_income}")
    print(f"总支出：{total_expense}")
    print(f"剩余预算：{remaining_budget}")
    print("各类别支出：")
    categories = {}
    for expense in data["expenses"]:
        categories[expense["category"]] = categories.get(expense["category"], 0) + expense["amount"]
    for category, amount in categories.items():
        print(f"{category}：{amount}")


def main():
    load_data()
    while True:
        print("\n=================================")
        print("欢迎使用个人账单管理系统")
        print("=================================")
        print("1. 记录收入")
        print("2. 记录支出")
        print("3. 查看所有账单")
        print("4. 查询账单")
        print("5. 设置月度预算")
        print("6. 查看月度统计报告")
        print("7. 退出系统")
        choice = input("请输入选项序号：")

        if choice == "1":
            add_income()
        elif choice == "2":
            add_expense()
        elif choice == "3":
            show_bills()
        elif choice == "4":
            query_bills()
        elif choice == "5":
            set_budget()
        elif choice == "6":
            monthly_report()
        elif choice == "7":
            print("感谢使用，再见！")
            break
        else:
            print("无效的选项，请重新输入！")


if __name__ == "__main__":
    main()
