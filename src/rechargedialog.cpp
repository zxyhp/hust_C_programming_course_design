#include "rechargedialog.h"
#include "ui_rechargedialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// 函数名：RechargeDialog
// 功能：RechargeDialog类的构造函数，设置窗口界面的两个输入框的输
//      入内容限制
// 输入参数：父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
RechargeDialog::RechargeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RechargeDialog)
{
    ui->setupUi(this);

    // 设置学号输入框只能输入数字 最大输入长度是10位 eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator1);

    // 设置充值金额输入框 只能输入1-999中整数
    ui->moneyEdit->setMaxLength(3);
    QRegExp regx2("[1-9][0-9]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->moneyEdit);
    ui->moneyEdit->setValidator(validator2);
}

//===========================================================
// 函数名：~RechargeDialog
// 功能：RechargeDialog类的析构函数，在RechargeDialog类的对象释放时
//      释放ui
// 输入参数：空
// 返回值：空
//===========================================================
RechargeDialog::~RechargeDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_okButton_clicked
// 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据输入信息
//      调用系统中的充值函数完成学生的账户充值
// 输入参数：空
// 返回值：空
//===========================================================
void RechargeDialog::on_okButton_clicked()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 学号
    string stuNum = ui->stuNumEdit->text().toStdString();
    // 学号输入应该是十位数字
    if(stuNum.size()<10){
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入格式正确的学号！"));
        return;
    }

    // 充值金额不能为空
    if(ui->moneyEdit->text()==""){
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入充值金额！"));
    }

    // 充值金额
    float money = ui->moneyEdit->text().toFloat();

    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();
    // 获取时间
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);
    // 充值
    CO->recharge(date,time,stuNum,money);

    ui->stuNumEdit->clear();
    ui->moneyEdit->clear();
    this->close();
}

//===========================================================
// 函数名：on_cancelButton_clicked
// 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void RechargeDialog::on_cancleButton_clicked()
{
    ui->stuNumEdit->clear();
    ui->moneyEdit->clear();
    this->close();
}
