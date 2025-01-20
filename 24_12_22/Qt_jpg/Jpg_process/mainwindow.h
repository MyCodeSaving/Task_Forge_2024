#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();  // 打开图片
    void saveImage();  // 保存图片
    void compressImage(); // 压缩图片
    void adjustImage();  // 调整图片属性（亮度/对比度等）

private:
    Ui::MainWindow *ui;
    QImage currentImage;  // 当前加载的图片
    QImage sourceImage;
};
#endif // MAINWINDOW_H
