#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class sendThread;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class CHistoryPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CHistoryPage *m_pHistoryPage;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connect_server();
    void reset_server(int index);
    void close_server();

private:
    Ui::MainWindow *ui;
    sendThread* thread=nullptr;
    int selectedSleepTime;
};
#endif // MAINWINDOW_H
