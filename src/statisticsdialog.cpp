#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// 函数名：StatisticsDialog
// 功能：StatisticsDialog类的构造函数，完成成员变量m_sortedRecords
//      的初始化
// 输入参数：有序消费日志语句的链表头节点指针；父窗口部件的指针（没
//          有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
StatisticsDialog::StatisticsDialog(sortedXf *xf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog)
{
    ui->setupUi(this);
    this->m_sortedRecords = xf;

    // 设置学号输入框只能输入数字 最大输入长度是十位 eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// 函数名：~StatisticsDialog
// 功能：StatisticsDialog类的析构函数，在StatisticsDialog类的对象
//      释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_backButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void StatisticsDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_inquireButton_clicked
// 功能：该类窗口界面上的查询按钮的click()信号的槽函数，遍历消费信息
//      链表根据限定日期和学号，计算学生在该段时间的消费次数和金额并
//      显示在窗口界面的组件上
// 输入参数：空
// 返回值：空
//===========================================================
void StatisticsDialog::on_inquireButton_clicked()
{
    // 输入的学号
    string stuNum = ui->stuNumEdit->text().toStdString();

    if(stuNum.size()<10){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请输入格式正确的学号！"));
        return;
    }

    // 消费总次数
    int allTimes = 0;
    // 消费总金额
    float allConMoney = 0;

    // 如果输入的学好不存在 则结束查询
    auto CO = CardOpration::getinstance();
    if(CO->students.find(stuNum) == CO->students.end()){
        ui->timesLCD->display(QString::number(allTimes));
        ui->moneyLCD->display(QString::number(allConMoney,'f',2));
        return;
    }

    // 起始日期
    string startDate = ui->startDateEdit->date().toString("yyyyMMdd").toStdString();
    // 结束日期
    string endDate = ui->endDateEdit->date().toString("yyyyMMdd").toStdString();

    // 如果起始日期大于结束日期则结束
    if(startDate>endDate) ;
    // 如果起始日期比整个记录的最大日期都大则结束
    else if(startDate>this->m_sortedRecords->date.substr(0,8)) ;
    // 如果记录为空，或结束日期比整个记录的最小日期都小则结束
    else if(this->m_sortedRecords->next==nullptr || endDate<this->m_sortedRecords->next->date ) ;
    //正常搜索
    else{
        // 先定位到指定日期范围处
        sortedXf* p = this->m_sortedRecords->next;
        // 找到p->date>=startDate的p
        while(p->date < startDate) p = p->next;
        // 在制定范围的日期内寻找制定学号的记录
        while(p!=nullptr && p->date <= endDate){
            if(p->stuNum == stuNum){
                allTimes++;
                allConMoney += p->money;
            }
            p = p->next;
        }
    }

    // 在窗口界面组件上显示搜索结果
    ui->timesLCD->display(QString::number(allTimes));
    ui->moneyLCD->display(QString::number(allConMoney,'f',2));
}
