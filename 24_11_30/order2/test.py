import json
import os
import csv
import joblib
import re
import numpy as np
import chardet

# 1. 读取数据集
with open('./data/code/testdataset.json', 'r') as f:
    dataset = json.load(f)

# 提取 patch 文件内容
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

# 2. 特征提取函数
def extract_static_features(patch_content):
    # 统计新增和删除的行数
    added_lines = len(re.findall(r"^\+[^+]", patch_content, re.MULTILINE))
    deleted_lines = len(re.findall(r"^-", patch_content, re.MULTILINE))
    # 统计关键词数量（以 C/C++ 为例）
    keywords = ["if", "for", "while", "return"]
    keyword_count = sum(len(re.findall(rf"\b{kw}\b", patch_content)) for kw in keywords)
    return [added_lines, deleted_lines, keyword_count]

def extract_tfidf_features(patch_contents, vectorizer):
    return vectorizer.transform(patch_contents).toarray()

# 3. 模型加载与预测
# 加载预训练的模型
model = joblib.load('model.pkl')  # 确保文件名与训练时一致
vectorizer = joblib.load('tfidf_vectorizer.pkl')  # 加载训练时使用的 TF-IDF 向量化器

# 4. 特征提取
results = []
patch_contents = list(patch_files.values())
tfidf_features = extract_tfidf_features(patch_contents, vectorizer)

# 组合特征
combined_features = []
for commit, content in patch_files.items():
    static_features = extract_static_features(content)
    patch_tfidf_features = tfidf_features[patch_contents.index(content)]
    combined_features.append(np.hstack([static_features, patch_tfidf_features]))

# 5. 进行预测
for i, patch in enumerate(dataset):
    commit = patch['commit']
    features = combined_features[i]
    probability = model.predict_proba([features])[0][1]
    prediction = model.predict([features])[0]

    vulnerability_type = prediction  # 获取漏洞类型

    results.append([commit, probability, 1, vulnerability_type])

# 6. 输出结果
with open('result.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['commit', 'probability', 'prediction', 'vulnerability_type'])
    writer.writerows(results)
