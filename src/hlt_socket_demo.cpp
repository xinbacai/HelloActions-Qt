#include "hlt_socket_demo.h"

#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDesktopServices>
//#include "CommonDataWidget/CPrintPreviewWidget.h"
//#include "PublicFunction.h"
#include <sstream> // 添加头文件包含
#include <QNetworkInterface>

#include "include/hl7Interface/IMSH.h"
#include "include/hl7Interface/IHL7Message.h"
#include "include/hl7Interface/IHL7Segment.h"
#include "include/hl7Interface/helper.h"
#include "include/hl7Interface/base.h"
#include "include/hl7Interface/HL7Parser.h"
#include "QThread"
// NOTE:增加的
//#include "include/hl7Interface/IORU_R01.h"

//static int sg_iLisMessageCtrlID = 0;
//static int sg_iPidIDIndex = 0;

CHistoryPage::CHistoryPage(QWidget *parent) : QWidget(parent)
{
//    m_strDatabaseName = "\\drug.db";
//    m_iDatabaseColumnCount = 125;// 当前列数，包括ID
//    m_iResultIndexCount = 6;
//    m_iMaxTestResult = 16;
    //
//    _InitHistoryTableWidget();
//    _InitTestDataWidget();
//    _InitButtonWidget();
//    _LoadQss();
//    _InitLayout();
//    // 初始化数据库
//    _InitDataBase();
//    // 初始化excel
//    _InitExcel();
    // 打印预览
//    m_pPrintPreviewWidget = new CPrintPreviewWidget;
    // 上传
//    m_strPisServer = "192.168.8.60";
//    m_iPisServerPort = 8004;

//    m_strPoctServer = "192.168.1.104"; // TODO 设定IP 121 155
//    m_iPoctServerPort = 7709; //7711
//    m_pTcpSocket1 = new QTcpSocket(this);
//    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &CHistoryPage::_SlotPoctReadMesg);
//    connect(m_pTcpSocket1, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(_SlotPoctConnectError(QAbstractSocket::SocketError)));
//    connect(m_pTcpSocket, &QTcpSocket::disconnected, this, &CHistoryPage::_SlotPoctDisConnect);
//    m_pTcpSocket->abort();
//    m_pTcpSocket->connectToHost(m_strPoctServer, m_iPoctServerPort);

//    m_bPisHaveConnect = false;
//    m_bPoctHaveConnect = false;

//    qDebug() <<"poct conn" << m_bPoctHaveConnect;
}

CHistoryPage::~CHistoryPage()
{
//    _FreeExcel();// 是否excel
//    if(m_pPrintPreviewWidget != NULL)
//    {
//        delete m_pPrintPreviewWidget;
//        m_pPrintPreviewWidget = NULL;
//    }
}

sendThread* CHistoryPage::create_thread_cnt(){
    if(!sendthread){
        sendthread = new sendThread(this);
    }
    return sendthread;
}

void CHistoryPage::delete_thread_cnt() {
    if (sendthread) {
        if(sendthread->isRunning()) {
            sendthread->set_cnt_status(false);
            sendthread->m_dataTimer->stop(); // 停止定时器
            sendthread->m_heartbeatTimer->stop();
            sendthread->quit();
            sendthread->wait();
        }
        delete sendthread;
        sendthread = nullptr;
    }
}

void CHistoryPage::start_thread(){
    if(sendthread){
        sendthread->start();
    }
}

void CHistoryPage::stop_thread(){
    if(sendthread){
        sendthread->quit();
    }
}



sendThread::sendThread(QObject *parent) : QThread(parent)
{
//    if(!m_TcpSocket){
//        m_TcpSocket = new QTcpSocket(this);
//    }
    cnt_status = true;
    m_dataToSend = "MSH|^~\\&|system1|W|system2|UHN|200105231927||ADT^A01^ADT_A01|22139243|P|2.4\t"
                             "EVN|A01|200105231927|\t"
                             "PID||9999999999^^|2216506^||Duck^Donald^^^MR.^MR.||19720227|M|||123 Foo ST.^^TORONTO^ON^M6G 3E6^CA^H^~123 Foo ST.^^TORONTO^ON^M6G 3E6^CA^M^|1811|( 416 )111 - 1111||E^ENGLISH|S|PATIENT DID NOT INDICATE|211004554^||||||||||||\t"
                             "PV1|||ZFAST TRACK^WAITING^13|E^EMERGENCY||369^6^13^U EM EMERGENCY DEPARTMENT^ZFAST TRACK WAITING^FT WAIT 13^FTWAIT13^FT WAITING^FTWAIT13|^MOUSE^MICKEY^M^^DR.^MD|||SUR||||||||I|211004554^||||||||||||||||||||W|||||200105231927|||||\t"
                             "PV2||F|^R / O APPENDICIAL ABSCESS|||||||||||||||||||||||||\t"
                             "IN1|1||001001|OHIP||||||||||||^^^^^|||^^^^^^M^|||||||||||||||||||||||||^^^^^^M^|||||\t"
                             "ACC|";
//    connect(m_TcpSocket, &QTcpSocket::readyRead, this, &QThread::_SlotPoctReadMesg);
//    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(_SlotPoctConnectError(QAbstractSocket::SocketError)));
//    connect(m_TcpSocket, &QTcpSocket::disconnected, this, &QThread::_SlotPoctDisConnect);
}

sendThread::~sendThread()
{
    // 关闭并销毁 socket 对象
    if (m_TcpSocket) {
        if (m_TcpSocket->isOpen()) {
            qDebug() << "关闭tcp socket";
            m_TcpSocket->close();
        }
        m_TcpSocket->deleteLater(); // 在正确的线程中销毁对象
        m_TcpSocket = nullptr;
    }
}


void sendThread::SetPoctServer(QString strPoctServer, int iPort)
{
    server_ip = strPoctServer;
    server_port = iPort;

}

void sendThread::set_cnt_status(bool flag){
    cnt_status = flag;
}

void sendThread::set_sleep_time(int time){
    sleep_time = time;
}

void sendThread::stop(){
    if(!m_TcpSocket){
        m_TcpSocket->abort();
        delete m_TcpSocket;
        m_TcpSocket = nullptr;
    }
    quit();
    wait();
}

void sendThread::sendHeartbeat()
{
    if (!m_TcpSocket || m_TcpSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket not connected, cannot send heartbeat";
        return;
    }
    if (!cnt_status) {
        qDebug() << "停止定时器";
        return;
    }
}

void sendThread::sendData()
{
    if (!m_TcpSocket || m_TcpSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket not connected, cannot send data";
        return;
    }
    if (!cnt_status) {
        qDebug() << "停止定时器";
        return;
    }
    // TODO 增加结束线程条件
    qDebug() << "Sending data";
    m_TcpSocket->write(m_dataToSend);
    if (m_TcpSocket->waitForBytesWritten()) {
        qDebug() << "Data sent successfully";
    } else {
        qDebug() << "Error sending data:" << m_TcpSocket->errorString();
    }
}

void sendThread::onConnected()
{
    // 创建发送心跳的定时器
    m_heartbeatTimer = new QTimer(this);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &sendThread::sendHeartbeat);
    // TODO 外接按钮发送
//    m_heartbeatTimer->start(7000); // 每隔7秒发送一次心跳


    // 创建发送数据的定时器
    m_dataTimer = new QTimer(this);
    connect(m_dataTimer, &QTimer::timeout, this, &sendThread::sendData);
    m_dataTimer->start(sleep_time); // 每隔10秒发送一次数据
}

void sendThread::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error1:" << error;

    qDebug() << "Socket error2:" << m_TcpSocket->errorString();
}

void sendThread::run()
{
    if (!m_TcpSocket) {
        qDebug() << "Creating new socket";
        m_TcpSocket = new QTcpSocket();
        connect(m_TcpSocket, &QTcpSocket::connected, this, &sendThread::onConnected);
    }

    if (m_TcpSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Socket already connected";
        return;
    }

    qDebug() << "server_ip:" << server_ip;
    qDebug() << "server_port:" << server_port;

    m_TcpSocket->abort();
    m_TcpSocket->connectToHost(server_ip, server_port);

    if (!m_TcpSocket->waitForConnected()) {
        qDebug() << "Error: " << m_TcpSocket->errorString();
    }
}


// NOTE: 被注释的代码







/**
  * @brief 条件数据查找
  * @param
  * @return
  */
//void CHistoryPage::_SlotCheckQuery()
//{
//    // 查询数据库
//    // 时间范围
////    QDate qBeginDate = m_pBeginDataWidget->GetDate();// 当前时间结果
////    QDate qEndDate = m_pEndDataWidget->GetDate();
////    if(qBeginDate > qEndDate)
////    {
////        QMessageBox::about(this, tr("Warning"), tr("Date error"));
////        return;// 时间错误
////    }
////    QString strSelect = QString("SELECT * FROM drugdata WHERE ");
////    qDebug() <<"begin:" << qBeginDate <<"end time " <<qEndDate;

////    qEndDate = qEndDate.addDays(1);
////    strSelect += "TestTime > datetime('";
////    strSelect += qBeginDate.toString("yyyy-MM-dd") + "')";
////    // 结束时间
////    strSelect += " AND TestTime < datetime('";
////    strSelect += qEndDate.toString("yyyy-MM-dd") + "')";


////    if(m_pDonorIDWidget->GetLineText() != "")
////    {
////        strSelect += QString(" AND DonorID = '") +
////                m_pDonorIDWidget->GetLineText() + QString("'");
////    }
////    if(m_pProductDefinitionWidget->GetCurrentSelectText() != "")
////    {
////        strSelect += QString(" AND ProductDefinition = '") +
////                m_pProductDefinitionWidget->GetCurrentSelectText() + QString("'");
////    }
////    if(m_pProductLotWidget->GetLineText() != "")
////    {
////        strSelect += QString(" AND ProductLot = '") +
////                m_pProductLotWidget->GetLineText() + QString("'");
////    }

////    qDebug() << "query " << strSelect;

