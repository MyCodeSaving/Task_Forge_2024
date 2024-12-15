import os
import numpy as np
from PIL import Image
from sklearn.svm import SVC
from sklearn.metrics import classification_report, accuracy_score
from sklearn.model_selection import train_test_split

# 设置图片路径
class1_path = "dataset/cats/"  # 类别1图片路径
class2_path = "dataset/dogs/"  # 类别2图片路径

# 加载图片数据
def load_images(path, label):
    images = []
    labels = []
    for file in os.listdir(path):
        img_path = os.path.join(path, file)
        try:
            img = Image.open(img_path)
            images.append(np.array(img).flatten())  # 展平为1D数组
            labels.append(label)
        except Exception as e:
            print(f"跳过无法加载的图片: {file}, 错误: {e}")
    return images, labels

# 加载两类数据
class1_images, class1_labels = load_images(class1_path, 0)  # 类别1标记为0
class2_images, class2_labels = load_images(class2_path, 1)  # 类别2标记为1

# 合并数据
X = np.array(class1_images + class2_images)
y = np.array(class1_labels + class2_labels)

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 训练SVM分类器
svm = SVC(kernel='linear', random_state=42)
svm.fit(X_train, y_train)

# 测试并输出结果
y_pred = svm.predict(X_test)
print("分类报告:")
print(classification_report(y_test, y_pred))
print("准确率:", accuracy_score(y_test, y_pred))
