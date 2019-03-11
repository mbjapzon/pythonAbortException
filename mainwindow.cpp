#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(PythonRunner * runner, QWidget *parent) :
    QMainWindow(parent),
    m_runner(runner),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Connect buttons
    connect(ui->startDemoBtn, &QPushButton::released, this, &MainWindow::startTestButtonClicked);
    connect(ui->pauseResumeBtn, &QPushButton::released, this, &MainWindow::pauseButtonClicked);
    connect(ui->abortBtn, &QPushButton::released, this, &MainWindow::abortButtonClicked);

    //Initialize mutex
    m_pControlMutex = new QMutex(QMutex::Recursive);

    //Initialize timer
    m_timer = new QTimer(this);
    //Connect the timer to a timeout function
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateCounter()));

}

MainWindow::~MainWindow()
{
    //If the mutex is not destroyed...destroy it.
    if (m_pControlMutex)
    {
        delete m_pControlMutex;
        m_pControlMutex = nullptr;
    }

    delete ui;
}

void MainWindow::startTestButtonClicked()
{
    //Disable the start button
    ui->startDemoBtn->setEnabled(false);
    //Since the "test" is nonblocking we create a thread as to not block the UI
    QtConcurrent::run(this, &MainWindow::startDemo);
    //Start button has been clicked... start the timer
    m_timer->start(1000);
}

void MainWindow::pauseButtonClicked()
{
    //The pause button has been clicked...
    QMutexLocker locker(m_pControlMutex);

    //If we should pause...
    if (ui->pauseResumeBtn->isChecked())
    {
        //Set the button text to "Resume", inform the python runner to pause, and stop the timer
        ui->pauseResumeBtn->setText("Resume");
        m_runner->pause();
        m_timer->stop();
    }
    //If we should resume...
    if (!ui->pauseResumeBtn->isChecked())
    {
        //Set the button text to "Pause", inform the python runner to resume, and start the timer
        ui->pauseResumeBtn->setText("Pause");
        m_runner->resume();
        m_timer->start(1000);
    }
}

void MainWindow::abortButtonClicked()
{
    //The abort button is clicked...
    QMutexLocker locker(m_pControlMutex);

    //Stop the timer
    m_timer->stop();

    //Inform the python runner to abort
    m_runner->abort();

    //Enable the Start button
    ui->startDemoBtn->setEnabled(true);
}

void MainWindow::updateCounter()
{
    //When the timer is running we will call the "getCounter" function and set the counter label
    int counter = m_runner->getCounter();
    ui->counterLbl->setText(QString::number(counter));
}

void MainWindow::startDemo()
{
    m_runner->runLoop();
}