////    // 查找开始
////    m_pHistoryDataTableWidget->setRowCount(0);
////    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
////    while(qSqlQuery.next())
////    {
////        QStringList strLineDataList;
////        // id
////        strLineDataList.push_back(qSqlQuery.value(ID_INDEX).toString());
////        // Name
////        strLineDataList.push_back(qSqlQuery.value(DONOR_FIREST_NAME).toString() + " "
////                                  + qSqlQuery.value(DONOR_LASE_NAME).toString());
////        // TestTime
////        strLineDataList.push_back(qSqlQuery.value(TEST_TIME).toString());
////        // DonorID
////        strLineDataList.push_back(qSqlQuery.value(DONOR_ID).toString());
////        // Product Lot
////        strLineDataList.push_back(qSqlQuery.value(PRODUCT_LOT).toString());
////        // Product Difinition
////        strLineDataList.push_back(qSqlQuery.value(PRODUCT_DEFINITION).toString());
////        // 数据
////        qDebug() << "list " << strLineDataList;
////        m_strTableLineDataList.push_back(strLineDataList);
////        // 表格
////        InsertOneLine(m_pHistoryDataTableWidget, strLineDataList);
////    }
////    // 显示到控件
////    m_pHistoryDataTableWidget->update();
//}

//void CHistoryPage::_SlotCheckSelectAll()
//{
//    m_pHistoryDataTableWidget->selectAll();
//}

//void CHistoryPage::_SlotCheckDeselectAll()
//{
//    m_pHistoryDataTableWidget->setCurrentItem(NULL, QItemSelectionModel::Clear);
//}
///**
//  * @brief 删除选中
//  * @param
//  * @return
//  */
//void CHistoryPage::_SlotCheckDelete()
//{
//    QSet<int> iCurrentSelectSet;
//    _GetCurrentSelectRows(iCurrentSelectSet);
//    int iCurrentSelectSetCount = iCurrentSelectSet.count();
//    if(iCurrentSelectSetCount <= 0)
//    {
//        QMessageBox::information(NULL, tr("Tip"), tr("Please select the item first!"),
//                                 QMessageBox::Ok , QMessageBox::Ok);
//        return;
//    }
//    // 删除提示
//    int iRet = QMessageBox::question(NULL, tr("Tip"), tr("Are you sure to delete this?"),
//                                     QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
//    if(iRet == QMessageBox::No)
//    {
//        return;
//    }
//    // 删除
//    QSetIterator<int> qIter(iCurrentSelectSet);
//    while (qIter.hasNext())
//    {
//        int iRow = qIter.next();
//        qDebug() << "delete row " << iRow;
//        _DeleteOneRow(iRow);
//    }
//}

//void CHistoryPage::_SlotCheckExport()
//{
//    _NewExcel();
//    QSet<int> iCurrentSelectRowSet;
//    _GetCurrentSelectRows(iCurrentSelectRowSet);
//    if(iCurrentSelectRowSet.count() < 1)
//    {
//        QMessageBox::critical(NULL, "Error", "Please select the item first!", QMessageBox::Ok, QMessageBox::Ok);
//        return;
//    }
//    //遍历
//    QSetIterator<int> qIter(iCurrentSelectRowSet);
//    int iExcelRowIndex = 2;// 递增行,execl单元格为2行2列开始
////    QAxObject *pColumnAxObject = NULL;
////    while (qIter.hasNext())
////    {
////        int iRow = qIter.next();
////        // 获取ID
////        QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iRow, 0);
////        if(pItem == NULL)
////        {// 无选择行
////            return;
////        }
////        QString strID = pItem->text();
////        bool bOk = false;
////        int iCurrentID = strID.toInt(&bOk, 10);
////        if(bOk && iCurrentID >= 0)// 有效ID判断
////        {
////            QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
////            strSelect += strID;
////            qDebug() << "slel " << strSelect;
////            QSqlQuery qSqlQuery(strSelect);// 数据库中存放103列(id)
////            if(qSqlQuery.next())
////            {
////                for(int i = 1; i < m_iDatabaseColumnCount; ++i)// i=0为ID，不导出
////                {
////                    _SetExcelCellValue(iExcelRowIndex, i + 1, qSqlQuery.value(i).toString());// execl单元格为2行2列开始
////                }
////                // 列号
////                pColumnAxObject = m_pSheet->querySubObject("Range(QVariant, QVariant)",
////                                                           "A" + QString::number(iExcelRowIndex));//获取单元格
////                pColumnAxObject->dynamicCall("SetValue(const QVariant&)", QVariant(QString::number(iExcelRowIndex-1)));//设置单元格的值
////                ++iExcelRowIndex;
////                qDebug() <<"excel one row sucess " << iExcelRowIndex;
////            }

////        }
////    }
////    // 保存
////    QString strFile = QFileDialog::getSaveFileName(this, tr("Save File"),
////                                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
////                                               "Excel (*.xls *.xlsx)");
////    if(!strFile.isEmpty())
////    {
////        _SaveExcel(strFile);
////        QMessageBox::information(NULL, "Tip", "Export to Excel successful!", QMessageBox::Ok, QMessageBox::Ok);
////        qDebug() <<"save excel sucess";
////    }
//}

//void CHistoryPage::_SlotCheckPrint()
//{
////    // 遍历选择行，逐行打印
////    QSet<int> iCurrentSelectRowSet;
////    _GetCurrentSelectRows(iCurrentSelectRowSet);
////    qDebug() << "select row count_" << iCurrentSelectRowSet.count();
////    if(iCurrentSelectRowSet.count() < 1)
////    {
////        QMessageBox::critical(NULL, "Error", "Please select the item first!", QMessageBox::Ok, QMessageBox::Ok);
////        return;
////    }
////    // 打印HTML数据流
////    QString strPrintHtml = "";
////    bool bFirstAddHtml = false;
////    // 资源文件
//////    QString strTCubeHtmlSrc = strTCubeHtml;
////    QString strTCupHtml = "";
//////    QString strTCupHtmlSrc = strTCupHtml;
////    // 杯型
////    QString strCupType = "";
////    // 遍历
////    QSetIterator<int> qIter(iCurrentSelectRowSet);
////    QTableWidgetItem *pItem = NULL;
////    bool bOk = false;
////    QString strID = "";
////    int iCurrentID = 0;
////    while (qIter.hasNext())
////    {
////        int iRow = qIter.next();
////        // 获取杯型
////        pItem = m_pHistoryDataTableWidget->item(iRow, 5);
////        if(pItem != NULL)
////        {//
////            strCupType = pItem->text();
////            qDebug() << "cup type" << strCupType;
////            if(strCupType.isEmpty())// 此处杯型判定，根据杯型选择不同的打印模板，目前只简单判定是否为空，后面模板详细判定
////            {
////                QMessageBox::critical(NULL, "Error", "Critical data lost!", QMessageBox::Ok, QMessageBox::Ok);
////                return;
////            }
////        }
////        // 资源文件
////        strTCupHtml = GetHtmlStream(QCoreApplication::applicationDirPath() + "/Resources/" + strCupType + ".html");
////        // 获取ID
////        pItem = m_pHistoryDataTableWidget->item(iRow, 0);
////        if(pItem == NULL)
////        {// 无选择行
////            return;
////        }
////        strID = pItem->text();
////        iCurrentID = strID.toInt(&bOk, 10);
////        if(bOk && iCurrentID >= 0)// 有效ID判断
////        {
////            QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
////            strSelect += strID;
////            qDebug() << "slel " << strSelect;
////            QSqlQuery qSqlQuery(strSelect);// 数据库中存放103列(id)
////            if(qSqlQuery.next())
////            {
////                // 替换数据
////                if(bFirstAddHtml)
////                {
////                    strPrintHtml += QString("</br> <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" "
////                                            "style=\"width:98%;height:100%;border:5px solid #008cd6;margin-top:180px;\"> "
////                                            "<tr><td></td></tr></table>");
////                }
////                if(gk_strTCubeTypeList.contains(strCupType))
////                {// 方杯
////                    _ReplaceCubeHtmlData(qSqlQuery, strTCupHtml);
////                    qDebug() << "print to cube pdf";
////                }
////                else
////                {// 圆杯
////                    _ReplaceCupHtmlData(qSqlQuery, strTCupHtml);
////                    qDebug() << "print to cup pdf";
////                }
////                strPrintHtml += strTCupHtml;
////                bFirstAddHtml = true;
////            }
////        }
////    }
////    //
////    m_pPrintPreviewWidget->SetHtml(strPrintHtml);
////    m_pPrintPreviewWidget->show();
//}
///**
//  * @brief 上传到POCT服务器
//  * @param
//  * @return
//  */
//void CHistoryPage::_SlotCheckPoct()
//{
//    qDebug() << "m_bPoctHaveConnect " << m_bPoctHaveConnect;
//    if(!m_bPoctHaveConnect)
//    {// 还没有自动连接
//        m_pTcpSocket1->abort();
////        qDebug() << "error connect";
//        m_pTcpSocket1->connectToHost(m_strPoctServer,m_iPoctServerPort);
////        qDebug() << "retrry connect";
//    }


//    _UpdateToPoctServer(_ORUR01SampleResult());// 上传数据



