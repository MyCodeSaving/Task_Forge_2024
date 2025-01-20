import requests
from bs4 import BeautifulSoup
import pandas as pd
import time

# 设置请求头，模拟浏览器访问
headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
}

# 目标 URL 模板
base_url = "http://bang.dangdang.com/books/bestsellers/01.00.00.00.00.00-recent7-0-0-1-{}"


# 定义爬取函数
def scrape_dangdang_top500():
    all_books = []  # 存储所有图书数据
    total_pages = 10  # 爬取前 10 页数据

    for page in range(1, total_pages + 1):
        print(f"正在爬取第 {page} 页数据...")
        url = base_url.format(page)

        # 发送请求
        response = requests.get(url, headers=headers)
        if response.status_code != 200:
            print(f"请求失败，状态码：{response.status_code}")
            continue

        # 解析 HTML 内容
        soup = BeautifulSoup(response.text, "html.parser")
        book_items = soup.find_all("ul", class_="bang_list")[0].find_all("li")

        # 遍历当前页的每一本图书
        for item in book_items:
            try:
                # 提取图书名称
                title = item.find("div", class_="name").find("a").get_text(strip=True)

                # 提取作者
                author_tag = item.find("div", class_="publisher_info").find_all("a")
                author = author_tag[0].get_text(strip=True) if author_tag else "未知"

                # 提取出版社
                publisher = author_tag[1].get_text(strip=True) if len(author_tag) > 1 else "未知"

                # 提取出版时间
                publish_time = item.find("div", class_="publisher_info").get_text(strip=True).split("/")[-1]

                # 提取价格
                price = item.find("span", class_="price_n").get_text(strip=True)

                # 保存到列表
                all_books.append({
                    "图书名称": title,
                    "作者": author,
                    "出版社": publisher,
                    "出版时间": publish_time,
                    "价格": price
                })
            except Exception as e:
                print(f"数据提取出错：{e}")
                continue

        # 避免请求过快被封禁，设置延迟
        time.sleep(1)

    print("数据爬取完成！")
    return all_books


# 主函数
def main():
    # 爬取数据
    books_data = scrape_dangdang_top500()

    # 保存到 CSV 文件
    if books_data:
        df = pd.DataFrame(books_data)
        df.to_csv("dangdang_top500_books.csv", index=False, encoding="utf-8-sig")
        print("数据已保存到 dangdang_top500_books.csv")
    else:
        print("未获取到任何数据！")


if __name__ == "__main__":
    main()
