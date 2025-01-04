#ifndef CAMPUSCARD_H
#define CAMPUSCARD_H

#include <QWidget>
#include "cardcontrol.h"
#include "cardoprotion.h"
#include "canteenappdialog.h"
#include "canteenopration.h"
#include "oprationlog.h"
#include "setdatetime.h"
#include "summarizedialog.h"
#include "leaddata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CampusCard; }
QT_END_NAMESPACE

class CampusCard : public QWidget
{
    Q_OBJECT

public:
    //===========================================================
    // 函数名：CampusCard
    // 功能：CampusCard类的构造函数，在CampusCard类的对象声明时调用
    //      此处主要完成几个全局指针变量的初始化：日期时间(QDateTime*)、
    //      学生操作(CardOpration*)、食堂应用操作(CanteenOpration*)，
    //      以及设置时间日期组件(dateTimeEdit)的更新功能
    // 输入参数：父窗口部件的指针（没有父窗口则传入nullptr）
    // 返回值：空
    //===========================================================
    CampusCard(QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CampusCard
    // 功能：CampusCard类的析构函数，在CampusCard类的对象释放时调用，
    //      此处主要是释放ui和写入操作日志
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CampusCard();

private slots:
    //===========================================================
    // 函数名：on_quit_clicked
    // 功能：该类窗口界面上的退出按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_quit_clicked();

    //===========================================================
    // 函数名：timeUpdate
    // 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加
    //      1ms，同时也更新全局时间日期指针变量所指的日期时间
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void timeUpdate();

    //===========================================================
    // 函数名：on_CardControl_clicked
    // 功能：该类窗口界面上的学生管理按钮的click()信号的槽函数，打开学生
    //      管理的窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_CardControl_clicked();

    //===========================================================
    // 函数名：on_CanteenApp_clicked
    // 功能：该类窗口界面上的食堂应用按钮的click()信号的槽函数，打开食堂
    //      应用的窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_CanteenApp_clicked();

    //===========================================================
    // 函数名：on_Summary_clicked
    // 功能：该类窗口界面上的汇总分析按钮的click()信号的槽函数，打开汇总
    //      分析的窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_Summary_clicked();

    //===========================================================
    // 函数名：on_inputAllBtn_clicked
    // 功能：该类窗口界面上的导入批量数据按钮的click()信号的槽函数，选择
    //      是否导入批量数据
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_inputAllBtn_clicked();

private:
    Ui::CampusCard *ui;
    CardOpration *CO;
    CanteenOpration *COP;

    //===========================================================
    // 函数名：writeOpLog
    // 功能：将操作日志语句链表内的内容全部写进同目录下的oplog.txt文件中
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void writeOpLog();

    //===========================================================
    // 函数名：updateData
    // 功能：调用批量数据导入类LeadData中的函数来导入文件中的批量操作数据
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateData();
};
#endif // CAMPUSCARD_H
