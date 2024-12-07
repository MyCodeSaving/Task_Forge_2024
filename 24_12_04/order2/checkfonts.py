import matplotlib.font_manager as fm

# 打印可用的字体列表
font_paths = fm.findSystemFonts(fontpaths=None, fontext='ttf')
for path in font_paths:
    print(path)