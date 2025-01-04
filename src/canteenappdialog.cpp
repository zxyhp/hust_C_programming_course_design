#include "ui_canteenappdialog.h"
#include "canteenappdialog.h"

//===========================================================
// 函数名：CanteenAppDialog
// 功能：CanteenAppDialog类的构造函数，完成时间日期组件(dateTimeEdit)
//      的初始化，及其更新的设置
// 输入参数：时间日期变量；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CanteenAppDialog::CanteenAppDialog(QDateTime dt,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanteenAppDialog)
{
    ui->setupUi(this);

    // 初始化DateTimeEdit的时间
    ui->dateTimeEdit->setDateTime(dt);
    // 建立时间变化设置
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // 每1ms就会发射信号timeout()
}

//===========================================================
// 函数名：~CanteenAppDialog
// 功能：CanteenAppDialog类的析构函数，在CanteenAppDialog类的对象
//      释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CanteenAppDialog::~CanteenAppDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_goBackButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CanteenAppDialog::on_goBackButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_winTable_cellDoubleClicked
// 功能：该类窗口界面上的winTable组件的单元格双击信号的槽函数，创建并
//      打开单元格对应的食堂消费窗口界面
// 输入参数：被选中单元格的行下标，被选中单元格的列下标
// 返回值：空
//===========================================================
void CanteenAppDialog::on_winTable_cellDoubleClicked(int row, int column)
{
    // 获取被选中单元格对应的窗口号
    int winNum = row*9 + column + 1;
    // 创建食堂消费窗口类对象
    WindowDialog m_win(winNum,ui->dateTimeEdit->dateTime());
    // 打开食堂消费窗口界面
    m_win.exec();
}

//===========================================================
// 函数名：timeUpdate
// 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加1ms
// 输入参数：空
// 返回值：空
//===========================================================
void CanteenAppDialog::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}
