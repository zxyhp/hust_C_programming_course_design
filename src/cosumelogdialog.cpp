#include "cosumelogdialog.h"
#include "ui_cosumelogdialog.h"

//===========================================================
// 函数名：CosumeLogDialog
// 功能：CosumeLogDialog类的构造函数，完成成员变量m_winNum的初始化，
//      并将对应窗口号的消费记录信息同步到窗口界面的表组件上
// 输入参数：食堂窗口号-1；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CosumeLogDialog::CosumeLogDialog(int num, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CosumeLogDialog)
{
    ui->setupUi(this);

    this->m_winNum = num;
    // 设置窗口界面的标题标签
    ui->winNumLabel->setText(QString::number(m_winNum+1));
    // 更新表内容
    this->updateTable();
}

//===========================================================
// 函数名：~CosumeLogDialog
// 功能：CosumeLogDialog类的析构函数，在CosumeLogDialog类的对象释
//      放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CosumeLogDialog::~CosumeLogDialog()
{
    delete ui;
}


//===========================================================
// 函数名：updateTable
// 功能：将对应窗口号的消费记录内容显示到窗口界面的tableWidget组件上
// 输入参数：空
// 返回值：空
//===========================================================
void CosumeLogDialog::updateTable()
{
    // 获取食堂对象
    auto COP = CanteenOpration::getinstance();
    winXfHead hp = COP->getXfRecords(m_winNum);
    int lineOfNow = COP->getWinLine(m_winNum);
    int num = hp.num;
    // 设置TableWidget的行数
    ui->tableWidget->setRowCount(FILELINES);
    // 辅助变量
    winXfStr *p = hp.now->previous;
    string str;
    while(num>0){
        lineOfNow = (lineOfNow-1+FILELINES)%FILELINES;
        // 日期 eg.2021-10-05
        str = p->date.substr(0,4)+"-"+p->date.substr(4,2)+"-"+p->date.substr(6,2);
        ui->tableWidget->setItem(lineOfNow,0,new QTableWidgetItem(QString::fromStdString(str)));
        // 时间 eg.07:00:11.72
        str = p->time.substr(0,2)+":"+p->time.substr(2,2)+":"+p->time.substr(4,2)+"."+p->time.substr(6,2);
        ui->tableWidget->setItem(lineOfNow,1,new QTableWidgetItem(QString::fromStdString(str)));
        // 卡号
        ui->tableWidget->setItem(lineOfNow,2,new QTableWidgetItem(QString::fromStdString(p->cardNum)));
        // 消费金额
        ui->tableWidget->setItem(lineOfNow,3,new QTableWidgetItem(QString::number(p->money,'f',2)));
        num--;
        p=p->previous;
    }
}

//===========================================================
// 函数名：on_goBackButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CosumeLogDialog::on_goBackButton_clicked()
{
    this->close();
}


