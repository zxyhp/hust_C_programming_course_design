#ifndef CHECKFILEDIALOG_H
#define CHECKFILEDIALOG_H

#include <QDialog>
#include "struct.h"

namespace Ui {
class CheckFileDialog;
}

class CheckFileDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：CheckFileDialog
    // 功能：CheckFileDialog类的构造函数，完成成员变量m_sortXfStr的
    //      初始化
    // 输入参数：有序消费日志语句的链表头节点指针；父窗口部件的指针（没
    //          有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit CheckFileDialog(xfStr* s, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CheckFileDialog
    // 功能：CheckFileDialog类的析构函数，在CanteenAppDialog类的对象
    //      释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CheckFileDialog();

private slots:

    //===========================================================
    // 函数名：on_backBtn_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_backBtn_clicked();

    //===========================================================
    // 函数名：on_checkBtn_clicked
    // 功能：该类窗口界面上的搜索按钮的click()信号的槽函数，搜索消费记录
    //      保存文件内是否有增减一行或多行的改变
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_checkBtn_clicked();

private:
    Ui::CheckFileDialog *ui;
    // 对应经过归并排序后有序(从头至尾按时间升序)的xf操作
    xfStr* m_sortXfStr;

    //===========================================================
    // 函数名：getXfStr
    // 功能：对传入字符串进行解密
    // 输入参数：加密字符串
    // 返回值：解密后的字符串
    //===========================================================
    string getXfStr(string str);

    //===========================================================
    // 函数名：updateTable
    // 功能：对传入字符串进行解密
    // 输入参数：被增加行的信息；被删除行的信息
    // 返回值：空
    //===========================================================
    void updateTable(vector<string> addVec,vector<string> deleteVec);
};

#endif // CHECKFILEDIALOG_H