//    // 遍历选择行，逐行上传
////    QSet<int> iCurrentSelectRowSet;
////    _GetCurrentSelectRows(iCurrentSelectRowSet);
////    qDebug() << "select row count_" << iCurrentSelectRowSet.count();
////    if(iCurrentSelectRowSet.count() < 1)
////    {
////        QMessageBox::critical(NULL, "Error", "Please select the item first!", QMessageBox::Ok, QMessageBox::Ok);
////        return;
////    }
////    // 遍历
////    QSetIterator<int> qIter(iCurrentSelectRowSet);
////    while (qIter.hasNext())
////    {
////        int iRow = qIter.next();
////        QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iRow, 0);
////        if(pItem == NULL)
////        {// 无选择行
////            return;
////        }
////        QString strID = pItem->text();
////        bool bOk = false;
////        int iCurrentID = strID.toInt(&bOk, 10);
////        if(bOk && iCurrentID >= 0)// 有效ID判断
////        {
////            QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
////            strSelect += strID;
////            qDebug() << "slel " << strSelect;
////            QSqlQuery qSqlQuery(strSelect);// 数据库中存放103列(id)
////            if(qSqlQuery.next())
////            {
////                _UpdateToPoctServer(_ORUR01SampleResult(qSqlQuery));// 上传数据
////            }
////        }
////    }
//}
///**
//  * @brief 当前选择cell改变，只处理行改变
//  * @param
//  * @return
//  */
//void CHistoryPage::_SlotHistoryDataSelectChange(
//        int iCurrentRow, int iCurrentColumn, int iPreviousRow, int iPreviousColumn)
//{
//    qDebug()<< "cru row " << iCurrentRow << "count " <<m_pHistoryDataTableWidget->rowCount();
//    qDebug()<< "cru iCurrentColumn " << iCurrentColumn << iPreviousColumn;
//    if(iCurrentRow == iPreviousRow || m_pHistoryDataTableWidget->rowCount() == 0
//            || iCurrentRow >= m_pHistoryDataTableWidget->rowCount()
//            || iCurrentRow < 0)
//    {
//        return;// 行未更改，不做处理
//    }
//    // 清空控件
//    m_pTestDataTextEdit->setText("");
//    m_pCurrentTestDataTableWidget->setRowCount(0);
//    // 获取ID
//    QTableWidgetItem *pItem = m_pHistoryDataTableWidget->item(iCurrentRow, 0);
//    if(pItem == NULL)
//    {// 无选择行
//        return;
//    }
//    QString strID = pItem->text();
//    bool bOk = false;
//    int iCurrentID = strID.toInt(&bOk, 10);
//    if(bOk && iCurrentID >= 0)
//    {
//        QString strSelect = QString("SELECT * FROM drugdata WHERE id = ");
//        strSelect += strID;
//        qDebug() << "slel " << strSelect;
//        QSqlQuery qSqlQuery(strSelect);// 数据库中存放列(id)
//        while(qSqlQuery.next())
//        {
//            // 清空数据
//            m_strCurrentTestInfoList.clear();
//            m_qTestDataList.clear();
//            // birth data
//            m_strCurrentTestInfoList.push_back(QString("Birth Date: ") + qSqlQuery.value(BIRTH_DATE).toString());
//            // test site
//            m_strCurrentTestInfoList.push_back(QString("Test Site: ") + qSqlQuery.value(TEST_SITE).toString());
//            // operator
//            m_strCurrentTestInfoList.push_back(QString("Operator: ") + qSqlQuery.value(OPERATOR).toString());
//            // test reason
//            QString strTestReason = "";
//            if(qSqlQuery.value(PRE_EMPLOYMENT).toString() == "true")
//            {// pro-employment
//                strTestReason += tr("Pre-Employment  ");
//            }
//            if(qSqlQuery.value(RANDOM).toString() == "true")
//            {// Random
//                strTestReason += tr("Random  ");
//            }
//            if(qSqlQuery.value(REASON_SUSPICION).toString() == "true")
//            {// Reason suspicion cause
//                strTestReason += tr("Reason suspicion cause  ");
//            }
//            if(qSqlQuery.value(POST_ACCIDENT).toString() == "true")
//            {// Post Accident
//                strTestReason += tr("Post Accident  ");
//            }
//            if(qSqlQuery.value(RETURN_TO_DUTY).toString() == "true")
//            {// Return to Duty
//                strTestReason += tr("Return to Duty  ");
//            }
//            if(qSqlQuery.value(FOLLW_UP).toString() == "true")
//            {// Follow Up
//                strTestReason += tr("Follow Up  ");
//            }
//            if(qSqlQuery.value(OTHER_REASON).toString() == "true")
//            {// Other Reason
//                strTestReason += tr("Other Reason  ");
//            }
//           // Comment
//            strTestReason += qSqlQuery.value(COMMENTS).toString();
//            m_strCurrentTestInfoList.push_back(QString("Reason for Test: ") + strTestReason);
//            // Temperature normal#
//            m_strCurrentTestInfoList.push_back(QString("Temperature normal: ")  + qSqlQuery.value(TEMPERATURE_NORMAL).toString());
//            // Expiration Date
//            m_strCurrentTestInfoList.push_back(QString("Expiration Date: ") + qSqlQuery.value(EXPIRATION_DATE).toString());
//            // Product ID
//            m_strCurrentTestInfoList.push_back(QString("Product ID:") + qSqlQuery.value(PRODUCT_ID).toString());
//            // Number of Programs
//            bool bProgramNumOk = false;
//            m_iCurrentDataProgramNumber = qSqlQuery.value(PROGRAM_NUMBER).toInt(&bProgramNumOk);
//            if(m_iCurrentDataProgramNumber > m_iMaxTestResult)
//            {
//                m_iCurrentDataProgramNumber = m_iMaxTestResult;// 最大结果数据位16个0-15
//            }
//            if(bProgramNumOk)
//            {
//                m_strCurrentTestInfoList.push_back(QString("Programs Number: ") + qSqlQuery.value(PROGRAM_NUMBER).toString());
//                // name result cutoff
//                for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
//                {
//                    QStringList strDataList;
//                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + i * m_iResultIndexCount).toString());
//                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + 1 + i * m_iResultIndexCount).toString());
//                    strDataList.push_back(qSqlQuery.value(PROGRAM_NAME_BEGIN + 2 + i * m_iResultIndexCount).toString());
//                    //
//                    m_qTestDataList.push_back(strDataList);
//                }
//            }
//            else
//            {
//                m_strCurrentTestInfoList.push_back(QString("Programs Number: No Number"));
//            }
//        }
////        else
////        {// 未查到数据
////            m_strCurrentTestInfoList.push_back(QString("No Data!"));
////        }
//    }
//    else
//    {
//        m_strCurrentTestInfoList.push_back(QString("No ID!"));
//    }


//    // 更新控件
//    int iTestInfoCount = m_strCurrentTestInfoList.count();
//    for(int i = 0; i < iTestInfoCount; ++i)
//    {
//        m_pTestDataTextEdit->insertPlainText(m_strCurrentTestInfoList.at(i) + QString("\r\n"));
//    }
//    // table
////    for(int i = 0; i < m_iCurrentDataProgramNumber; ++i)
////    {
////        InsertOneLine(m_pCurrentTestDataTableWidget, m_qTestDataList.at(i));
////    }
//    m_pTestDataTextEdit->update();
//    m_pCurrentTestDataTableWidget->update();
//}
//// poct上传返回
//void CHistoryPage::_SlotPoctReadMesg()
//{
//    QByteArray qPoctTcpReadMsg =   m_pTcpSocket1->readAll();

//    qDebug()  << "tcp read" << qPoctTcpReadMsg;
//    QMessageBox::information(NULL, "Tip", "Export to POCT Server Sucess!", QMessageBox::Ok, QMessageBox::Ok);
//}
//// 链接错误
//void CHistoryPage::_SlotPoctConnectError(QAbstractSocket::SocketError socketError)
//{
//    QMessageBox::information(NULL, "Tip", "Poct Connect Error", QMessageBox::Ok, QMessageBox::Ok);
//}
//// 断开链接，重新链接
//void CHistoryPage::_SlotPoctDisConnect()
//{
//    //QMessageBox::information(NULL, "Tip", "DisConect", QMessageBox::Ok, QMessageBox::Ok);
//    m_pTcpSocket1->abort();
////    m_pTcpSocket->connectToHost(m_strPoctServer, m_iPoctServerPort);
//}

//void CHistoryPage::ShowCurrentDateTest()
//{
//    // 查询数据库
//    // 时间范围
//    QDate qBeginDate = QDate::currentDate();// 当前时间结果
//    QDate qEndDate = qBeginDate.addDays(1);

//    QString strSelect = QString("SELECT * FROM drugdata WHERE ");
//    qDebug() <<"begin:" << qBeginDate <<"end time " <<qEndDate;

//    strSelect += "TestTime > datetime('";
//    strSelect += qBeginDate.toString("yyyy-MM-dd") + "')";
//    // 结束时间
//    strSelect += " AND TestTime < datetime('";
//    strSelect += qEndDate.toString("yyyy-MM-dd") + "')";

//    qDebug() << "slel " << strSelect;
//    m_pHistoryDataTableWidget->setRowCount(0);
//    QSqlQuery qSqlQuery(strSelect);// 数据库中存放69列(id)
//    while(qSqlQuery.next())
//    {
//        QStringList strLineDataList;
//        // id
//        strLineDataList.push_back(qSqlQuery.value(ID_INDEX).toString());
//        // Name
//        strLineDataList.push_back(qSqlQuery.value(DONOR_FIREST_NAME).toString() + " " + qSqlQuery.value(DONOR_LASE_NAME).toString());
//        // TestTime
//        strLineDataList.push_back(qSqlQuery.value(TEST_TIME).toString());
//        // DonorID
//        strLineDataList.push_back(qSqlQuery.value(DONOR_ID).toString());
//        // Product Lot
//        strLineDataList.push_back(qSqlQuery.value(PRODUCT_LOT).toString());
//        // Product Difinition
//        strLineDataList.push_back(qSqlQuery.value(PRODUCT_DEFINITION).toString());
//        // 数据
//        qDebug() << "list " << strLineDataList;
//        m_strTableLineDataList.push_back(strLineDataList);
//        // 表格
////        InsertOneLine(m_pHistoryDataTableWidget, strLineDataList);
//    }
//    // 显示到控件
//    m_pHistoryDataTableWidget->update();
//}





//bool CHistoryPage::ConnectDataBase(const QString &kstrDBName)
//{
//    QSqlDatabase qSqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
////    db.setHostName("host");
////    db.setDatabaseName("dbname");
////    db.setUserName("username");
////    db.setPassword("password");
//    qSqlDataBase.setDatabaseName(kstrDBName);
//    if (!qSqlDataBase.open())
//    {
//        QMessageBox::warning(0, QObject::tr("Database Error"),
//                              qSqlDataBase.lastError().text());
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}

//// 插入数据库
///**
//  * @brief 插入测试页测试结果至数据库
//  * @param
//  * @return
//  */
//void CHistoryPage::InsertToDatabase()
//{

//    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
//    {
//        QString strInsert =
//                "INSERT INTO drugdata (DonorFirstName, DonorLastName, TestTime, BirthDate, DonorID, TestSite, Operator, "
//                 "PreEmployment, Random, ReasonSuspicionCause, PostAccident, ReturnToDuty, FollowUp, OtherReason, Comments, "
//                 "TemperatureNormal, Oxidant, SpecificGravity, PH, Nitrite, Creatinine, "
//                 "Email, ProductDefinition, ExpirationDate, ProductLot, ProductID, ProgramsNumber";
//        for(int i = 0; i < 16; ++i)
//        {
//            strInsert += QString(", ") + QString("ProgramName") + QString::number(i);
//            strInsert += QString(", ") + QString("Result") + QString::number(i);
//            strInsert += QString(", ") + QString("Cutoff") + QString::number(i);
//            strInsert += QString(", ") + QString("T") + QString::number(i);
//            strInsert += QString(", ") + QString("C") + QString::number(i);
//            strInsert += QString(", ") + QString("ControlLine") + QString::number(i);
//        }
//        strInsert += QString(", ") + QString("PrintImagePath");
//        //
//        strInsert += QString(") VALUES (?");
//        for(int i = 0; i < m_iDatabaseColumnCount-2; ++i)// 共计m_iDatabaseColumnCount列,不包括ID，循环少前一个，共计2个
//        {//
//            strInsert += QString(", ?");
//        }
//        strInsert += QString(")");

