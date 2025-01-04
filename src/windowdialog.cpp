#include "windowdialog.h"
#include "ui_windowdialog.h"

//===========================================================
// 函数名：WindowDialog
// 功能：WindowDialog类的构造函数，初始化成员变量m_winNu，同时完成
//      时间日期组件(dateTimeEdit)的初始化及其更新的设置
// 输入参数：窗口号-1；时间日期变量；父窗口部件的指针（没有父窗口则
//          会传入nullptr）
// 返回值：空
//===========================================================
WindowDialog::WindowDialog(int winNum,QDateTime dt,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowDialog)
{
    ui->setupUi(this);

    this->m_winNum = winNum;
    ui->winNumLabel->setNum(winNum);

    // 初始化DateTimeEdit的时间
    ui->dateTimeEdit->setDateTime(dt);
    // 建立时间变化设置
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // 每1ms就会发射信号timeout()

    // 初始化当日消费总次数、总金额的显示组件
    this->updateLCDNum();
}

//===========================================================
// 函数名：~WindowDialog
// 功能：WindowDialog类的析构函数，在WindowDialog类的对象释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
WindowDialog::~WindowDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_pushButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void WindowDialog::on_pushButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_consumeButton_clicked
// 功能：该类窗口界面上的消费按钮的click()信号的槽函数，打开消费窗口
//      界面
// 输入参数：空
// 返回值：空
//===========================================================
void WindowDialog::on_consumeButton_clicked()
{
    CosumeDialog cosume(this->m_winNum-1);
    cosume.exec();
    this->updateLCDNum();
}

//===========================================================
// 函数名：on_csLogButton_clicked
// 功能：该类窗口界面上的查看消费日志按钮的click()信号的槽函数，打开
//      消费日志窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void WindowDialog::on_csLogButton_clicked()
{
    CosumeLogDialog cosumeLog(this->m_winNum-1);
    cosumeLog.exec();
}

//===========================================================
// 函数名：timeUpdate
// 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加1ms
// 输入参数：空
// 返回值：空
//===========================================================
void WindowDialog::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}

//===========================================================
// 函数名：updateLCDNum
// 功能：更新窗口界面上显示的窗口当日消费总次数和总金额的示数
// 输入参数：空
// 返回值：空
//===========================================================
void WindowDialog::updateLCDNum()
{
    auto COP = CanteenOpration::getinstance();
    pair<int,float> p = COP->getWinConsume(this->m_winNum-1);
    ui->csTimesLCD->display(QString::number(p.first));
    ui->csMoneyLCD->display(QString::number(p.second,'f',2));
}
