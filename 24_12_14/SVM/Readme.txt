#### 文件架构

- images：
  - 用爬虫爬取的原始图像（初始每个类别爬取100张图片，后手动清洗无关图片）；

- dataset：
  - 预处理后的图像数据集；

- figs：
  - 演示图片，包括代码运行结果等；

- FetchResource.py：
  - 爬虫程序（爬取原始图像数据）；

- ImgTransToDataset.py：
  - 图像预处理程序；

- Svm.py：
  - 支持向量机二分类主程序；