//        QSqlQuery qSqlQuery;
//        qSqlQuery.prepare(strInsert);

////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorFirstName.toLocal8Bit());
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorLastName.toLocal8Bit());
////        QString strTestDate = m_sDetectorPageUserDataStruct.qTestDateTime.date().toString("yyyy-MM-dd")
////                + m_sDetectorPageUserDataStruct.qTestDateTime.time().toString("-hh-mm-ss");
////        qSqlQuery.addBindValue(strTestDate);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.qBirthDate);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strDonorID);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strTestSite.toLocal8Bit());
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strOperator);// 获得用户身份
////        //
////        QString strFlag = "false";
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bPreEmployment ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bRandom ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bReasonableSuspicionCause ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bPostAccident ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bReturnToDuty ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bFollowUp ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bOtherReason ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        // commets
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strOtherReasonComments);
////        //
////        strFlag = m_sDetectorPageUserDataStruct.bTemperatureNormal ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        // PIS
////        strFlag = m_sDetectorPageUserDataStruct.bOxidant ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        strFlag = m_sDetectorPageUserDataStruct.bSpecificGravity ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        strFlag = m_sDetectorPageUserDataStruct.bPH ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        strFlag = m_sDetectorPageUserDataStruct.bNitrite ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        strFlag = m_sDetectorPageUserDataStruct.bCreatinine ? "true" : "false";
////        qSqlQuery.addBindValue(strFlag);
////        // email
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strEmail.toLocal8Bit());
////        // product details
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductDefinition);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strExpriationDate);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductLot);
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.strProductID);
////        // program and picture
////        qSqlQuery.addBindValue(m_sDetectorPageUserDataStruct.iProgramsNumber);
////        // 插入结果数据
////        int iTestResultDataCount = m_pTestResultDataList.count();
////        for(int i = 0; i < iTestResultDataCount; ++i)
////        {
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strProgramName.toLocal8Bit());
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strResult.toLocal8Bit());
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCutoffValue);
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iTValue);
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCValue);
////            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strControlLine.toLocal8Bit());
////        }
////        for(int i = iTestResultDataCount; i < 16; ++i)
////        {
////            qSqlQuery.addBindValue("");
////            qSqlQuery.addBindValue("");
////            qSqlQuery.addBindValue(0);
////            qSqlQuery.addBindValue(0);
////            qSqlQuery.addBindValue(0);
////            qSqlQuery.addBindValue("");
////        }
////        qSqlQuery.addBindValue(m_strTestPrintImagePath.toLocal8Bit());
////        if (!qSqlQuery.exec())
////        {
////            qDebug() << qSqlQuery.lastError();
////            QMessageBox::warning(0, QObject::tr("Data base error!"),
////                                  qSqlQuery.lastError().text());
////        }
////        qSqlQuery.finish();
////        // 测试查询
////        qSqlQuery.exec("SELECT id, DonorFirstName FROM drugdata");
////        while (qSqlQuery.next()) {
////            int name = qSqlQuery.value(0).toInt();
////            QString age = qSqlQuery.value(1).toString();
////            qDebug() << name << ": " << age;
////        }
//    }
////    else
////    {
////        qDebug() << "打开数据库失败";
//    //    }
//}

//void CHistoryPage::SetPisServer(QString strPisServer, int iPort)
//{
//    m_strPisServer = strPisServer;
//    m_iPisServerPort = iPort;
//}

//void CHistoryPage::SetPoctServer(QString strPoctServer, int iPort)
//{
//    m_strPoctServer = strPoctServer;
//    m_iPoctServerPort = iPort;
//}

//void CHistoryPage::SetUserName(QString strUserName)
//{
//    m_strUserName = strUserName;
////    m_pPrintPreviewWidget->SetUserName(strUserName);
//}

//void CHistoryPage::AutoConnectPisServer(QString strServer, int iPort, bool bAuto)
//{
//    qDebug() << "history auto " << strServer << iPort;
//    m_bPisHaveConnect = bAuto;
//    m_strPisServer = strServer;
//    m_iPisServerPort = iPort;
//    if(m_bPisHaveConnect)
//    {
//        m_pTcpSocket1->abort();
//        m_pTcpSocket1->connectToHost(strServer, iPort);
//    }
//    else
//    {

//    }
//}

//void CHistoryPage::AutoConnectPoctServer(QString strServer, int iPort, bool bAuto)
//{
//    m_bPoctHaveConnect = bAuto;
//    m_strPoctServer = strServer;
//    m_iPoctServerPort = iPort;
//    qDebug() << "history auto " << strServer << iPort << m_bPoctHaveConnect;
//    if(m_bPoctHaveConnect)
//    {
//        m_pTcpSocket1->abort();
//        m_pTcpSocket1->connectToHost(m_strPoctServer, m_iPoctServerPort);
//    }
//    else
//    {
//        // 断开链接
//    }

//}

//QString CHistoryPage::GetHostMacAddress()
//{
//    QList<QNetworkInterface> qNetworkInterfaceList = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
//    int iNetworkListCount = qNetworkInterfaceList.count();
//    QString strMacAddr = "";
//    for(int i = 0; i < iNetworkListCount; i ++)
//    {
//        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
//        if(qNetworkInterfaceList[i].flags().testFlag(QNetworkInterface::IsUp)
//                && qNetworkInterfaceList[i].flags().testFlag(QNetworkInterface::IsRunning)
//                && !qNetworkInterfaceList[i].flags().testFlag(QNetworkInterface::IsLoopBack))
//        {
//            strMacAddr = qNetworkInterfaceList[i].hardwareAddress();
//            break;
//        }
//    }
//    return strMacAddr;
//}


//void CHistoryPage::LoadQss(QWidget *pWidget, QString strQssFilePath)
//{
//    if(pWidget == NULL || strQssFilePath.isNull())
//    {
//        return;
//    }
//    QFile qFile(strQssFilePath);
//    if(!qFile.open(QFile::ReadOnly))
//    {
//        return;
//    }
//    QTextStream qTextStream(&qFile);
//    QString strStylesheet = qTextStream.readAll();
//    pWidget->setStyleSheet(strStylesheet);
//    qFile.close();
//}

//void CHistoryPage::_LoadQss()
//{
//   LoadQss(this, ":/qss/HistoryPage/HistoryPage.qss");
//}



////void CHistoryPage::SetTestResultDataList(QList<TestResultData *> pTestResultDataList, QString strPrintImagePath)
////{
////    m_pTestResultDataList = pTestResultDataList;
////    m_strTestPrintImagePath = strPrintImagePath;
////    qDebug() << "get history  test size: " << m_pTestResultDataList.count() << m_strTestPrintImagePath;
////}

////void CHistoryPage::SetTestUserData(DetectorPageUserData sDetectorPageUserDataStruct)
////{
////    m_sDetectorPageUserDataStruct = sDetectorPageUserDataStruct;
////    qDebug() << "user histroyt  data: " << m_sDetectorPageUserDataStruct.strOtherReasonComments;
////}

////void CHistoryPage::SetCupType(QStringList strCupTypeList)
////{
////    m_strCupTypeList = strCupTypeList;
////    m_strCupTypeList.insert(0, "");// 允许为空
////    m_pProductDefinitionWidget->SetCupType(m_strCupTypeList);
////}
///**
//  * @brief 显示当天测试结果数据至Table控件
//  * @param
//  * @return
//  */

//// query condition group
///**
//  * @brief 创建数据库操作组合控件
//  * @param
//  * @return
//  */
////QGroupBox *CHistoryPage::_CreateQueryConditionGroup()
////{
////    QGroupBox *pGroupBox = new QGroupBox(tr("Query Condition"), this);
////    pGroupBox->setFixedHeight(146);
////    //
////    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID"), "", this);
////    m_pDonorIDWidget->SetLabelObjectName("m_pDonorIDWidget");
////    m_pProductLotWidget = new CLabelLineEditWidget(tr("Product Lot"), "", this);
////    //
////    m_pBeginDataWidget = new CLabelDateWidget(tr("Begin Date"), QDate::currentDate(), this);
////    m_pEndDataWidget = new CLabelDateWidget(tr("End Date"), QDate::currentDate(), this);
////    // 中划线
////    m_pBeginToEndLabel = new QLabel(this);
////    m_pBeginToEndLabel->setFixedSize(32, 41);
////    //画 pixmap
////    QPixmap qPixmap(32, 41);
////    qPixmap.fill(Qt::transparent);
////    QPainter qPainter(&qPixmap);
////    qPainter.setPen(QColor(159, 159, 159));
////    qPainter.drawLine(0, 35, qPixmap.width(), 35);
////    // 设置label
////    m_pBeginToEndLabel->setPixmap(qPixmap);
////    //
////    QStringList strProductDefinitionList;
////    strProductDefinitionList << tr("T Cup") << tr("T Cupa");
////    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
////    //
////    // subject
////    QHBoxLayout *pDonorLayout = new QHBoxLayout;
////    pDonorLayout->addSpacing(19);
////    pDonorLayout->addWidget(m_pDonorIDWidget);
////    pDonorLayout->addWidget(m_pProductLotWidget);
////    pDonorLayout->addWidget(m_pBeginDataWidget);
////    pDonorLayout->addWidget(m_pBeginToEndLabel);
////    pDonorLayout->addWidget(m_pEndDataWidget);
////    pDonorLayout->addSpacing(19);
////    //
////    QHBoxLayout *pDefinitionLayout = new QHBoxLayout;
////    pDefinitionLayout->addSpacing(19);
////    pDefinitionLayout->addWidget(m_pProductDefinitionWidget);
////    pDefinitionLayout->addStretch(100);
////    //
////    QVBoxLayout *pLayout = new QVBoxLayout;
////    pLayout->addLayout(pDonorLayout);
////    pLayout->addLayout(pDefinitionLayout);

////    pGroupBox->setLayout(pLayout);

