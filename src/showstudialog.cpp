#include "setdatetime.h"
#include "showstudialog.h"
#include "ui_showstudialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// 函数名：ShowStuDialog
// 功能：ShowStuDialog类的构造函数，初始化成员变量m_stuNum，根据传
//      入学生学号更新窗口界面信息
// 输入参数：学生学号；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
ShowStuDialog::ShowStuDialog(string stuNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowStuDialog)
{
    ui->setupUi(this);
    this->m_stuNum = stuNum;
    this->updateTableWidget();
}

//===========================================================
// 函数名：~ShowStuDialog
// 功能：ShowStuDialog类的析构函数，在ShowStuDialog类的对象释放
//      时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
ShowStuDialog::~ShowStuDialog()
{
    delete ui;
}

//===========================================================
// 函数名：updateTableWidget
// 功能：根据传入学号更新窗口界面组件上显示的窗口信息
// 输入参数：空
// 返回值：空
//===========================================================
void ShowStuDialog::updateTableWidget(){

    // 解决中文字符在窗口桌面上乱码的问题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 获取传入学号的学生的其他信息
    auto CO = CardOpration::getinstance();
    string name = CO->students[m_stuNum].name;
    float money = CO->students[m_stuNum].money;
    int numOfCard = CO->students[m_stuNum].numOfCard;

    // 设置信息标签内容
    ui->stuNumLabel->setText(QString::fromStdString(m_stuNum));
    ui->nameLabel->setText(codec->toUnicode(name.c_str()));
    ui->moneyLabel->setText(QString::number(money,'f',2));

    // 设置卡片显示表的表头
    ui->cardTable->clear();
    ui->cardTable->setColumnCount(2);
    QStringList head;
    head<<codec->toUnicode("卡号")<<codec->toUnicode("状态");
    ui->cardTable->setHorizontalHeaderLabels(head);

    // 设置卡片显示表的行数
    ui->cardTable->setRowCount(numOfCard);

    // 遍历学生的历代卡号链表 并将其添加到卡片显示表上
    int i=0;
    CardNum *p = CO->students[m_stuNum].cardhead;
    QString status[]={codec->toUnicode("使用中"),codec->toUnicode("冻结中")};
    while(p->next!=NULL){
        p = p->next;
        ui->cardTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(p->number)));
        ui->cardTable->setItem(i,1,new QTableWidgetItem(status[CO->cards[p->number].status]));
        i++;
    }
}

//===========================================================
// 函数名：on_closeButton_clicked
// 功能：该类窗口界面上的关闭按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void ShowStuDialog::on_closeButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_ChangePasswordBtn_clicked
// 功能：该类窗口界面上的修改密码按钮的click()信号的槽函数，复用
//      PasswordDialog类对应的窗口来修改密码
// 输入参数：空
// 返回值：空
//===========================================================
void ShowStuDialog::on_ChangePasswordBtn_clicked()
{
    // 防止中文乱码
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 获取时间
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // 操作日志对象 用于后序写入操作的日志
    OprationLog oplog;
    // 修改密码操作日志语句样例
    // date,time,修改密码,学号,成功
    // date,time,修改密码,学号,失败,失败原因
    string oplogStr = date + "," + time + "," + "修改密码"
            + "," + this->m_stuNum + ",";

    // 密码输入窗口
    PasswordDialog* PD = new PasswordDialog;
    PD->changeTitle(codec->toUnicode("密码验证"),codec->toUnicode("旧密码"));
    // 连接槽 如果输入密码并确认 则 this->m_password就会获取到输入的密码
    connect(PD,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    PD->exec();

    auto CO = CardOpration::getinstance();

    // 如果取消了修改
    if(this->m_password == "") ;
    // 如果旧密码输入错误
    else if(this->m_password != CO->students[this->m_stuNum].password){
        oplogStr += "失败,旧密码验证错误\n";
        oplog.writeOpLog(oplogStr);
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("密码修改失败!"));
    }
    // 如果旧密码输入正确
    else{
        // 获取新密码
        PD->changeTitle(codec->toUnicode("修改密码"),codec->toUnicode("新密码"));
        PD->exec();
        // 如果获取到了新密码
        if(this->m_password != ""){
            oplogStr += "成功\n";
            oplog.writeOpLog(oplogStr);
            // 修改密码
            CO->students[this->m_stuNum].password = this->m_password;
            QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("密码修改成功!"));
        }
    }
}

//===========================================================
// 函数名：receiveData
// 功能：用于接收密码输入窗口中输入框内输入的字符串至成员变量m_password中
// 输入参数：密码输入窗口中输入框内输入的字符串
// 返回值：空
//===========================================================
void ShowStuDialog::receiveData(QString date)
{
    this->m_password = date.toStdString();
}
