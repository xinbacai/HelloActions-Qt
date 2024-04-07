#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hlt_socket_demo.h"
#include <QTimer>

QList<int> sleepTimeList = {5000, 10000, 20000, 40000};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pHistoryPage = new CHistoryPage(this);
    connect(ui->btn_cnt,&QPushButton::clicked,this,&MainWindow::connect_server);
    connect(ui->btn_break,&QPushButton::clicked,this,&MainWindow::close_server);
    connect(ui->comboBox_time_spued,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::reset_server);
    ui->line_ip->setText("192.168.1.155"); // TODO 设定IP 121 155
    ui->line_port->setText("21110"); // 6001 7709 21110
}

MainWindow::~MainWindow()
{
    delete ui;
    delete  m_pHistoryPage;
}

void MainWindow::reset_server(int index){
    // 获取选中的值
    selectedSleepTime = sleepTimeList[index];
    if(thread){
        // 停止定时器
          thread->m_dataTimer->stop();

          // 更新定时器的间隔时间
          thread->m_dataTimer->setInterval(selectedSleepTime);

          // 重新启动定时器
          thread->m_dataTimer->start();
    }
    else{
        qDebug() << "当前子线程未创建";
    }

}

void MainWindow::connect_server(){
    //    m_pHistoryPage->_SlotCheckPoct(); // NOTE：主线程发送测试


    thread = m_pHistoryPage->create_thread_cnt();
    thread->SetPoctServer(ui->line_ip->text(), ui->line_port->text().toInt());
    thread->set_cnt_status(true);
    selectedSleepTime = sleepTimeList[ui->comboBox_time_spued->currentIndex()];
    qDebug() << "selectedSleepTime：" << selectedSleepTime;
    thread->set_sleep_time(selectedSleepTime);
    thread->start();
//    thread->m_dataTimer->start(selectedSleepTime);
    ui->btn_cnt->setEnabled(false);
}

void MainWindow::close_server(){
    ui->btn_break->setEnabled(false);
    if(thread){
        m_pHistoryPage->delete_thread_cnt();
//        delete thread;
        thread = nullptr;
        qDebug() << "thread:" << thread;
    }

    ui->btn_break->setEnabled(true);
    ui->btn_cnt->setEnabled(true);
}