////    return pGroupBox;
////}
///**
//  * @brief 初始化Table控件
//  * @param
//  * @return
//  */
//void CHistoryPage::_InitHistoryTableWidget()
//{
//    // table
//    m_pHistoryDataTableWidget = new QTableWidget(this);
//    m_pHistoryDataTableWidget->setMinimumSize(659, 350);
//    m_pHistoryDataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    m_pHistoryDataTableWidget->setFocusPolicy(Qt::NoFocus);
//    m_iTableColumnCount = 6;
//    // 设置列数量
//    m_pHistoryDataTableWidget->setColumnCount(m_iTableColumnCount);
//    m_pHistoryDataTableWidget->setColumnHidden(0, true);// 首列为ID数据，隐藏不显示
//    // 不显示行号
//    QHeaderView *pVerticalHeader = m_pHistoryDataTableWidget->verticalHeader();
//    pVerticalHeader->setHidden(true);
//    pVerticalHeader->setHighlightSections(false);
//    // 表单样式
//    QHeaderView *pHeaderView = m_pHistoryDataTableWidget->horizontalHeader();
//    //pHeaderView->setDefaultSectionSize(110);
//    pHeaderView->resizeSection(0, 10);
//    pHeaderView->resizeSection(1, 150);
//    pHeaderView->resizeSection(2, 180);
//    pHeaderView->resizeSection(3, 80);
//    pHeaderView->resizeSection(4, 120);
//    //pHeaderView->resizeSection(5, 120);
//    pHeaderView->setHighlightSections(false);
//    pHeaderView->setDisabled(true);
//    // 充满表格
//    pHeaderView->setStretchLastSection(true);
//    // 多选
//    m_pHistoryDataTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    // 每次选择一行
//    m_pHistoryDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    //m_pHistoryDataTableWidget->setStyleSheet("border-radius:5; color:#94f3dc; font: 18px ;selection-background-color: #d9f4fe;selection-color: black");
//    // 设置表头内容
//    QStringList qstrHeaderList;
//    qstrHeaderList << tr("id") << tr("Donor Name") << tr("Test Time") << tr("Donor ID")
//                          << tr("Product Lot") << tr("Product Definition");
//    //
//    m_pHistoryDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
//    // 显示格子线
//    m_pHistoryDataTableWidget->setShowGrid(true);
//     //
//     connect(m_pHistoryDataTableWidget, SIGNAL(currentCellChanged(int,int,int,int)),
//             this, SLOT(_SlotHistoryDataSelectChange(int,int,int,int)));
//}
///**
//  * @brief 初始化测试数据显示控件，每行详细数据
//  * @param
//  * @return
//  */
//void CHistoryPage::_InitTestDataWidget()
//{
//    m_pTestDataTextEdit = new QTextEdit(this);
//    m_pTestDataTextEdit->setFixedSize(300, 100);
//    //m_pTestDataTextEdit->setEnabled(false);

//    m_pCurrentTestDataTableWidget = new QTableWidget(this);
//    m_pCurrentTestDataTableWidget->setFixedWidth(300);
//    m_pCurrentTestDataTableWidget->setColumnCount(3);
//    m_pCurrentTestDataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    m_pCurrentTestDataTableWidget->setFocusPolicy(Qt::NoFocus);
//    // 每次选择一行
//    m_pCurrentTestDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    //m_pCurrentTestDataTableWidget->setRowCount(16);// 最大16个项目
//    // 不显示行号
//    QHeaderView *pVerticalHeader = m_pCurrentTestDataTableWidget->verticalHeader();
//    pVerticalHeader->setHidden(true);
//    pVerticalHeader->setHighlightSections(false);
//    QHeaderView *pHeaderView = m_pCurrentTestDataTableWidget->horizontalHeader();
//    pHeaderView->setDefaultSectionSize(90);
//    pHeaderView->setDisabled(true);
//    pHeaderView->setHighlightSections(false);
//    // 充满表格
//    pHeaderView->setStretchLastSection(true);
//    // 设置表头内容
//    QStringList qstrHeaderList;
//    qstrHeaderList << tr("Program") << tr("Result") << tr("Cutoff Value");
//    m_pCurrentTestDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
//    // 显示格子线
//    m_pCurrentTestDataTableWidget->setShowGrid(true);
//}

//void CHistoryPage::_InitButtonWidget()
//{
//    // button
//    m_pQueryButton = new QPushButton(tr("Query"), this);
//    connect(m_pQueryButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckQuery()));
//    m_pSelectAllButton = new QPushButton(tr("Selet All"), this);
//    connect(m_pSelectAllButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckSelectAll()));
//    m_pDeselectAllButton = new QPushButton(tr("Deselect All"), this);
//    connect(m_pDeselectAllButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckDeselectAll()));
//    m_pDeleteButton = new QPushButton(tr("Delete"), this);
//    connect(m_pDeleteButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckDelete()));
//    m_pExportButton = new QPushButton(tr("Export"), this);
//    connect(m_pExportButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckExport()));
//    m_pPrintButton = new QPushButton(tr("Print"), this);
//    connect(m_pPrintButton, &QPushButton::clicked, this, &CHistoryPage::_SlotCheckPrint);
//    m_pPoctButton = new QPushButton(tr("Poct"), this);
//    connect(m_pPoctButton, &QPushButton::clicked, this, &CHistoryPage::_SlotCheckPoct);

//}

//void CHistoryPage::_InitLayout()
//{
//    QVBoxLayout *pLayout = new QVBoxLayout;

//    QHBoxLayout *pDataLayout = new QHBoxLayout;
//    QVBoxLayout *pTestDataLayout = new QVBoxLayout;
//    pTestDataLayout->setMargin(0);
//    pTestDataLayout->addWidget(m_pCurrentTestDataTableWidget);
//    pTestDataLayout->addWidget(m_pTestDataTextEdit);
//    // 第一行
//    pDataLayout->addSpacing(18);
//    pDataLayout->addWidget(m_pHistoryDataTableWidget);
//    pDataLayout->addLayout(pTestDataLayout);
//    pDataLayout->addSpacing(18);
//    //
//    pLayout->addLayout(pDataLayout);
//    // group
//    QHBoxLayout *pQueryLayout = new QHBoxLayout;
//    pQueryLayout->addSpacing(18);
////    pQueryLayout->addWidget(_CreateQueryConditionGroup());
//    pQueryLayout->addSpacing(18);
//    pLayout->addLayout(pQueryLayout);
//    //
//    QHBoxLayout *pButtonLayout = new QHBoxLayout;
//    pButtonLayout->addStretch(10);
//    pButtonLayout->addWidget(m_pQueryButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pSelectAllButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pDeselectAllButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pDeleteButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pExportButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pPrintButton);
//    pButtonLayout->addSpacing(10);
//    pButtonLayout->addWidget(m_pPoctButton);
//    pButtonLayout->addStretch(10);
//    //
//    pLayout->addLayout(pButtonLayout);
//    this->setLayout(pLayout);
//}

//bool CHistoryPage::_DeleteOneRow(int iRow)
//{
//    if(iRow < 0 || iRow >= m_pHistoryDataTableWidget->rowCount())
//    {
//        return false;
//    }
//    QTableWidgetItem *pIDItem = m_pHistoryDataTableWidget->item(iRow, 0);
//    if(pIDItem == NULL)
//    {
//        return false;
//    }
//    QString strDatabaseID = pIDItem->text();
//    qDebug()<<"str DatabaseID: " << strDatabaseID;
//    // 数据库删除
//    _DeleteDatabase(strDatabaseID);
//    // 控件删除
//    m_pHistoryDataTableWidget->removeRow(iRow);
//    return true;
//}
///**
//  * @brief 获取当前选中行
//  * @param
//  * @return
//  */
//bool CHistoryPage::_GetCurrentSelectRows(QSet<int> &iSelectSet)
//{
//    QList<QTableWidgetItem* > qItemsList = m_pHistoryDataTableWidget->selectedItems();
//    int iItemCount = qItemsList.count();
//    if(iItemCount <= 0)
//    {
//        return false;
//    }
//    for(int i=0; i < iItemCount; ++i)
//    {
//        //获取选中的行
//        int iItemRow = m_pHistoryDataTableWidget->row(qItemsList.at(i));
//        iSelectSet.insert(iItemRow);
//    }
//    return  true;
//}

///**
//  * @brief 删除指定ID的数据
//  * @param
//  * @return
//  */
//bool CHistoryPage::_DeleteDatabase(QString strID)
//{
//    if(strID == "")
//    {
//        return false;
//    }
//    // 判定是否为数字
//    bool bOK = false;
//    int iID = strID.toInt(&bOK, 10);
//    if(!bOK || iID < 0)
//    {
//        return false;
//    }
//    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
//    {
//        QString strDelete = "DELETE FROM drugdata WHERE id = ";
//        strDelete += strID;
//        QSqlQuery qSqlQuery;
//        if (!qSqlQuery.exec(strDelete))
//        {
//            qDebug() << qSqlQuery.lastError();
//            QMessageBox::warning(0, QObject::tr("Delete Data base error!"),
//                                  qSqlQuery.lastError().text());
//            qSqlQuery.finish();
//            return false;
//        }
//        qSqlQuery.finish();
//    }
//    return true;
//}
//void CHistoryPage::_InitExcel()
//{
////    m_pApplication = new QAxObject();
////    bool bCon =  m_pApplication->setControl("Excel.Application");//连接Excel控件
////    qDebug() <<"conn " << bCon;
////    m_pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
////    m_pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
////    m_pWorkBooks = m_pApplication->querySubObject("Workbooks");
//    m_strCharNumberList << "A1" << "B1" << "C1" << "D1" << "E1" << "F1" << "G1" << "H1" << "I1" <<
//                         "J1" << "K1" << "L1" << "M1" << "N1" << "O1" << "P1" << "Q1" << "R1" << "S1" <<
//                         "T1" << "U1" << "V1" << "W1" << "X1" << "Y1" << "Z1";
//    for(int i = 0; i <26; ++i)
//    {
//        m_strCharNumberList << QString("A") + m_strCharNumberList.at(i);
//    }
//    for(int i = 0; i <26; ++i)
//    {
//        m_strCharNumberList << QString("B") + m_strCharNumberList.at(i);
//    }
//    for(int i = 0; i <26; ++i)
//    {
//        m_strCharNumberList << QString("C") + m_strCharNumberList.at(i);
//    }
//    for(int i = 0; i <26; ++i)
//    {
//        m_strCharNumberList << QString("D") + m_strCharNumberList.at(i);
//    }
//    // 初始化title名称
//    m_strTitleNameList << "ID" << "DonorFirstName" << "DonorLastName" << "TestTime"
//                         << "BirthDate" << "DonorID" << "TestSite" << "Operator"
//                         << "PreEmployment" << "Random" << "ReasonSuspicionCause" << "PostAccident"
//                         << "ReturnToDuty" << "FollowUp" << "OtherReason" << "Comments"
//                         << "TemperatureNormal" << "Oxidant" << "SpecificGravity" << "PH"
//                         << "Nitrite" <<"Creatinine" << "Email" << "ProductDefinition" << "ExpirationDate"
//                         << "ProductLot" << "ProductID" << "ProgramsNumber";

