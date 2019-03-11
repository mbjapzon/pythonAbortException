#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrentRun>
#include <QTimer>
#include "PythonRunner.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(PythonRunner * runner, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    PythonRunner * m_runner;
    QMutex* m_pControlMutex;
    QTimer * m_timer;

    Q_SLOT void startTestButtonClicked(void);
    Q_SLOT void pauseButtonClicked(void);
    Q_SLOT void abortButtonClicked(void);

    //Timer timeout function
    Q_SLOT void updateCounter(void);

    void startDemo();
};

#endif // MAINWINDOW_H
