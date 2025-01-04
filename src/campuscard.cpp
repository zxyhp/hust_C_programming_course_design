#include "campuscard.h"
#include "ui_campuscard.h"

#include <QMessageBox>
#include <QTextCodec>
#include <QDir>

//===========================================================
// 函数名：CampusCard
// 功能：CampusCard类的构造函数，在CampusCard类的对象声明时调用
//      此处主要完成几个全局指针变量的初始化：日期时间(QDateTime*)、
//      学生操作(CardOpration*)、食堂应用操作(CanteenOpration*)，
//      以及设置时间日期组件(dateTimeEdit)的更新功能
// 输入参数：父窗口部件的指针（没有父窗口则传入nullptr）
// 返回值：空
//===========================================================
CampusCard::CampusCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CampusCard)
{
    ui->setupUi(this);

    // 初始化时间
    auto DT = SetDateTime::getinstance();
    // 初始化 日期时间组件 的时间
    ui->dateTimeEdit->setDateTime(*DT);

    // 建立时间变化设置
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    // 每1ms就会发射信号timeout() 更新ui->dateTimeEdit的显示时间
    timer->start(1);

    // 初始化一个学生操作的类的对象指针
    this->CO = CardOpration::getinstance();

    // 初始化一个食堂应用操作的类的对象指针
    this->COP = CanteenOpration::getinstance();
}

//===========================================================
// 函数名：~CampusCard
// 功能：CampusCard类的析构函数，在CampusCard类的对象释放时调用，
//      此处主要是释放ui和写入操作日志
// 输入参数：空
// 返回值：空
//===========================================================
CampusCard::~CampusCard()
{
    delete ui;
    // 在程序关闭的时候将存在内存的操作日志语句写入文件中
    this->writeOpLog();
}

//===========================================================
// 函数名：writeOpLog
// 功能：将操作日志语句链表内的内容全部写进同目录下的oplog.txt文件中
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::writeOpLog()
{
    // 保存所有操作日志语句的字符串
    string oplog = "";

    // 获取 保存操作日志语句链表的头尾节点指针 的 全局指针变量(opLogHead*类型)
    auto head = OprationLog::getinstance();

    // 获取操作日志语句链表的头节点指针
    opLogStr *p = head->first;

    // 遍历操作日志语句链表 将其中的内容都先获取到oplog中
    while(p->next!=nullptr){
        p=p->next;
        oplog += p->str;
    }

    // 将oplog写入同目录下的oplog.txt中
    string filePath = QDir::currentPath().toStdString() + "/oplog.txt";
    ofstream fp(filePath);
    fp<<oplog;
    fp.close();
}

//===========================================================
// 函数名：updateData
// 功能：调用批量数据导入类LeadData中的函数来导入文件中的批量操作数据
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::updateData()
{
    clock_t startTime = clock();
    string tipStr = "导入数据共花费时间：";

    // 声明一个批量数据导入类的指针
    LeadData *LD = new LeadData;

    // 从同目录下的students.txt中获取开户学生信息并完成其开户
    // 同时在控制台输出initStu函数运行所需的时间(s)
    LD->runTime(&LeadData::initStu);

    // 从同目录下的cz.tzt和xf.txt中获取学生操作和消费数据信息
    // 同时在控制台输出getOpration函数运行所需的时间(s)
    LD->runTime(&LeadData::getOpration);

    // 对 获取的学生操作和消费数据信息 按日期时间 进行升序排序
    LD->runTime(&LeadData::sortOpration);

    // 在系统中完成学生的操作和消费
    // 同时获取最后一条操作的日期和时间（所有操作的最大日期时间）
    pair<string,string> p = LD->carryOutOp();

    // 根据最大的时间日期 更新窗口组件dateTimeEdit上所显示的日期时间
    ui->dateTimeEdit->setDate(QDate::fromString(QString::fromStdString(p.first),"yyyyMMdd"));
    ui->dateTimeEdit->setTime(QTime::fromString(QString::fromStdString(p.second+"0"),"hhmmsszzz"));

    // 清除批量数据导入类指针
    delete LD;

    clock_t endTime = clock();

    // 花费时间字符串
    stringstream ss;
    string timeStr;
    ss<<fixed<<setprecision(3)<<double(endTime - startTime)/CLOCKS_PER_SEC;
    ss>>timeStr;

    tipStr += timeStr + "s";

    // 弹窗显示花费时间
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QMessageBox::information(this,codec->toUnicode("提示"),
                         codec->toUnicode(tipStr.c_str()));
}

//===========================================================
// 函数名：on_quit_clicked
// 功能：该类窗口界面上的退出按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::on_quit_clicked()
{
    this->close();
}

//===========================================================
// 函数名：timeUpdate
// 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加
//      1ms，同时也更新全局时间日期指针变量所指的日期时间
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::timeUpdate()
{
    auto DT = SetDateTime::getinstance();
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
    DT->setDate(time.date());
    DT->setTime(time.time());
}

//===========================================================
// 函数名：on_CardControl_clicked
// 功能：该类窗口界面上的学生管理按钮的click()信号的槽函数，打开学生
//      管理的窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::on_CardControl_clicked()
{
    CardControl *cardcontrol = new CardControl(ui->dateTimeEdit->dateTime());
    cardcontrol->exec();
}

//===========================================================
// 函数名：on_CanteenApp_clicked
// 功能：该类窗口界面上的食堂应用按钮的click()信号的槽函数，打开食堂
//      应用的窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::on_CanteenApp_clicked()
{
    CanteenAppDialog *canteenApp = new CanteenAppDialog(ui->dateTimeEdit->dateTime());
    canteenApp->exec();
}

//===========================================================
// 函数名：on_Summary_clicked
// 功能：该类窗口界面上的汇总分析按钮的click()信号的槽函数，打开汇总
//      分析的窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::on_Summary_clicked()
{
    SummarizeDialog *summarize = new SummarizeDialog(this->COP);
    summarize->exec();
}

//===========================================================
// 函数名：on_inputAllBtn_clicked
// 功能：该类窗口界面上的导入批量数据按钮的click()信号的槽函数，选择
//      是否导入批量数据
// 输入参数：空
// 返回值：空
//===========================================================
void CampusCard::on_inputAllBtn_clicked()
{
    // 防止中文在窗口界面上乱码
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 弹出选择是否进行到导入的窗口
    QMessageBox msgBox;
    msgBox.setText(codec->toUnicode("批量数据仅可导入一次，且导入后不可撤销。"));
    msgBox.setInformativeText(codec->toUnicode("是否继续导入?"));
    msgBox.addButton(codec->toUnicode("是"),QMessageBox::AcceptRole);
    msgBox.addButton(codec->toUnicode("否"),QMessageBox::RejectRole);
    int ret = msgBox.exec();        // 选择信息传给ret变量

    switch (ret) {
    // 如果选择是
    case QMessageBox::AcceptRole:
        // 导入数据按键不可再点击
        ui->inputAllBtn->setEnabled(false);
        // 调用导入批量数据的函数
        this->updateData();
        break;

    // 如果选择否
    case QMessageBox::RejectRole:
        break;

    default:
        // 不会到达这里
        break;
    }

}
