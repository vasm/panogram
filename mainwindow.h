#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <panogram_state.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_importButton_released();
    void onImageLoaded();

    void on_selectDestinationButton_released();
    void updateDestDirectory(QString& path);

    void keyPressEvent(QKeyEvent* event);

    void on_goButton_released();

private:
    Ui::MainWindow *ui;
    PanogramState _state;
};

#endif // MAINWINDOW_H
