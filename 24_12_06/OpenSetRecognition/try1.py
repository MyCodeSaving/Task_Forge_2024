# 此文件尝试监督学习，但考虑到未知类别分类的复杂性，故废弃

import os
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder
from skimage.io import imread
from skimage.transform import resize
from skimage.util import random_noise
from skimage.color import rgb2gray
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox

# ------------------------------
# 数据集加载与预处理
# ------------------------------

def load_dataset(dataset_path, img_size=(64, 64)):
    data = []
    labels = []
    classes = sorted(os.listdir(dataset_path))
    for label, class_name in enumerate(classes):
        class_path = os.path.join(dataset_path, class_name)
        if os.path.isdir(class_path):
            for img_file in os.listdir(class_path):
                img_path = os.path.join(class_path, img_file)
                img = imread(img_path)
                img = resize(img, img_size)
                if img.ndim == 3:  # Convert RGB to grayscale
                    img = rgb2gray(img)
                data.append(img)
                labels.append(label)
    data = np.array(data, dtype=np.float32)
    labels = np.array(labels)
    data = (data - np.mean(data)) / np.std(data)  # Standardization
    return data, labels, classes

def augment_data(data, labels):
    augmented_data = []
    augmented_labels = []
    for img, label in zip(data, labels):
        augmented_data.append(img)  # Original
        augmented_data.append(np.flipud(img))  # Flip vertically
        augmented_data.append(np.fliplr(img))  # Flip horizontally
        augmented_data.append(random_noise(img))  # Add noise
        augmented_labels.extend([label] * 4)
    return (np.array(augmented_data), np.array(augmented_labels))

# ------------------------------
# 卷积神经网络实现
# ------------------------------

