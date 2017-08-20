#include "panogram_state.h"

#include <QImageReader>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QDebug>


PanogramState::PanogramState()
{

}


bool PanogramState::loadImage(const QString &filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    _imageFilename = QFileInfo(filename).baseName();
    _image = reader.read();

    if (_image.isNull()) {
        QMessageBox::information(nullptr, QApplication::applicationDisplayName(),
                                 QApplication::instance()->tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }

    if (_image.width() < 2 * _image.height()) {
        QMessageBox::information(nullptr, QApplication::applicationDisplayName(),
                                 QApplication::instance()->tr("Looks like '%1' is not a panoramic: %2x%3. We cannot cut it into squares.")
                                 .arg(QFileInfo(filename).fileName()).arg(_image.width()).arg(_image.height()));
        _image = QImage();
        return false;
    }

    qDebug() << "Loaded image: " << _image.size();


    return true;
}

void PanogramState::setDestinationDirectory(QString &destDir)
{
    _destDir = destDir;
}

bool PanogramState::process()
{
    if (_image.isNull()) {
        QMessageBox::information(nullptr, QApplication::applicationDisplayName(),
                                 QApplication::instance()->tr("Cannot proceed: source image is not loaded."));
        return false;
    }

    if (_destDir.isEmpty()) {
        QMessageBox::information(nullptr, QApplication::applicationDisplayName(),
                                 QApplication::instance()->tr("Cannot proceed: destination directory is not set."));
        return false;
    }

    int w = _image.height();
    QImage outputRect(w, w, _image.format());

    for (int i = 0; w * (i + 1) < _image.width(); ++i) {
        // copy pixels with offsets to a square image:
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < w; ++y) {
                outputRect.setPixelColor(x, y, _image.pixelColor(x + (i * w), y));
            }
        }

        QString outFilename = QDir::toNativeSeparators(
                    QString("%1/%2_pano_%3.jpg").arg(_destDir).arg(_imageFilename).arg(i));
        outputRect.save(outFilename);
    }

    return true;
}
