#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QImageReader>
#include <QBuffer>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 连接按钮和槽函数
    connect(ui->btnOpen, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(ui->btnCompress, &QPushButton::clicked, this, &MainWindow::compressImage);
    connect(ui->sliderBrightness, &QSlider::valueChanged, this, &MainWindow::adjustImage);
    connect(ui->sliderContrast, &QSlider::valueChanged, this, &MainWindow::adjustImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, "打开图片", "", "Images (*.jpg *.jpeg *.png)");
    if (!filePath.isEmpty())
    {
        currentImage.load(filePath);
        sourceImage.load(filePath);
        ui->labelImage->setPixmap(QPixmap::fromImage(currentImage).scaled(ui->labelImage->size(), Qt::KeepAspectRatio));
    }
}

void MainWindow::saveImage()
{
    QString savePath = QFileDialog::getSaveFileName(this, "保存图片", "", "JPEG (*.jpg);;PNG (*.png)");
    if (!savePath.isEmpty())
    {
        currentImage.save(savePath);
    }
}

void MainWindow::compressImage()
{
    if (sourceImage.isNull())
    {
        QMessageBox::warning(this, "警告", "请先加载图片！");
        return;
    }

    // 获取压缩质量（QSlider）
    int quality = ui->sliderQuality->value();

    QByteArray buffer;
    QBuffer qBuffer(&buffer);
    qBuffer.open(QIODevice::WriteOnly);

    // 使用 QImageWriter 进行压缩
    QImageWriter writer(&qBuffer, "JPEG");
    writer.setQuality(quality);
    writer.write(sourceImage);

    // 更新图片
    sourceImage.loadFromData(buffer);
    ui->labelImage->setPixmap(QPixmap::fromImage(currentImage).scaled(ui->labelImage->size(), Qt::KeepAspectRatio));
}

void MainWindow::adjustImage()
{
    if (sourceImage.isNull())
        return;

    QImage adjustedImage = sourceImage;
    int brightness = ui->sliderBrightness->value();
    int contrast = ui->sliderContrast->value();

    for (int y = 0; y < adjustedImage.height(); ++y)
    {
        for (int x = 0; x < adjustedImage.width(); ++x)
        {
            QColor color = adjustedImage.pixelColor(x, y);

            // 对比度调整公式：(value - 128) * contrast + 128
            int r = qBound(0, (color.red() - 128) * contrast / 100 + 128 + brightness, 255);
            int g = qBound(0, (color.green() - 128) * contrast / 100 + 128 + brightness, 255);
            int b = qBound(0, (color.blue() - 128) * contrast / 100 + 128 + brightness, 255);

            adjustedImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    currentImage = adjustedImage;
    ui->labelImage->setPixmap(QPixmap::fromImage(currentImage).scaled(ui->labelImage->size(), Qt::KeepAspectRatio));
}
