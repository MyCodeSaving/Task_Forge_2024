**这是个人账单管理系统的工作目录。**

#### 文件架构：

1.  `build` 文件夹和 `dist` 文件夹是根据 `bill_manager.spec` 文件由 `pyinstaller` 打包成的，
    其中 `dist` 文件夹下的 `exe` 文件可以直接运行；
2.  `resources` 文件夹存放数据 `bills_data.json` ；
3.  `bill_manager.py` 是主程序文件；
4.   `figs` 是屏幕截图等演示图片的存放位置；

### 解释文档

#### 1. 简介

这个代码实现了一个简单的个人账单管理系统，帮助用户记录收入和支出，并进行查询、统计以及预算管理。系统将数据保存在一个 JSON 文件中，允许用户在程序关闭后仍能保存账单信息。

#### 2. 代码结构

整个系统包括几个主要功能：

- 记录收入和支出。
- 展示和查询账单。
- 设置月度预算并生成统计报告。
- 数据持久化：使用 JSON 文件存储数据。
- 错误处理：确保用户输入的日期和金额合法。

#### 3. 主要函数和流程

##### 3.1 `resource_path(relative_path)`

```python
def resource_path(relative_path):
    """获取资源文件的路径，兼容打包后的情况"""
    try:
        # PyInstaller会将资源打包到 _MEIPASS 文件夹
        base_path = sys._MEIPASS
    except AttributeError:
        base_path = os.path.abspath(".")  # 如果不是打包后的环境，使用当前工作目录
    return os.path.join(base_path, relative_path)  # 拼接路径并返回
```

**功能**：获取资源文件（如 `bills_data.json`）的正确路径。无论是在开发环境运行，还是打包后的执行文件中，都能正确获取资源文件路径。

- **为什么要这样做**：打包程序后，资源文件（如数据文件）会被存放在一个临时目录（`_MEIPASS`）中。我们需要通过这个函数来确保程序能够正确访问这些文件。

##### 3.2 `load_data()`

```python
def load_data():
    """加载本地文件数据"""
    global data
    if os.path.exists(DATA_FILE):  # 如果文件存在
        with open(DATA_FILE, "r", encoding="utf-8") as file:
            data = json.load(file)  # 从JSON文件加载数据
```

**功能**：加载存储的账单数据。程序启动时会自动读取 `bills_data.json` 文件，获取之前保存的收入、支出记录以及月度预算。

##### 3.3 `save_data()`

```python
def save_data():
    """保存数据到本地文件"""
    with open(DATA_FILE, "w", encoding="utf-8") as file:
        json.dump(data, file, ensure_ascii=False, indent=4)  # 将数据保存到JSON文件
```

**功能**：将当前数据（收入、支出记录等）保存到 `bills_data.json` 文件中，确保数据持久化存储。每次新增记录时，都会自动保存。

##### 3.4 `is_valid_date(date_str)`

```python
def is_valid_date(date_str):
    """验证日期格式是否正确"""
    try:
        datetime.datetime.strptime(date_str, "%Y-%m-%d")  # 尝试将输入的字符串转换为日期
        return True  # 格式正确
    except ValueError:
        return False  # 日期格式错误
```

**功能**：验证用户输入的日期是否符合正确的格式（`YYYY-MM-DD`）。如果格式不正确，系统会提示用户重新输入。

##### 3.5 `add_income()` 和 `add_expense()`

```python
def add_income():
    """记录收入"""
    # 省略输入部分
    # 检查日期和金额是否有效，然后将收入信息添加到数据中

def add_expense():
    """记录支出"""
    # 省略输入部分
    # 检查日期和金额是否有效，然后将支出信息添加到数据中
```

**功能**：

- `add_income()`：记录用户的收入信息，包括日期、金额、类别和备注，并保存到数据文件中。
- `add_expense()`：记录用户的支出信息，同样包括日期、金额、类别和备注，并保存到数据文件中。

这两个函数包含了对用户输入的检查，确保日期格式和金额是合法的。

##### 3.6 `show_bills()`

```python
def show_bills():
    """展示所有账单"""
    print("\n所有收入记录：")
    for income in data["incomes"]:  # 遍历所有收入
        print(f"日期：{income['date']}，金额：{income['amount']}，类别：{income['category']}，备注：{income['remark']}")

    print("\n所有支出记录：")
    for expense in data["expenses"]:  # 遍历所有支出
        print(f"日期：{expense['date']}，金额：{expense['amount']}，类别：{expense['category']}，备注：{expense['remark']}")
```

**功能**：显示所有已记录的收入和支出信息。

##### 3.7 `query_bills()`

```python
def query_bills():
    """查询账单"""
    # 根据用户输入的查询条件（日期、日期范围或类别），筛选出符合条件的账单记录
```

**功能**：用户可以根据日期、日期范围或类别查询账单，系统会根据选择进行筛选并展示相应的记录。

##### 3.8 `set_budget()`

```python
def set_budget():
    """设置月度预算"""
    try:
        budget = float(input("请输入每月预算金额："))
        if budget <= 0:
            print("预算金额必须为正数！")
            return
        data["monthly_budget"] = budget
        save_data()  # 保存设置的预算
        print("预算已设置成功！")
    except ValueError:
        print("金额输入错误，请输入数字！")
```

**功能**：设置每月的预算金额。用户可以输入预算金额，如果输入有效，会保存到数据文件中。

##### 3.9 `monthly_report()`

```python
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
```

**功能**：生成并展示月度统计报告，包含总收入、总支出、剩余预算及各类别的支出金额。

##### 3.10 `main()`

```python
def main():
    load_data()  # 加载数据
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

        # 根据用户输入的选项调用不同的功能
```

**功能**：系统主循环，用户可以通过菜单选择不同的功能，程序会调用相应的函数执行操作。

#### 4. 总结

这个个人账单管理系统是一个简单的财务管理工具，能够帮助用户记录收入和支出，设置预算，生成月度报告。通过文件持久化存储，确保数据在系统关闭后不会丢失。用户友好的错误提示和输入验证，能够确保系统的健壮性和稳定性。