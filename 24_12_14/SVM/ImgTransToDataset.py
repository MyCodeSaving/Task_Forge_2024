import os
from PIL import Image

# 定义路径
class1_path = "images/cats/"  # 类别1图片路径
class2_path = "images/dogs/"  # 类别2图片路径
output_path = "dataset/"  # 输出路径
os.makedirs(output_path, exist_ok=True)


# 定义图片处理函数
def process_and_save_images(input_path, output_folder, prefix, size=(32, 32)):
    """
    处理图片并保存到指定文件夹
    :param input_path: 输入图片路径
    :param output_folder: 输出文件夹
    :param prefix: 输出文件名前缀
    :param size: 调整后的图片尺寸 (宽, 高)
    """
    os.makedirs(output_folder, exist_ok=True)  # 确保输出文件夹存在
    count = 0
    for file in os.listdir(input_path):
        img_path = os.path.join(input_path, file)
        try:
            # 加载图片并调整大小
            img = Image.open(img_path).convert("RGB")
            img_resized = img.resize(size)

            # 保存图片，命名为 "prefix_0.jpg", "prefix_1.jpg" ...
            output_file = os.path.join(output_folder, f"{prefix}_{count}.jpg")
            img_resized.save(output_file)
            count += 1
        except Exception as e:
            print(f"跳过无法处理的图片: {file}, 错误: {e}")


# 处理并保存类别1和类别2的图片
process_and_save_images(class1_path, output_path, "cats/cat", size=(32, 32))
process_and_save_images(class2_path, output_path, "dogs/dog", size=(32, 32))

print("图片处理完成，已保存到 dataset 文件夹！")
