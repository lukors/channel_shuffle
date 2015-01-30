#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QProcess"
#include "QMouseEvent"
#include "QDrag"
#include "QMimeData"
#include "QDir"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateOutput();
    void openAboutWindow();
    void updateFileType();
    void hotkeyRed();
    void hotkeyGreen();
    void hotkeyBlue();
    void hotkeyAlpha();

private:
    Ui::MainWindow *ui;
    QString *path;
    About *aboutWindow;
    QAction *actionRed;
    QAction *actionGreen;
    QAction *actionBlue;
    QAction *actionAlpha;
};

#endif // MAINWINDOW_H
