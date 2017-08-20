#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_importButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Image files (*.png *.jpg *.jpeg *.bmp)"));

    if (fileName.isEmpty()) // probably "cancel" was pressed
        return;

    if (_state.loadImage(fileName)) {
        // set default output directory the same where input image is
        QString filePath = QFileInfo(fileName).absolutePath();
        updateDestDirectory(filePath);
        this->onImageLoaded();
    }
}


void MainWindow::onImageLoaded()
{
    ui->goButton->setEnabled(true);
}


void MainWindow::on_selectDestinationButton_released()
{
    QString startPath;
    if (!ui->destinationTextfield->text().isEmpty()) {
        startPath = ui->destinationTextfield->text();
    } else {
        startPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    }

    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Destination Directory"),
                                                    startPath,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    // here we suppose that dirName is correct and won't check it
    if (!dir.isEmpty())
        updateDestDirectory(dir);
}


void MainWindow::updateDestDirectory(QString &path)
{
    _state.setDestinationDirectory(path);
    ui->destinationTextfield->setText(path);
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::on_goButton_released()
{
    if (_state.process())
        qDebug() << "Yay!";
}
