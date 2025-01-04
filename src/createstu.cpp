#include "createstu.h"
#include "ui_createstu.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// 函数名：CreateStu
// 功能：CreateStu类的构造函数，设置窗口界面的两个输入框的输入内容限制
// 输入参数：父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CreateStu::CreateStu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateStu)
{
    ui->setupUi(this);

    // 设置学号输入框只能输入数字 最大输入长度是十位 eg.2020121753
    ui->StuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->StuNumEdit);
    ui->StuNumEdit->setValidator(validator1);

    // 设置姓名输入框只能输入汉字 最大输入长度是四个汉字
    ui->nameEdit->setMaxLength(4);
    QRegExp regx2("[\u4e00-\u9fa5]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->nameEdit);
    ui->nameEdit->setValidator(validator2);
}

//===========================================================
// 函数名：~CreateStu
// 功能：CreateStu类的析构函数，在CreateStu类的对象释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CreateStu::~CreateStu()
{
    delete ui;
}

//===========================================================
// 函数名：on_okButton_clicked
// 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据输入信息
//      调用系统中的开户函数完成学生的开户
// 输入参数：空
// 返回值：空
//===========================================================
void CreateStu::on_okButton_clicked()
{
    // 解决中文字符在窗口界面上乱码的问题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 学号
    string stuNum = ui->StuNumEdit->text().toStdString();
    // 姓名
    string name = codec->fromUnicode(ui->nameEdit->text()).toStdString();

    // 学号输入应该是十位数字 姓名输入是2-4个汉字
    if(stuNum.size()<10 || name.size()<4){
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入格式正确的学号和姓名！"));
        return;
    }

    // 输入正确则调用相应函数
    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();
    // 获取时间
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);
    // 开户
    CO->createStu(date,time,stuNum,name);

    ui->StuNumEdit->clear();
    ui->nameEdit->clear();
    this->close();
}

//===========================================================
// 函数名：on_cancleButton_clicked
// 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CreateStu::on_cancleButton_clicked()
{
    ui->StuNumEdit->clear();
    ui->nameEdit->clear();
    this->close();
}
