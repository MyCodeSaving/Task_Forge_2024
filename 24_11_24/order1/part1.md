### 第一部分：物理结构设计（关系表结构）

#### 教学任务书之学生

| 列名     | 数据类型    | 约束 |
| -------- | ----------- | ---- |
| 教学班号 | varchar(20) | 外键 |
| 学号     | varchar(20) |      |
| 选课日期 | datetime    |      |
| id       | int         | 主键 |

#### 教学任务书之教师

| 列名     | 数据类型    | 约束 |
| -------- | ----------- | ---- |
| 教学班号 | varchar(20) | 外键 |
| 教师编号 | varchar(20) |      |
| 是否主讲 | bit(1)      |      |
| id       | int         | 主键 |

#### 日历信息表

| 列名     | 数据类型    | 约束 |
| -------- | ----------- | ---- |
| 学期     | varchar(40) | 主键 |
| 开始日期 | date        |      |
| 周数     | int         |      |
| 年份     | int         |      |

#### 教学任务书

| 列名     | 数据类型     | 约束 |
| -------- | ------------ | ---- |
| 教学班号 | varchar(20)  | 主键 |
| 课程编号 | varchar(20)  |      |
| 学时     | varchar(20)  |      |
| 理论学时 | varchar(20)  |      |
| 实验学时 | varchar(20)  |      |
| 课程名称 | varchar(50)  |      |
| 学分     | float        |      |
| 理论学分 | float        |      |
| 实验学分 | float        |      |
| 备注     | varchar(400) |      |
| 学期     | varchar(40)  | 外键 |

#### 教学任务书之教材

| 列名     | 数据类型    | 约束 |
| -------- | ----------- | ---- |
| 教学班号 | varchar(20) | 外键 |
| 教材名称 | varchar(50) |      |
| 版本     | varchar(20) |      |
| 作者     | varchar(50) |      |
| 书刊号   | varchar(40) |      |
| id       | int         | 主键 |

#### 教室信息表

| 列名       | 数据类型    | 约束 |
| ---------- | ----------- | ---- |
| 教室编号   | varchar(20) | 主键 |
| 名称       | varchar(40) |      |
| 类型       | varchar(20) |      |
| 座位数     | int         |      |
| 是否多媒体 | bit(1)      |      |
| 考试座位数 | int         |      |
| 教学楼     | varchar(40) |      |
| 校区       | varchar(10) |      |

#### 教材信息表

| 列名     | 数据类型     | 约束 |
| -------- | ------------ | ---- |
| ISBN     | varchar(40)  | 主键 |
| 教材名称 | varchar(255) |      |
| 版本     | varchar(255) |      |
| 作者     | varchar(255) |      |
| 出版年份 | year         |      |

#### 教学任务书之时间教室

| 列名     | 数据类型    | 约束 |
| -------- | ----------- | ---- |
| 教学班号 | varchar(20) | 外键 |
| 上课日期 | date        |      |
| 课次     | int         |      |
| 连续学时 | int         |      |
| 课程类型 | varchar(20) |      |
| 教师编号 | varchar(20) |      |
| id       | int         | 主键 |
| 教室编号 | varchar(20) |      |
| 星期几   | varchar(10) |      |
| 第几周   | int         |      |
| 周时间段 | varchar(20) |      |

### 第二部分：数据库对象设计

#### 2.1 数据库命令

以下是一些常见的数据库命令，涵盖了表的创建、插入数据、删除数据等操作。

**创建表命令示例：**

1. 创建 `教学任务书之学生` 表

```sql
CREATE TABLE 教学任务书之学生 (
    教学班号 VARCHAR(20) NOT NULL,
    学号 VARCHAR(20) NOT NULL,
    选课日期 DATETIME,
    id INT AUTO_INCREMENT PRIMARY KEY,
    FOREIGN KEY (教学班号) REFERENCES 教学任务书 (教学班号)
);
```

1. 创建 `教学任务书之教师` 表

```sql
CREATE TABLE 教学任务书之教师 (
    教学班号 VARCHAR(20) NOT NULL,
    教师编号 VARCHAR(20) NOT NULL,
    是否主讲 BIT(1),
    id INT AUTO_INCREMENT PRIMARY KEY,
    FOREIGN KEY (教学班号) REFERENCES 教学任务书 (教学班号)
);
```

1. 创建 `日历信息表` 表

```sql
CREATE TABLE 日历信息表 (
    学期 VARCHAR(40) PRIMARY KEY,
    开始日期 DATE,
    周数 INT,
    年份 INT
);
```

1. 创建 `教学任务书` 表

