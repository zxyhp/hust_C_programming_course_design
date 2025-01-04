#include "inputstunumdialog.h"
#include "ui_inputstunumdialog.h"

#include <QRegExpValidator>
#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// 函数名：inPutStuNumDialog
// 功能：inPutStuNumDialog类的构造函数，完成成员变量m_choice的初始化，
//      并根据选择，设置该复用窗口界面的窗口名，同时设置输入框的输入限制
// 输入参数：选择数；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
inPutStuNumDialog::inPutStuNumDialog(int choice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inPutStuNumDialog)
{
    ui->setupUi(this);

    m_choice = choice;

    // 根据选择设置窗口标题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    switch(choice){
    case 1: // 1选择补卡
       this->setWindowTitle(codec->toUnicode("校园卡补卡"));
        break;
    case 2: // 2选择挂失
        this->setWindowTitle(codec->toUnicode("校园卡挂失"));
        break;
    case 3: // 3选择解挂
        this->setWindowTitle(codec->toUnicode("校园卡解挂"));
        break;
    case 4: // 4选择销户
        this->setWindowTitle(codec->toUnicode("学生销户"));
        break;
    default:
        break;
    }

    // 设置学号输入框只能输入数字 最大输入长度是十位 eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// 函数名：~inPutStuNumDialog
// 功能：inPutStuNumDialog类的析构函数，在CosumeDialog类的对象释
//      放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
inPutStuNumDialog::~inPutStuNumDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_okButton_clicked
// 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据选择变量
//      和输入信息调用系统中对应的功能函数（m_choice：1,补卡；2,挂失；
//      3,解挂；4,销户）
// 输入参数：空
// 返回值：空
//===========================================================
void inPutStuNumDialog::on_okButton_clicked()
{
    // 学号
    string stuNum = ui->stuNumEdit->text().toStdString();

    // 学号输入应该是十位数字
    if(stuNum.size()<10){
        // 解决中文字符在窗口界面上乱码的问题
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入格式正确的学号！"));
        return;
    }

    // 获取时间
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // 完成对应的操作
    auto CO = CardOpration::getinstance();
    switch(m_choice){
    case 1: // 1选择补卡
        CO->makeUpCard(date,time,stuNum);
        break;
    case 2: // 2选择挂失
        CO->lossSolution_1(date,time,stuNum);
        break;
    case 3: // 3选择解挂
        CO->lossSolution_2(date,time,stuNum);
        break;
    case 4: // 4选择销户
        CO->deleteStu(date,time,stuNum);
        break;
    default:
        break;
    }

    ui->stuNumEdit->clear();
    this->close();
}

//===========================================================
// 函数名：on_cancelButton_clicked
// 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void inPutStuNumDialog::on_cancelButton_clicked()
{
    ui->stuNumEdit->clear();
    this->close();
}