//    for(int i = 0; i < 16; ++i)
//    {
//        m_strTitleNameList << QString("ProgramName") + QString::number(i);
//        m_strTitleNameList << QString("Result") + QString::number(i);
//        m_strTitleNameList << QString("Cutoff") + QString::number(i);
//        m_strTitleNameList << QString("T") + QString::number(i);
//        m_strTitleNameList << QString("C") + QString::number(i);
//        m_strTitleNameList << QString("ControlLine") + QString::number(i);
//    }
//    m_strTitleNameList << "PrintImagePath";
//}

//void CHistoryPage::_NewExcel()
//{
////    m_pWorkBooks->dynamicCall("Add");
////    m_pWorkBook = m_pApplication->querySubObject("ActiveWorkBook");
////    m_pSheets = m_pWorkBook->querySubObject("Sheets");
////    m_pSheet = m_pSheets->querySubObject("Item(int)", 1);
//    // 设置标题

//    qDebug() << "now QAxObject this model,the model is windows dll  " ;//m_iDatabaseColumnCount << m_strCharNumberList.count() << m_strTitleNameList.count();

////    QAxObject *pTitleAxObject = NULL;
////    for(int i = 0; i < m_iDatabaseColumnCount; ++i)
////    {
////        pTitleAxObject = m_pSheet->querySubObject("Range(QVariant, QVariant)", m_strCharNumberList.at(i));//获取单元格
////        pTitleAxObject->dynamicCall("SetValue(const QVariant&)", QVariant(m_strTitleNameList.at(i)));//设置单元格的值
////    }
//}

//void CHistoryPage::_SetExcelCellValue(int iRow, int iColumn, const QString &kstrValue)
//{
////    QAxObject *pRange = m_pSheet->querySubObject("Cells(int,int)", iRow, iColumn);
////    pRange->dynamicCall("Value", kstrValue);
//}

//void CHistoryPage::_SaveExcel(const QString &kstrFileName)
//{
//    /*m_pWorkBook->dynamicCall("SaveAs(const QString &)",
//                               QDir::toNativeSeparators(kstrFileName))*/;
//}

//void CHistoryPage::_FreeExcel()
//{
////    if (m_pApplication != NULL)
////    {
////        m_pApplication->dynamicCall("Quit()");
////        delete m_pApplication;
////        m_pApplication = NULL;
////    }
//}

//void CHistoryPage::_ReplaceCubeHtmlData(QSqlQuery &qSqlQuery, QString &strTCubeHtml)
//{
//    QString strFindWord = "";
//    // operator id
//    strFindWord = "${OperatorID}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), qSqlQuery.value(OPERATOR).toString());
//    // test date
//    qDebug() << "string time " << qSqlQuery.value(TEST_TIME).toString();
//    QDateTime qTestDateTime = QDateTime::fromString(qSqlQuery.value(TEST_TIME).toString(), "yyyy-MM-dd-hh-mm-ss");
//    qDebug() << "date time " << qTestDateTime << "dd" << qTestDateTime.date().toString("yyyy-MM-dd");
//    strFindWord = "${TestDate}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qTestDateTime.date().toString("yyyy-MM-dd"));
//    // test time
//    strFindWord = "${TestTime}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qTestDateTime.time().toString());
//    // donor id
//    strFindWord = "${DonorID}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(DONOR_ID).toString());
//    // email address
//    strFindWord = "${EmailAddress}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(EMAIL).toString());
//    // Testing Site
//    strFindWord = "${TestingSite}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEST_SITE).toString());
//    // Specimen Type
//    strFindWord = "${UrineCheck}";
//    if(gk_strTCupTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
//    {
//        strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), "checked");
//    }
//    strFindWord = "${SalivaCheck}";
//    if(gk_strTCubeTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
//    {
//        strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), "checked");
//    }
//    // reason for test
//    strFindWord = "${PreEmploymentCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRE_EMPLOYMENT).toBool() ? "checked" : "");
//    strFindWord = "${RandomCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(RANDOM).toBool() ? "checked" : "");
//    strFindWord = "${ReasonableCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(REASON_SUSPICION).toBool() ? "checked" : "");
//    strFindWord = "${PostAccidentCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(POST_ACCIDENT).toBool() ? "checked" : "");
//    strFindWord = "${ReturnToDutyCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(RETURN_TO_DUTY).toBool() ? "checked" : "");
//    strFindWord = "${FollowUpCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(FOLLW_UP).toBool() ? "checked" : "");
//    strFindWord = "${OtherCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(OTHER_REASON).toBool() ? "checked" : "");
//    // other
//    strFindWord = "${Other}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(COMMENTS).toString());
//    // ProductID
//    strFindWord = "${ProductID}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRODUCT_ID).toString());
//    // ProductLot
//    strFindWord = "${ProductLot}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRODUCT_LOT).toString());
//    // ExpirationDate
//    strFindWord = "${ExpirationDate}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(EXPIRATION_DATE).toString());
//    // temperature in range
//    strFindWord = "${TemperatureinRangeYesCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "checked" : "");
//    strFindWord = "${TemperatureinRangeNoCheck}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "" : "checked");
//    // 测试结果
//    int iTestResultDataListCount = qSqlQuery.value(PROGRAM_NUMBER).toInt();
//    QString strResultDataHtml = "";
//    int iResultIndex = PROGRAM_NAME_BEGIN;
//    for(int i = 0; i < iTestResultDataListCount; ++i)
//    {
//        strResultDataHtml += QString(" <tr style=\"text-align:center\"> <th>&nbsp;</th><td style=\"padding: 2px 0px;\">");
//        strResultDataHtml += QString("Strip") + QString::number(i);// strip的数值,0开始
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex).toString();// drug的数值项目名称
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+2).toString();// cutoff的数值
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+5).toString();// control Line的数值
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+1).toString();// result的数值
//        strResultDataHtml += QString("</td> <th>&nbsp;</th> </tr> ");
//        iResultIndex  = iResultIndex + m_iResultIndexCount;// 6组数据
//    }
//    strFindWord = "${ResultData}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), strResultDataHtml);
//    // 图片Image
//    QString strTestPrintImagePath = qSqlQuery.value(PRINT_IMAGE_PATH).toString();
//    QString strImageByte = GetImagePngBase64(QCoreApplication::applicationDirPath()  + strTestPrintImagePath);
//    strFindWord = "${test_image_01}";
//    strTCubeHtml = strTCubeHtml.replace(strTCubeHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
//}

//QString CHistoryPage::GetImagePngBase64(QString strImagePath)
//{
//    if(strImagePath.isNull())
//    {
//        return "";
//    }
//    QImage qImage(strImagePath);
//    QByteArray qByteArray;
//    QBuffer qBuffer(&qByteArray);
//    qImage.save(&qBuffer, "PNG");
//    QString strBase64(qByteArray.toBase64());
//    return strBase64;
//}

//void CHistoryPage::_ReplaceCupHtmlData(QSqlQuery &qSqlQuery, QString &strTCupHtml)
//{
//    QString strFindWord = "";
//    // operator id
//    strFindWord = "${OperatorID}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), qSqlQuery.value(OPERATOR).toString());
//    // test date
//    QDateTime qTestDateTime = QDateTime::fromString(qSqlQuery.value(TEST_TIME).toString(), "yyyy-MM-dd-hh-mm-ss");
//    qDebug() << "string time " << qSqlQuery.value(TEST_TIME).toString();
//    qDebug() << "date time " << qTestDateTime << "dd" << qTestDateTime.date().toString("yyyy-MM-dd");

//    strFindWord = "${TestDate}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qTestDateTime.date().toString("yyyy-MM-dd"));
//    // test time
//    strFindWord = "${TestTime}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qTestDateTime.time().toString());
//    // donor id
//    strFindWord = "${DonorID}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(DONOR_ID).toString());
//    // email address
//    strFindWord = "${EmailAddress}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(EMAIL).toString());
//    // Testing Site
//    strFindWord = "${TestingSite}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEST_SITE).toString());
//    // Specimen Type
//    strFindWord = "${UrineCheck}";
//    if(gk_strTCupTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
//    {
//        strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), "checked");
//    }
//    strFindWord = "${SalivaCheck}";
//    if(gk_strTCubeTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
//    {
//        strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), "checked");
//    }
//    // reason for test
//    strFindWord = "${PreEmploymentCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRE_EMPLOYMENT).toBool() ? "checked" : "");
//    strFindWord = "${RandomCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(RANDOM).toBool() ? "checked" : "");
//    strFindWord = "${ReasonableCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(REASON_SUSPICION).toBool() ? "checked" : "");
//    strFindWord = "${PostAccidentCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(POST_ACCIDENT).toBool() ? "checked" : "");
//    strFindWord = "${ReturnToDutyCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(RETURN_TO_DUTY).toBool() ? "checked" : "");
//    strFindWord = "${FollowUpCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(FOLLW_UP).toBool() ? "checked" : "");
//    strFindWord = "${OtherCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(OTHER_REASON).toBool() ? "checked" : "");
//    // other
//    strFindWord = "${Other}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(COMMENTS).toString());
//    // ProductID
//    strFindWord = "${ProductID}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRODUCT_ID).toString());
//    // ProductLot
//    strFindWord = "${ProductLot}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PRODUCT_LOT).toString());
//    // ExpirationDate
//    strFindWord = "${ExpirationDate}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(EXPIRATION_DATE).toString());
//    // temperature in range
//    strFindWord = "${TemperatureinRangeYesCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "checked" : "");
//    strFindWord = "${TemperatureinRangeNoCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(TEMPERATURE_NORMAL).toBool() ? "" : "checked");
//    // PIS
//    strFindWord = "${OxidantCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(OXIDANT).toBool() ? "checked" : "");
//    strFindWord = "${SpecificGravityCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(SPECIFIC_GRAVITY).toBool() ? "checked" : "");
//    strFindWord = "${PHCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(PH).toBool() ? "checked" : "");
//    strFindWord = "${NitriteCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(NITRITE).toBool() ? "checked" : "");
//    strFindWord = "${CreatinineCheck}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord),
//                              strFindWord.count(), qSqlQuery.value(CREATININE).toBool() ? "checked" : "");
//    // 测试结果
//    int iTestResultDataListCount = qSqlQuery.value(PROGRAM_NUMBER).toInt();
//    QString strResultDataHtml = "";
//    int iResultIndex = PROGRAM_NAME_BEGIN;
//    for(int i = 0; i < iTestResultDataListCount; ++i)
//    {
//        strResultDataHtml += QString(" <tr style=\"text-align:center\"> <th>&nbsp;</th><td style=\"padding: 2px 0px;\">");
//        strResultDataHtml += QString("Strip") + QString::number(i);// strip的数值,0开始
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex).toString();// drug的数值
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+2).toString();// cutoff的数值
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+5).toString();// control Line的数值
//        strResultDataHtml += QString("</td> <td>");
//        strResultDataHtml += qSqlQuery.value(iResultIndex+1).toString();// result的数值
//        strResultDataHtml += QString("</td> <th>&nbsp;</th> </tr> ");
//        iResultIndex  = iResultIndex + m_iResultIndexCount;// 6组数据
//    }
//    strFindWord = "${ResultData}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), strResultDataHtml);
//    // 图片Image
//    QString strTestPrintImagePath = qSqlQuery.value(PRINT_IMAGE_PATH).toString();
//    qDebug() << "print image path " << strTestPrintImagePath;
//    QString strImageByte = GetImagePngBase64(QCoreApplication::applicationDirPath()  + strTestPrintImagePath);
//    strFindWord = "${test_image_01}";
//    strTCupHtml = strTCupHtml.replace(strTCupHtml.indexOf(strFindWord), strFindWord.count(), strImageByte);
//}



