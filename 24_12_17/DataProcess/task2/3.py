import requests
from bs4 import BeautifulSoup

# 存储所有新闻标题的列表
all_titles = []

# 爬取前10页
for page in range(1, 11):
    url = f"https://news.cnblogs.com/n/page/{page}/"

    # 获取网页内容
    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.text, "html.parser")

        # 找到所有新闻标题
        titles = soup.find_all("h2", class_="news_entry")
        for title in titles:
            all_titles.append(title.get_text(strip=True))
    else:
        print(f"请求失败，状态码：{response.status_code}")

# 打印前10页新闻标题
for i, title in enumerate(all_titles, start=1):
    print(f"{i}. {title}")