```sql
CREATE TABLE 教学任务书 (
    教学班号 VARCHAR(20) PRIMARY KEY,
    课程编号 VARCHAR(20),
    学时 VARCHAR(20),
    理论学时 VARCHAR(20),
    实验学时 VARCHAR(20),
    课程名称 VARCHAR(50),
    学分 FLOAT,
    理论学分 FLOAT,
    实验学分 FLOAT,
    备注 VARCHAR(400),
    学期 VARCHAR(40),
    FOREIGN KEY (学期) REFERENCES 日历信息表 (学期)
);
```

1. 创建 `教学任务书之教材` 表

```sql
CREATE TABLE 教学任务书之教材 (
    教学班号 VARCHAR(20),
    教材名称 VARCHAR(50),
    版本 VARCHAR(20),
    作者 VARCHAR(50),
    书刊号 VARCHAR(40),
    id INT AUTO_INCREMENT PRIMARY KEY,
    FOREIGN KEY (教学班号) REFERENCES 教学任务书 (教学班号)
);
```

1. 创建 `教室信息表` 表

```sql
CREATE TABLE 教室信息表 (
    教室编号 VARCHAR(20) PRIMARY KEY,
    名称 VARCHAR(40),
    类型 VARCHAR(20),
    座位数 INT,
    是否多媒体 BIT(1),
    考试座位数 INT,
    教学楼 VARCHAR(40),
    校区 VARCHAR(10)
);
```

1. 创建 `教材信息表` 表

```sql
CREATE TABLE 教材信息表 (
    ISBN VARCHAR(40) PRIMARY KEY,
    教材名称 VARCHAR(255),
    版本 VARCHAR(255),
    作者 VARCHAR(255),
    出版年份 YEAR
);
```

1. 创建 `教学任务书之时间教室` 表

```sql
CREATE TABLE 教学任务书之时间教室 (
    教学班号 VARCHAR(20),
    上课日期 DATE,
    课次 INT,
    连续学时 INT,
    课程类型 VARCHAR(20),
    教师编号 VARCHAR(20),
    id INT AUTO_INCREMENT PRIMARY KEY,
    教室编号 VARCHAR(20),
    星期几 VARCHAR(10),
    第几周 INT,
    周时间段 VARCHAR(20),
    FOREIGN KEY (教学班号) REFERENCES 教学任务书 (教学班号),
    FOREIGN KEY (教室编号) REFERENCES 教室信息表 (教室编号)
);
```

#### 2.2 数据表

在第一部分中已经列出了所有的数据表结构，具体包括各个表的列名、数据类型和约束。这里无需再次列出，只需要确保在创建数据库时执行了相应的 `CREATE TABLE` 语句。

#### 2.3 索引建立

为了提高查询效率，特别是在涉及外键和常用查询字段时，可以为相应的列创建索引。以下是几个创建索引的例子：

1. 为 `教学任务书之学生` 表的 `教学班号` 创建索引：

```sql
CREATE INDEX idx_教学任务书之学生_教学班号 ON 教学任务书之学生(教学班号);
```

1. 为 `教学任务书之教师` 表的 `教师编号` 创建索引：

```sql
CREATE INDEX idx_教学任务书之教师_教师编号 ON 教学任务书之教师(教师编号);
```

1. 为 `教学任务书` 表的 `课程编号` 创建索引：

```sql
CREATE INDEX idx_教学任务书_课程编号 ON 教学任务书(课程编号);
```

1. 为 `教材信息表` 表的 `ISBN` 创建索引：

```sql
CREATE INDEX idx_教材信息表_ISBN ON 教材信息表(ISBN);
```

#### 2.4 视图建立

视图可以帮助我们简化复杂查询的实现，以下是一些可能的视图创建示例：

1. 创建一个视图，显示每个教师与其所授课程的信息：

```sql
CREATE VIEW 教师课程信息 AS
SELECT 教学任务书之教师.教师编号, 教学任务书.课程名称, 教学任务书.学分
FROM 教学任务书之教师
JOIN 教学任务书 ON 教学任务书之教师.教学班号 = 教学任务书.教学班号;
```

1. 创建一个视图，显示所有教材的信息及对应的课程：

```sql
CREATE VIEW 教材课程信息 AS
SELECT 教学任务书之教材.教材名称, 教学任务书之教材.作者, 教学任务书.课程名称
FROM 教学任务书之教材
JOIN 教学任务书 ON 教学任务书之教材.教学班号 = 教学任务书.教学班号;
```

1. 创建一个视图，显示所有教室的使用信息：

```sql
CREATE VIEW 教室使用情况 AS
SELECT 教室信息表.教室编号, 教室信息表.名称, 教室信息表.类型, 教学任务书之时间教室.上课日期, 教学任务书之时间教室.课次
FROM 教室信息表
JOIN 教学任务书之时间教室 ON 教室信息表.教室编号 = 教学任务书之时间教室.教室编号;
```

