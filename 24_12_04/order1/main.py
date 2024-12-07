import requests
from bs4 import BeautifulSoup
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import time

from matplotlib import rcParams

# 设置字体，解决中文显示问题
rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
rcParams['axes.unicode_minus'] = False   # 解决负号显示问题

# 爬取豆瓣电影Top 250数据
def fetch_douban_top250():
    base_url = "https://movie.douban.com/top250"
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36"
    }
    movies = []

    for page in range(10):  # 每页25部电影，共10页
        url = f"{base_url}?start={page * 25}"
        response = requests.get(url, headers=headers)
        soup = BeautifulSoup(response.text, "html.parser")

        # 解析电影数据
        items = soup.find_all("div", class_="item")
        for item in items:
            title = item.find("span", class_="title").text
            rating = float(item.find("span", class_="rating_num").text)
            people = item.find("div", class_="star").find_all("span")[-1].text
            people_count = int(people[:-3].replace(",", ""))  # 提取评价人数
            movies.append({"title": title, "rating": rating, "people_count": people_count})

        print(f"Page {page + 1} data fetched.")
        time.sleep(2)  # 遵守爬虫礼仪，防止被封

    return movies

# 数据可视化
def visualize_data(movies):
    # 转换为DataFrame
    df = pd.DataFrame(movies)

    # 电影评分分布
    plt.figure(figsize=(10, 5))
    plt.hist(df["rating"], bins=10, color="skyblue", edgecolor="black")
    plt.title("Rating Distribution of Top 250 Movies")
    plt.xlabel("Rating")
    plt.ylabel("Number of Movies")
    plt.grid(axis="y", linestyle="--", alpha=0.7)
    plt.show()

    # 评价人数Top 10电影
    top10 = df.nlargest(10, "people_count")
    plt.figure(figsize=(12, 6))
    plt.barh(top10["title"], top10["people_count"], color="lightcoral")
    plt.title("Top 10 Movies with Most Reviews")
    plt.xlabel("Number of Reviews")
    plt.ylabel("Movie Title")
    plt.gca().invert_yaxis()  # 翻转Y轴
    plt.show()

    # 评分最高的10部电影
    top_rated = df.nlargest(10, "rating")
    plt.figure(figsize=(12, 6))
    plt.barh(top_rated["title"], top_rated["rating"], color="lightgreen")
    plt.title("Top 10 Highest Rated Movies")
    plt.xlabel("Rating")
    plt.ylabel("Movie Title")
    plt.gca().invert_yaxis()
    plt.show()

# 数据分析
def analyze_data(movies):
    df = pd.DataFrame(movies)

    # 1. 平均评分和评价人数
    avg_rating = df["rating"].mean()
    avg_people_count = df["people_count"].mean()
    print(f"Average rating: {avg_rating:.2f}")
    print(f"Average number of reviews: {avg_people_count:.0f}")

    # 2. 评分区间分布
    bins = [0, 7, 8, 9, 10]
    labels = ["0-7", "7-8", "8-9", "9-10"]
    df["rating_group"] = pd.cut(df["rating"], bins=bins, labels=labels, right=False)
    rating_distribution = df["rating_group"].value_counts().sort_index()
    print("\nRating interval distribution:")
    print(rating_distribution)

    # 可视化评分区间分布
    plt.figure(figsize=(8, 5))
    plt.bar(rating_distribution.index, rating_distribution.values, color="orchid")
    plt.title("Rating Interval Distribution")
    plt.xlabel("Rating Interval")
    plt.ylabel("Number of Movies")
    plt.grid(axis="y", linestyle="--", alpha=0.7)
    plt.show()

    # 3. 最低评分电影
    lowest_rated = df.nsmallest(5, "rating")
    print("\nLowest Rated 5 Movies:")
    print(lowest_rated[["title", "rating", "people_count"]])

    # 4. 评价人数和评分的相关性
    correlation = df["rating"].corr(df["people_count"])
    print(f"\nCorrelation between rating and number of reviews: {correlation:.2f}")

    # 相关性可视化
    plt.figure(figsize=(10, 6))
    plt.scatter(df["people_count"], df["rating"], alpha=0.7, color="teal")
    plt.title("Relationship between Rating and Number of Reviews")
    plt.xlabel("Number of Reviews")
    plt.ylabel("Rating")
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.show()

if __name__ == "__main__":
    print("Fetching data, please wait...")
    movies_data = fetch_douban_top250()
    print("Data fetching completed!\nStarting visualization and analysis...")

    # 可视化
    visualize_data(movies_data)

    # 数据分析
    analyze_data(movies_data)
