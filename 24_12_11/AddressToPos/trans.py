import re
from collections import OrderedDict


def standardize_address(address):
    """
    将不规则地址转换为标准化格式：街道, 城市, 省份, 国家。
    假设地址都在中国，默认追加"中国"。
    """
    # 去除多余的空格和符号
    address = re.sub(r'\s+', '', address)
    # print(address)
    address = re.sub(r'\|', '', address)
    # print(address)

    # # 提取省份、城市、区/县等
    # match = re.match(r"^(.*?省|.*?自治区)?(.*?市|.*?州)?(.*?区|.*?县|.*?镇|.*?村|.*?街)?(.*)", address)
    # if match:
    #     province, city, district, street = match.groups()
    #     # 构造标准格式
    #     standardized = ", ".join(filter(None, [street, district, city, province, "中国"]))
    #     return standardized
    return address


def process_addresses(input_file, output_file):
    """
    从文件中读取地址并处理，去除重复地址，保留顺序，写入标准化结果到新文件。
    """
    try:
        # 读取地址文件
        with open(input_file, 'r', encoding='utf-8') as infile:
            addresses = infile.readlines()

        # 使用 OrderedDict 来去重并保持顺序
        standardized_addresses = OrderedDict()

        for address in addresses:
            standardized_address = standardize_address(address.strip())
            # 将标准化的地址加入 OrderedDict，键是地址，值可以为空，自动去重
            standardized_addresses[standardized_address] = None

        # 将标准化后的地址保存到新文件
        with open(output_file, 'w', encoding='utf-8') as outfile:
            outfile.write("\n".join(standardized_addresses.keys()))  # 只写键，保留顺序

        print(f"地址处理完成，标准化结果已保存到 {output_file}")
    except Exception as e:
        print(f"处理文件时出现错误: {e}")


# 输入和输出文件路径
input_file = "address.txt"
output_file = "standardized_address.txt"

# 处理地址
process_addresses(input_file, output_file)