class ConvNet:
    def __init__(self, input_shape, num_classes):
        self.input_shape = input_shape
        self.num_classes = num_classes
        self.initialize_weights()

    def initialize_weights(self):
        self.conv1_filters = np.random.randn(8, 3, 3) * 0.1  # 8 filters, 3x3
        self.conv2_filters = np.random.randn(16, 3, 3) * 0.1
        self.fc1_weights = np.random.randn(16 * 16 * 16, 128) * 0.1  # Flattened
        self.fc2_weights = np.random.randn(128, self.num_classes) * 0.1
        self.fc1_bias = np.zeros(128)
        self.fc2_bias = np.zeros(self.num_classes)

    def relu(self, x):
        return np.maximum(0, x)

    def relu_derivative(self, x):
        return (x > 0).astype(float)

    def softmax(self, x):
        e_x = np.exp(x - np.max(x, axis=1, keepdims=True))
        return e_x / np.sum(e_x, axis=1, keepdims=True)

    # def convolve(self, x, filters):
    #     h, w = x.shape
    #     fh, fw = filters.shape[1], filters.shape[2]
    #     out = np.zeros((h - fh + 1, w - fw + 1, filters.shape[0]))
    #     for f in range(filters.shape[0]):
    #         for i in range(out.shape[0]):
    #             for j in range(out.shape[1]):
    #                 region = x[i:i+fh, j:j+fw]
    #                 out[i, j, f] = np.sum(region * filters[f])
    #     return out

    def convolve(self, x, filters):
        # 检查输入数据的维度
        if len(x.shape) == 3:  # (height, width, channels)
            h, w, d = x.shape
        elif len(x.shape) == 4:  # (batch_size, height, width, channels)
            batch_size, h, w, d = x.shape
        else:
            raise ValueError(f"Invalid input shape {x.shape}")

        fh, fw = filters.shape[1], filters.shape[2]  # 获取滤波器的尺寸
        out_h = h - fh + 1
        out_w = w - fw + 1
        out = np.zeros((out_h, out_w, filters.shape[0]))  # 输出大小 (h - fh + 1, w - fw + 1, filter数量)

        for f in range(filters.shape[0]):
            for i in range(out_h):
                for j in range(out_w):
                    region = x[i:i + fh, j:j + fw]
                    out[i, j, f] = np.sum(region * filters[f])  # 卷积操作
        return out

    def pool(self, x, pool_size=2, stride=2):
        h, w, d = x.shape
        out = np.zeros((h // pool_size, w // pool_size, d))
        for k in range(d):
            for i in range(0, h, stride):
                for j in range(0, w, stride):
                    out[i // stride, j // stride, k] = np.max(x[i:i+pool_size, j:j+pool_size, k])
        return out

    # def forward(self, x):
    #     self.conv1 = self.relu(self.convolve(x, self.conv1_filters))
    #     self.pool1 = self.pool(self.conv1)
    #     self.conv2 = self.relu(self.convolve(self.pool1, self.conv2_filters))
    #     self.pool2 = self.pool(self.conv2)
    #     self.flatten = self.pool2.flatten()
    #     self.fc1 = self.relu(np.dot(self.flatten, self.fc1_weights) + self.fc1_bias)
    #     self.fc2 = self.softmax(np.dot(self.fc1, self.fc2_weights) + self.fc2_bias)
    #     return self.fc2

    def forward(self, x):
        # 这里的 x 可能是单张图片，或者批量图像
        if len(x.shape) == 3:
            x = np.expand_dims(x, axis=0)  # 扩展为 (1, height, width, channels)，为了批量处理

        self.conv1 = self.relu(self.convolve(x, self.conv1_filters))
        self.pool1 = self.pool(self.conv1)
        self.conv2 = self.relu(self.convolve(self.pool1, self.conv2_filters))
        self.pool2 = self.pool(self.conv2)

        # Flatten the result before passing to fully connected layers
        self.flatten = self.pool2.flatten()
        self.fc1 = self.relu(np.dot(self.flatten, self.fc1_weights) + self.fc1_bias)
        self.fc2 = self.softmax(np.dot(self.fc1, self.fc2_weights) + self.fc2_bias)
        return self.fc2

    def backward(self, x, y, lr=0.001):
        # Compute gradients
        output_error = self.fc2 - y
        grad_fc2_weights = np.dot(self.fc1.T, output_error)
        grad_fc2_bias = np.sum(output_error, axis=0)

        hidden_error = np.dot(output_error, self.fc2_weights.T) * self.relu_derivative(self.fc1)
        grad_fc1_weights = np.dot(self.flatten.T, hidden_error)
        grad_fc1_bias = np.sum(hidden_error, axis=0)

        # Update weights
        self.fc2_weights -= lr * grad_fc2_weights
        self.fc2_bias -= lr * grad_fc2_bias
        self.fc1_weights -= lr * grad_fc1_weights
        self.fc1_bias -= lr * grad_fc1_bias

# ------------------------------
# 训练与可视化
# ------------------------------

def train_model(model, X_train, y_train, X_val, y_val, epochs=10, lr=0.001):
    history = {"loss": [], "val_loss": [], "accuracy": [], "val_accuracy": []}
    for epoch in range(epochs):
        train_loss, train_acc = 0, 0
        for i in range(len(X_train)):
            x = X_train[i]
            y = y_train[i]
            prediction = model.forward(x)
            train_loss += -np.sum(y * np.log(prediction + 1e-9))
            train_acc += np.argmax(prediction) == np.argmax(y)
            model.backward(x, y, lr)

        val_loss, val_acc = 0, 0
        for i in range(len(X_val)):
            x = X_val[i]
            y = y_val[i]
            prediction = model.forward(x)
            val_loss += -np.sum(y * np.log(prediction + 1e-9))
            val_acc += np.argmax(prediction) == np.argmax(y)

        history["loss"].append(train_loss / len(X_train))
        history["accuracy"].append(train_acc / len(X_train))
        history["val_loss"].append(val_loss / len(X_val))
        history["val_accuracy"].append(val_acc / len(X_val))

        print(f"Epoch {epoch + 1}/{epochs} - "
              f"Loss: {train_loss:.4f}, Accuracy: {train_acc / len(X_train):.4f} - "
              f"Val Loss: {val_loss:.4f}, Val Accuracy: {val_acc / len(X_val):.4f}")
    return history

# ------------------------------
# 性能计算
# ------------------------------

def compute_score(model, X_test, y_test):
    test_acc = 0
    for i in range(len(X_test)):
        x = X_test[i]
        y = y_test[i]
        prediction = model.forward(x)
        test_acc += np.argmax(prediction) == np.argmax(y)
    return test_acc / len(X_test)

# ------------------------------
# Tkinter 界面
# ------------------------------

def run_gui(model, classes):
    def classify_image():
        file_path = filedialog.askopenfilename()
        if file_path:
            img = imread(file_path)
            img = resize(img, (64, 64))
            if img.ndim == 3:
                img = rgb2gray(img)
            prediction = model.forward(img)
            class_label = classes[np.argmax(prediction)]
            messagebox.showinfo("Classification Result", f"Predicted Class: {class_label}")

    root = tk.Tk()
    root.title("Image Classifier")
    upload_button = tk.Button(root, text="Upload Image", command=classify_image)
    upload_button.pack()
    root.mainloop()

# ------------------------------
# 主程序
# ------------------------------

if __name__ == "__main__":
    dataset_path = "./dataset1/train"
    img_size = (64, 64)
    data, labels, classes = load_dataset(dataset_path, img_size)
    (data, labels) = augment_data(data, labels)
    X_train, X_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, random_state=42)

    X_train = X_train[..., np.newaxis]  # 添加通道维度
    X_test = X_test[..., np.newaxis]  # 添加通道维度

    # One-hot encoding labels
    encoder = OneHotEncoder(sparse_output=False)    # 在较新的 scikit-learn 版本中，OneHotEncoder 的 sparse 参数已被替换为 sparse_output
    y_train = encoder.fit_transform(y_train.reshape(-1, 1))
    y_test = encoder.transform(y_test.reshape(-1, 1))

    # Initialize and train model
    model = ConvNet(input_shape=(64, 64), num_classes=len(classes))
    history = train_model(model, X_train, y_train, X_test, y_test)

    # Save model and run GUI
    run_gui(model, classes)