//void CHistoryPage::_UpdateToPisServer(string strUpdateData)
//{

//}

//// NOTE: 迁移到子线程中
//void CHistoryPage::_UpdateToPoctServer(string strUpdateData)
//{
//    qDebug() << __func__ << "start";
////    QByteArray qSendDataByteArray;
////    qSendDataByteArray[0]=0x0B;
////    m_pTcpSocket->write(qSendDataByteArray);
////    while(true){
////        m_pTcpSocket1->write(strUpdateData.c_str()); // NOTE: socket发送上传的数据
//////        if(!connect_status){
//////            break;
////        }
//////        sleep()
////    }

//    m_pTcpSocket1->write(strUpdateData.c_str()); // NOTE: socket发送上传的数据
////    qSendDataByteArray[0]=0x1c;
////    m_pTcpSocket->write(qSendDataByteArray);
////    qSendDataByteArray[0]=0x0d;
////    m_pTcpSocket->write(qSendDataByteArray);
//    qDebug() << __func__ << "end";

//}

//void inline  PrintMessageStr(IHL7Message* message, std::string& messageStr)
//{
//    if (message)
//    {
//        char* str = NULL;
//        message->GetMessageString(&str);
//        messageStr = str;
//        printf("%s", messageStr.c_str());
////        FreeMemory(str);//由DLL内部申请的内存记得调用FreeMemory释放
//    }
//}

//string CHistoryPage::_ORUR01SampleResult() //QSqlQuery qSqlQuery
//{
////    std::string demo =  "MSH|^~\\&|system1|W|system2|UHN|200105231927||ADT^A01^ADT_A01|22139243|P|2.4\t\
////            EVN|A01|200105231927|\t\
////            PID||9999999999^^|2216506^||Duck^Donald^^^MR.^MR.||19720227|M|||123 Foo ST.^^TORONTO^ON^M6G 3E6^CA^H^~123 Foo ST.^^TORONTO^ON^M6G 3E6^CA^M^|1811|( 416 )111 - 1111||E^ENGLISH|S|PATIENT DID NOT INDICATE|211004554^||||||||||||\t\
////            PV1|||ZFAST TRACK^WAITING^13|E^EMERGENCY||369^6^13^U EM EMERGENCY DEPARTMENT^ZFAST TRACK WAITING^FT WAIT 13^FTWAIT13^FT WAITING^FTWAIT13|^MOUSE^MICKEY^M^^DR.^MD|||SUR||||||||I|211004554^||||||||||||||||||||W|||||200105231927|||||\t\
////            PV2||F|^R / O APPENDICIAL ABSCESS|||||||||||||||||||||||||\t\
////            IN1|1||001001|OHIP||||||||||||^^^^^|||^^^^^^M^|||||||||||||||||||||||||^^^^^^M^|||||\t\
////            ACC|";

//    std::string demo = "MSH|^~\\&|MEDNOVEL_HD_A1|S06-2404A01|||20240401160852+0000||ORU^R01^ORU_R01|1|P|2.6|||AL|NE||UNICODE UTF-8\t"
//                              "SFT|Mednovel|1.0.0.0|Dialysis-A1\t"
//                              "PID|||1234567890^^^Hospital^PI||zhang^san^^^^^L|||F\t"
//                              "OBR|1|1^MEDNOVEL_HD_A1|1^MEDNOVEL_HD_A1|182777000^monitoring of patient^SCT|||20240401160852+0000\t"
//                              "OBX|1|NM|10001^MDN_HD_PATIENT_AGE^L|||^year^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|2|NM|10002^MDN_HD_PATIENT_WEIGHT^L|||^kg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|3|NM|10003^MDN_HD_PATIENT_HEIGHT^L|||^cm^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|4|ST|10004^MDN_HD_DIALYSIS_BRANDS_MODEL_SN^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|5|NM|10005^MDN_HD_DIALYSIS_STATUS_MP^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|6|NM|10006^MDN_HD_DIALYSIS_TIME_USED^L|||^s^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|7|NM|10007^MDN_HD_DIALYSIS_TIME_REMAIN^L|||^s^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|8|NM|10008^MDN_HD_DIALYSIS_TEMPERATURE_FRONT_ACTUAL^L|||^degrees^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|9|NM|10009^MDN_HD_DIALYSIS_TEMPERATURE_BACK^L|||^degrees^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|10|NM|10010^MDN_HD_DIALYSIS_CONDUCTIVITY_FRONT^L|||^mS/cm^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|11|NM|10011^MDN_HD_DIALYSIS_CONDUCTIVITY_BACK^L|||^mS/cm^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|12|NM|10012^MDN_HD_DIALYSIS_FLOWRATE_ACTUAL^L|||^ml/min^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|13|NM|10013^MDN_HD_DIALYSIS_VALUE_KTV^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|14|NM|10014^MDN_HD_DIALYSIS_VALUE_V^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|15|NM|10015^MDN_HD_DIALYSIS_PV^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|16|NM|10016^MDN_HD_DIALYSIS_PA^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|17|NM|10017^MDN_HD_DIALYSIS_PT^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|18|NM|10018^MDN_HD_DIALYSIS_RA^L|||^ml/min^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|19|NM|10019^MDN_HD_DIALYSIS_RE^L|||^ml/min^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|20|NM|10020^MDN_HD_DIALYSIS_VB^L|||^L^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|21|NM|10021^MDN_HD_DIALYSIS_TS^L|||^degrees^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|22|ST|10022^MDN_HD_DIALYSIS_UF^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|23|ST|10023^MDN_HD_DIALYSIS_UI^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|24|NM|10024^MDN_HD_DIALYSIS_FLOWRATE_SET^L|||^ml/min^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|25|ST|10025^MDN_HD_DIALYSIS_CO^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|26|NM|10026^MDN_HD_DIALYSIS_PN^L|||^mmol/l^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|27|NM|10027^MDN_HD_DIALYSIS_PB^L|||^mmol/l^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|28|NM|10028^MDN_HD_DIALYSIS_PS^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|29|ST|10029^MDN_HD_DIALYSIS_CL^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|30|ST|10030^MDN_HD_DIALYSIS_TS^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|31|ST|10031^MDN_HD_DIALYSIS_DI^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|32|ST|10032^MDN_HD_DIALYSIS_BL^L|||^ml/h^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|33|NM|10033^MDN_HD_DIALYSIS_CT^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|34|ST|10034^MDN_HD_DIALYSIS_BC^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|35|ST|10035^MDN_HD_DIALYSIS_HP^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|36|ST|10036^MDN_HD_DIALYSIS_BB^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|37|NM|10037^MDN_HD_DIALYSIS_MF^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|38|ST|10038^MDN_HD_NIBP_MODE^L|||^^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|39|NM|10039^MDN_HD_NIBP_PRESSURE_SYSTOLIC^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|40|NM|10040^MDN_HD_NIBP_PRESSURE_DIASTOLIC^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|41|NM|10041^MDN_HD_NIBP_PRESSURE_MEAN^L|||^mmHg^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|42|NM|10042^MDN_HD_NIBP_PULSE_RATE^L|||^/min^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|43|NM|10043^MDN_HD_SPO2_VALUE^L|||^%^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|44|NM|10044^MDN_HD_SPO2_PERFUSION_INDEX^L|||^%^L||DEMO|||R|||20240401160852+0000\t"
//                              "OBX|45|NM|10045^MDN_HD_SPO2_PULSE_RATE^L|||^/min^L||DEMO|||R|||20240401160852+0000";

//    return demo;
//    // 创建信息头数据
//    ORUR01ScopedPtr oruR01ScopedPtr;

////    oruR01ScopedPtr.Reset(CreateOruR01());

//    MSHScopedPtr mshPtr;
////    mshPtr.Reset(CreateMSH());

//    PIDScopedPtr pidPtr;
////    pidPtr.Reset(CreatePID());

//    OBRScopedPtr obrScopedPtr;
////    obrScopedPtr.Reset(CreateOBR());

//    ORUR01PatientResultScopedPtr patientResultScopedPtr;
////    patientResultScopedPtr.Reset(CreateOruR01PatientResult());

