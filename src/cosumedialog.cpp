#include "cosumedialog.h"
#include "ui_cosumedialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// 函数名：CosumeDialog
// 功能：CosumeDialog类的构造函数，完成成员变量m_winNum的初始化，
//      并设置窗口界面的两个输入框的输入内容限制
// 输入参数：食堂窗口号-1；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CosumeDialog::CosumeDialog(int winNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CosumeDialog)
{
    ui->setupUi(this);
    this->m_winNum = winNum;

    // 设置卡号输入框只能输入数字 最大输入长度是7位 eg.3123761
    ui->cardNumEdit->setMaxLength(7);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->cardNumEdit);
    ui->cardNumEdit->setValidator(validator1);

    // 设置消费金额只能输入0-100内的两位浮点数
    ui->addMoneyEdit->setMaxLength(5);
    QRegExp regx2("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->addMoneyEdit);
    ui->addMoneyEdit->setValidator(validator2);

}

//===========================================================
// 函数名：~CosumeDialog
// 功能：CosumeDialog类的析构函数，在CosumeDialog类的对象释放时
//      释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CosumeDialog::~CosumeDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_cancelButton_clicked
// 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CosumeDialog::on_cancelButton_clicked()
{
    ui->cardNumEdit->clear();
    ui->addMoneyEdit->clear();
    this->close();
}

//===========================================================
// 函数名：on_okButton_clicked
// 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据输入信息
//      调用系统中的消费函数完成学生的消费操作
// 输入参数：空
// 返回值：空
//===========================================================
void CosumeDialog::on_okButton_clicked()
{
    // 解决中文字符在窗口界面上乱码的问题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 卡号
    string cardNum = ui->cardNumEdit->text().toStdString();

    // 卡号输入应该是7位数字 姓名输入是2-4个汉字
    if(cardNum.size()<7){
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入格式正确的卡号！"));
        return;
    }

    // 如果输入消费金额为空
    if(ui->addMoneyEdit->text()==""){
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请先输入消费金额！"));
        return;
    }

    // 消费金额
    float conMoney = ui->addMoneyEdit->text().toFloat();

    // 获取时间
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // 消费
    auto COP = CanteenOpration::getinstance();
    COP->consume(false,m_winNum,cardNum,conMoney,date,time);

    ui->cardNumEdit->clear();
    ui->addMoneyEdit->clear();
    this->close();
}
