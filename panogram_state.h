#ifndef PANOGRAM_STATE_H
#define PANOGRAM_STATE_H

#include <QString>
#include <QImage>

class PanogramState
{
public:
    PanogramState();

    bool loadImage(const QString& filename);
    void setDestinationDirectory(QString& destDir);
    bool process();

protected:
    QImage _image;
    QString _imageFilename;
    QString _destDir;
};

#endif // PANOGRAM_STATE_H