//    ORUR01OrderObrScopedPtr orderObrScopedPtr;
////    orderObrScopedPtr.Reset(CreateOruR01OrderObservation());
//    //////////////////////////各字段数据填充/////////////////////////////
//    mshPtr.Get()->SetSendApp("DrugDetector");
//    //QString strFacilityID = "毒检分析仪^" + GetHostMacAddress() + "^IP-201";
//    QString strFacilityID =  GetHostMacAddress();
//    mshPtr.Get()->SetSendFacilityID(strFacilityID.toUtf8());//
//    mshPtr.Get()->SetSendFacilityName("毒检分析仪");
//    mshPtr.Get()->SetSendFacilityType("IP-201");
//    mshPtr.Get()->SetRecvApp("");
//    mshPtr.Get()->SetRecvFacilityID("");
//    mshPtr.Get()->SetRecvFacilityName("");
//    mshPtr.Get()->SetRecvFacilityType("");
//    mshPtr.Get()->SetDate(QDateTime::currentDateTime().
//                          toString("yyyyMMddhhmmss").toUtf8());//信息的日期时间
//    mshPtr.Get()->SetMessageType("ORU");
//    mshPtr.Get()->SetTriggerEvent("R01");
//    mshPtr.Get()->SetMessageStructure("ORU_R01");
//    mshPtr.Get()->SetMsgCtrlID(_GetMsgCtrlID().c_str());
//    mshPtr.Get()->SetProcessingID("P");
//    mshPtr.Get()->SetVersionID("2.4");
//    mshPtr.Get()->SetApplicationAckType("0");
//    mshPtr.Get()->SetCountryCode("CHN");
//    mshPtr.Get()->SetCharacterSet("Unicode");

//    // PID用户标识信息
//    pidPtr->SetPIDIndex(_GetIncIDIndex(sg_iPidIDIndex).c_str());
//    //    pidPtr->SetID("");// 身份证
//        pidPtr->SetCardNum("2121");// 病例号 qSqlQuery.value(DONOR_ID).toString().toUtf8()
////        pidPtr->SetAdmissionNum("");// 住院号
////        pidPtr->SetBed("");// 床号
////    QString strName = qSqlQuery.value(DONOR_FIREST_NAME).toString()
////            + qSqlQuery.value(DONOR_LASE_NAME).toString();
//    pidPtr->SetName("strName.toUtf8()");
////    QDate qBirthDate = QDate::fromString(qSqlQuery.value(BIRTH_DATE).toString(), "yyyy-MM-dd");
////    pidPtr->SetBirthDate(qBirthDate.toString("yyyy/MM/dd").toUtf8());
//    pidPtr->SetBirthDate("2024/03/27"); // qBirthDate.toString("yyyy/MM/dd").toUtf8()
////        pidPtr->SetBloodType("");
////        pidPtr->SetAddress("");
////        pidPtr->SetHomePhoneNum("");
////        pidPtr->SetTelephoneNum("");
////        pidPtr->SetSSNNum("");
////        pidPtr->SetNation("");
////        pidPtr->SetNativePlace("");


//    // OBR样本信息
////    QDateTime qTestDateTime = QDateTime::fromString(qSqlQuery.value(TEST_TIME).toString(), "yyyy-MM-dd-hh-mm-ss");
////    QString strTestDateTime = qTestDateTime.date().toString("yyyyMMdd") + qTestDateTime.time().toString("hhmmss");
//    QDateTime qTestDateTime = QDateTime::currentDateTime();
//    QString strTestDateTime = qTestDateTime.toString("yyyyMMdd-hhmmss");
//    // 使用donorID作为样本编号
////    QString strObrIndexStr = strFacilityID + "_" + qSqlQuery.value(DONOR_ID).toString()
////            + "_" + strTestDateTime;
////    qDebug() << "obrindex " << strTestDateTime << strObrIndexStr;
//    obrScopedPtr->SetOBRIndex("001");
//    obrScopedPtr->SetProjectID("0001");// 试剂批号/项目编号 qSqlQuery.value(PRODUCT_LOT).toString().toUtf8()
//    obrScopedPtr->SetSampleBarcode("0002");// 样本条码， productID qSqlQuery.value(PRODUCT_ID).toString().toUtf8()
//    obrScopedPtr->SetSampleID("0003");// 样本编号,使用DonorID qSqlQuery.value(DONOR_ID).toString().toUtf8()
//    obrScopedPtr->SetTestTime(strTestDateTime.toUtf8());
//    obrScopedPtr->SetDiagnosticMessage("");
//    obrScopedPtr->SetSubmitSampleTime("");
//    // 样本类型
//    QString strSampleType = "尿液";
////    if(gk_strTCupTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
////    {
////        strSampleType = "尿液";
////    }
////    if(gk_strTCubeTypeList.contains(qSqlQuery.value(PRODUCT_DEFINITION).toString()))
////    {
////        strSampleType = "唾液";
////    }
//    obrScopedPtr->SetSampleType(strSampleType.toUtf8());

//    obrScopedPtr->SetSubmittingPhysician("");
//    obrScopedPtr->SetSubmittingDepartment("");
//    obrScopedPtr->SetAttendingPhysician("");
//    obrScopedPtr->SetTreatDepartment("");

//    int iResultCount = 2; // qSqlQuery.value(PROGRAM_NUMBER).toInt();
////    int iResultIndex = PROGRAM_NAME_BEGIN;
//// OBX 结果信息
//    for(int i = 0; i< iResultCount; ++i)
//    {
//        OBXScopedPtr obxScopedPtr;
////        obxScopedPtr.Reset(CreateOBX());
////        QString strObxIndexTmp = strObrIndexStr + "_" + QString::number(i);

////        obxScopedPtr->SetOBXIndex(strObxIndexTmp.toUtf8());
//        obxScopedPtr->SetOBXIndex("002");
//        obxScopedPtr->SetValueType("ST");
//    //        obxScopedPtr->SetItemID("");

//        obxScopedPtr->SetItemName("zz"); // qSqlQuery.value(iResultIndex).toString().toUtf8()
//        obxScopedPtr->SetItemResult("hello world"); //qSqlQuery.value(iResultIndex+1).toString().toUtf8()
//        obxScopedPtr->SetTestTime(strTestDateTime.toUtf8());
//    //        obxScopedPtr->SetTestPhysician("");
//        orderObrScopedPtr->AddOBX(obxScopedPtr.Get());
//    }

//    ////////////////////////////////////填充消息内容///////////////////////
//    orderObrScopedPtr->SetOBR(obrScopedPtr.Get());

//    patientResultScopedPtr->SetPatient(pidPtr.Get());
//    patientResultScopedPtr->Add_ORU_R01_ORDER_OBSERVATION(orderObrScopedPtr.Get());
//    oruR01ScopedPtr->SetMSH(mshPtr.Get());
//    oruR01ScopedPtr->Add_Patient_Result(patientResultScopedPtr.Get());
//    /////////////////////////////////////查询oruR01ScopedPtr指针是否是实现了IHLMessage接口////////////////
//    IHL7Message* pIHL7Message = QueryInterface<IHL7Message, IObjectBase>(oruR01ScopedPtr.Get(), IF_HL7MESSAGE);
//    std::string strMessage = "";
//    PrintMessageStr(pIHL7Message, strMessage);
//    oruR01ScopedPtr->Release();//调用QueryInterface 会调用oruR01ScopedPtr的AddRef,这里需要释放
//    //
//    QString strTmp = QString::fromStdString(strMessage);
//    qDebug() << " poct message" << strTmp;
//    return strMessage;
//}

//string CHistoryPage::_GetMsgCtrlID()
//{
//    std::string strMsgCtrlID= "";
//    std::stringstream strStream1;
//    strStream1 << sg_iLisMessageCtrlID;
//    strStream1 >> strMsgCtrlID;
//    sg_iLisMessageCtrlID++;
//    return strMsgCtrlID;
//}

//string CHistoryPage::_GetIncIDIndex(int &iIndex)
//{
//    std::string strIndexStr= "";
//    std::stringstream strSteam;
//    strSteam << sg_iLisMessageCtrlID;
//    strSteam >> strIndexStr;
//    iIndex++;
//    return strIndexStr;
//}

///**
//  * @brief 创建数据库表
//  * @param
//  * @return
//  */
//void CHistoryPage::_InitDataBase()
//{
//    if (ConnectDataBase(QCoreApplication::applicationDirPath() + m_strDatabaseName))
//    {
//        QString strCreateTable  = "CREATE TABLE drugdata ("
//                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                                  "DonorFirstName VARCHAR,"
//                                  "DonorLastName VARCHAR,"
//                                  "TestTime VARCHAR,"
//                                  "BirthDate VARCHAR,"
//                                  "DonorID VARCHAR,"
//                                  "TestSite VARCHAR,"
//                                  "Operator VARCHAR,"
//                                  "PreEmployment VARCHAR,"
//                                  "Random VARCHAR,"
//                                  "ReasonSuspicionCause VARCHAR,"
//                                  "PostAccident VARCHAR,"
//                                  "ReturnToDuty VARCHAR,"
//                                  "FollowUp VARCHAR,"
//                                  "OtherReason VARCHAR,"
//                                  "Comments VARCHAR,"
//                                  "TemperatureNormal VARCHAR,"
//                                  "Oxidant VARCHAR,"
//                                  "SpecificGravity VARCHAR,"
//                                  "PH VARCHAR,"
//                                  "Nitrite VARCHAR,"
//                                  "Creatinine VARCHAR,"
//                                  "Email VARCHAR,"
//                                  "ProductDefinition VARCHAR,"
//                                  "ExpirationDate VARCHAR,"
//                                  "ProductLot VARCHAR,"
//                                  "ProductID VARCHAR,"
//                                  "ProgramsNumber INT,";
//        for(int i = 0; i < 16; ++i)
//        {
//            strCreateTable += QString("ProgramName") + QString::number(i) + QString(" VARCHAR,");
//            strCreateTable += QString("Result") + QString::number(i) + QString(" VARCHAR,");
//            strCreateTable += QString("Cutoff") + QString::number(i) + QString(" INT,");
//            strCreateTable += QString("T") + QString::number(i) + QString(" INT,");
//            strCreateTable += QString("C") + QString::number(i) + QString(" INT,");
//            strCreateTable += QString("ControlLine") + QString::number(i) + QString(" VARCHAR,");
//        }
//        strCreateTable += "PrintImagePath VARCHAR)";
//        //strCreateTable.replace(strCreateTable.count()-1, 1, ")");// 替换最后“,”为“)”
//        // 创建
//        QSqlQuery qSqlQuery;
//        if (!qSqlQuery.exec(strCreateTable))
//        {
//            //QMessageBox::critical(0, QObject::tr("Database Error"), qSqlQuery.lastError().text());
//            qDebug() << " database error: " << qSqlQuery.lastError().text();
//        }
//    }

//}




