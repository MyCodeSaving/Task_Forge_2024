# 需要pip安装openpyxl库

import re
import pandas as pd

def extract_address_lat_lng(line):
    """
    从每行文本中提取地址、纬度和经度
    假设格式为：地址 -> 纬度: X, 经度: Y
    """
    # 正则表达式匹配地址、纬度、经度
    match = re.match(r"(.*?) -> 纬度: ([\d.-]+), 经度: ([\d.-]+)", line)
    if match:
        address = match.group(1).strip()
        latitude = float(match.group(2))
        longitude = float(match.group(3))
        return address, latitude, longitude
    return None, None, None

def process_address_file(input_file, output_file):
    """
    从文件中读取地址并提取，最后将结果存储到 Excel 文件
    """
    addresses = []
    latitudes = []
    longitudes = []

    try:
        # 读取文本文件
        with open(input_file, 'r', encoding='utf-8') as infile:
            for line in infile:
                address, latitude, longitude = extract_address_lat_lng(line.strip())
                if address and latitude and longitude:
                    addresses.append(address)
                    latitudes.append(latitude)
                    longitudes.append(longitude)

        # 创建 DataFrame 并保存为 Excel
        df = pd.DataFrame({
            '地址': addresses,
            '纬度': latitudes,
            '经度': longitudes
        })
        df.to_excel(output_file, index=False)

        print(f"地址、纬度和经度已成功保存到 {output_file}")

    except Exception as e:
        print(f"处理文件时出错: {e}")

# 输入和输出文件路径
input_file = "result.txt"  # 输入文件路径
output_file = "addresses_with_coordinates.xlsx"  # 输出的 Excel 文件

# 处理文件
process_address_file(input_file, output_file)
