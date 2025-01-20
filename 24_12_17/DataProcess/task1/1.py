import requests
from bs4 import BeautifulSoup
import pandas as pd
from datetime import datetime

# 目标 URL
url = "https://news.ifeng.com/c/8fO7YAofX9e"


# 定义爬取函数
def scrape_news_detail(url):
    try:
        # 发送 HTTP 请求
        response = requests.get(url)
        if response.status_code != 200:
            print(f"请求失败，状态码：{response.status_code}")
            return None

        # 解析 HTML 页面
        soup = BeautifulSoup(response.text, "html.parser")

        # 提取标题
        title = soup.find("h1").get_text(strip=True)

        # 提取发布时间
        time_tag = soup.find("span", class_="index_date_RDRuA")
        publish_time = time_tag.get_text(strip=True) if time_tag else "未知"

        # 提取来源
        source_tag = soup.find("span", class_="index_source_EZq0n")
        source = source_tag.get_text(strip=True) if source_tag else "未知"

        # 提取文章内容
        content_div = soup.find("div", class_="index_text_D0U1y")
        if content_div:
            paragraphs = content_div.find_all("p")
            content = "\n".join([p.get_text(strip=True) for p in paragraphs])
        else:
            content = "文章内容获取失败"

        # # 提取分类（假设从导航路径中获取）
        # nav_links = soup.find("div", class_="nav").find_all("a")
        # category = nav_links[-1].get_text(strip=True) if nav_links else "未知"

        # 返回数据
        return {
            "标题": title,
            # "分类": category,
            "来源": source,
            "发布时间": publish_time,
            "爬取时间": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "文章内容": content
        }

    except Exception as e:
        print(f"爬取失败：{e}")
        return None


# 主函数
def main():
    # 采集单篇新闻数据
    news_data = scrape_news_detail(url)
    if news_data:
        print("爬取成功！数据如下：")
        for key, value in news_data.items():
            print(f"{key}: {value}")

        # 保存为 CSV 文件
        df = pd.DataFrame([news_data])
        df.to_csv("news.csv", index=False, encoding="utf-8-sig")
        print("数据已保存到 news.csv")


if __name__ == "__main__":
    main()
