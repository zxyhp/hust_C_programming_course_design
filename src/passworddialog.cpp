#include "passworddialog.h"
#include "ui_passworddialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// 函数名：PasswordDialog
// 功能：PasswordDialog类的构造函数，设置窗口界面的输入框的输入内容限制
// 输入参数：父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    // 密码输入只能是数字 4位密码
    ui->passwordEdit->setMaxLength(4);
    QRegExp regx("[0-9]+$");
    QValidator* validator = new QRegExpValidator(regx,ui->passwordEdit);
    ui->passwordEdit->setValidator(validator);
}

//===========================================================
// 函数名：~PasswordDialog
// 功能：PasswordDialog类的析构函数，在PasswordDialog类的对象释放时
//      释放ui
// 输入参数：空
// 返回值：空
//===========================================================
PasswordDialog::~PasswordDialog()
{
    delete ui;
}

//===========================================================
// 函数名：changeTitle
// 功能：改变窗口名和标题标签的内容
// 输入参数：新窗口名；新标题标签内容
// 返回值：空
//===========================================================
void PasswordDialog::changeTitle(QString winTitle, QString labelStr)
{
    this->setWindowTitle(winTitle);
    ui->titleLable->setText(winTitle);
    ui->passwordLabel->setText(labelStr);
}

//===========================================================
// 函数名：on_cancelButton_clicked
// 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void PasswordDialog::on_cancelButton_clicked()
{
    sendData("");
    ui->passwordEdit->clear();
    this->close();
}

//===========================================================
// 函数名：on_okButton_clicked
// 功能：根据输入框内容将输入密码传给父窗口的成员变量
// 输入参数：空
// 返回值：空
//===========================================================
void PasswordDialog::on_okButton_clicked()
{
    // 如果输入密码长度小于4
    if(ui->passwordEdit->text().size()<4){
        // 解决中文字符在窗口界面上乱码的问题
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请输入格式正确的密码！"));
        return;
    }

    // 发送数据的信号 它对应的槽可以改变父窗口中的密码成员变量
    sendData(ui->passwordEdit->text());
    ui->passwordEdit->clear();
    this->close();
}
