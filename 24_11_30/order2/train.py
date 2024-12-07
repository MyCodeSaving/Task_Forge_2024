import json
import os
import joblib
import chardet

# 加载 JSON 数据集
with open("./data/code/updated_data2.json", "r") as f:
    dataset = json.load(f)
patch_dir = "./data/code/patchfiles/"  # 补丁文件所在文件夹
patch_files = {}
for item in dataset:
    commit = item["commit"]
    patch_path = os.path.join(patch_dir, f"{commit}.patch")
    try:
        # 自动检测编码
        with open(patch_path, 'rb') as f:
            raw_data = f.read()
            result = chardet.detect(raw_data)
            encoding = result['encoding']
        # 根据检测到的编码读取文件
        with open(patch_path, 'r', encoding=encoding) as patch_file:
            patch_files[commit] = patch_file.read()
    except FileNotFoundError:
        print(f"Patch file for commit {commit} not found.")
        print(patch_path)

# 静态特征提取
import re
def extract_static_features(patch_content):
    # 统计新增和删除的行数
    added_lines = len(re.findall(r"^\+[^+]", patch_content, re.MULTILINE))
    deleted_lines = len(re.findall(r"^-", patch_content, re.MULTILINE))
    # 统计关键词数量（以 C/C++ 为例）
    keywords = ["if", "for", "while", "return", "int", "void", "char", "def", "class", "import", "public", "private", "protected", "static", "else"]
    keyword_count = sum(len(re.findall(rf"\b{kw}\b", patch_content)) for kw in keywords)
    return [added_lines, deleted_lines, keyword_count]
features = []
for commit, content in patch_files.items():
    features.append(extract_static_features(content))

# TF-IDF
from sklearn.feature_extraction.text import TfidfVectorizer
# 创建 TF-IDF 向量
patch_contents = list(patch_files.values())
vectorizer = TfidfVectorizer(max_features=1000)
tfidf_features = vectorizer.fit_transform(patch_contents).toarray()
joblib.dump(vectorizer, 'tfidf_vectorizer.pkl')

# 组合特征
import numpy as np
combined_features = np.hstack([features, tfidf_features])

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split

# 提取补丁类型作为标签
labels = [patch["vulnerability_type"] for patch in dataset]

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(combined_features, labels, test_size=0.3, random_state=42)

# 训练随机森林模型
model = RandomForestClassifier()
model.fit(X_train, y_train)

# 保存模型
joblib.dump(model, "model.pkl")

