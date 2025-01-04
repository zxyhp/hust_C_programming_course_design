#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <qabstractspinbox.h>
#include <QTableWidgetItem>
#include <QStringList>
#include <QTextCodec>

//===========================================================
// 函数名：SearchDialog
// 功能：SearchDialog类的构造函数，完成成员变量m_sortedRecords的
//      初始化，并设置窗口界面的四个输入框的输入内容限制
// 输入参数：有序食堂消费记录信息的链表头节点的指针；父窗口部件的指
//         针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
SearchDialog::SearchDialog(sortedXf *xf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    this->m_sortedRecords = xf;

    // 设置学号输入框只能输入数字 最大输入长度是十位 eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator1);

    // 设置姓名输入框只能输入汉字 最大输入长度是四个汉字
    ui->nameEdit->setMaxLength(4);
    QRegExp regx2("[\u4e00-\u9fa5]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->nameEdit);
    ui->nameEdit->setValidator(validator2);

    // 设置消费金额下限只能输入0-99.99内的两位浮点数
    ui->minMoneyEdit->setMaxLength(5);
    QRegExp regx3("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator3 = new QRegExpValidator(regx3,ui->minMoneyEdit);
    ui->minMoneyEdit->setValidator(validator3);

    // 设置消费金额上限只能输入0-99.99内的两位浮点数
    ui->maxMoneyEdit->setMaxLength(5);
    QRegExp regx4("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator4 = new QRegExpValidator(regx4,ui->maxMoneyEdit);
    ui->maxMoneyEdit->setValidator(validator4);

}

//===========================================================
// 函数名：~SearchDialog
// 功能：SearchDialog类的析构函数，在SearchDialog类的对象释放时
//      释放ui
// 输入参数：空
// 返回值：空
//===========================================================
SearchDialog::~SearchDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_stuNumCBox_stateChanged
// 功能：根据对学号勾选框的选择设置学号输入框的输入限制
// 输入参数：选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void SearchDialog::on_stuNumCBox_stateChanged(int arg1)
{
    // 选中
    if(arg1 == 2){
        ui->stuNumEdit->setReadOnly(false);
    }
    // 未选中
    else if(arg1 == 0){
        ui->stuNumEdit->clear();
        ui->stuNumEdit->setReadOnly(true);
    }
}

//===========================================================
// 函数名：on_nameCBox_stateChanged
// 功能：根据对姓名勾选框的选择设置姓名输入框的输入限制
// 输入参数：选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void SearchDialog::on_nameCBox_stateChanged(int arg1)
{
    // 选中
    if(arg1 == 2){
        ui->nameEdit->setReadOnly(false);
    }
    // 未选中
    else if(arg1 == 0){
        ui->nameEdit->setReadOnly(true);
        ui->nameEdit->clear();
    }
}

//===========================================================
// 函数名：on_DTCBox_stateChanged
// 功能：根据对日期时间勾选框的选择设置对两个日期时间编辑器的编辑限制
// 输入参数：选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void SearchDialog::on_DTCBox_stateChanged(int arg1)
{
    // enum ButtonSymbols { UpDownArrows, PlusMinus, NoButtons };
    // 选中
    if(arg1 == 2){
        ui->startDTEdit->setReadOnly(false);
        ui->endDTEdit->setReadOnly(false);
        ui->startDTEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        ui->endDTEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    }
    // 未选中
    else if(arg1 == 0){
        ui->startDTEdit->setReadOnly(true);
        ui->endDTEdit->setReadOnly(true);
        ui->startDTEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->endDTEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->startDTEdit->setDate(QDate::fromString("20210923","yyyyMMdd"));
        ui->startDTEdit->setTime(QTime::fromString("000000","HHmmss"));
        ui->endDTEdit->setDate(QDate::fromString("20210923","yyyyMMdd"));
        ui->endDTEdit->setTime(QTime::fromString("000000","HHmmss"));
    }
}

//===========================================================
// 函数名：on_moneyCBox_stateChanged
// 功能：根据对消费金额勾选框的选择设置两个金额输入框的输入限制
// 输入参数：选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void SearchDialog::on_moneyCBox_stateChanged(int arg1)
{
    // 选中
    if(arg1 == 2){
        ui->maxMoneyEdit->setReadOnly(false);
        ui->minMoneyEdit->setReadOnly(false);
    }
    // 未选中
    else if(arg1 == 0){
        ui->maxMoneyEdit->setReadOnly(true);
        ui->minMoneyEdit->setReadOnly(true);
        ui->maxMoneyEdit->clear();
        ui->minMoneyEdit->clear();
    }
}

//===========================================================
// 函数名：on_backButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SearchDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_searchButton_clicked
// 功能：该类窗口界面上的搜索按钮的click()信号的槽函数，根据搜索条件
//      的设定，搜索符合条件的消费信息，并将其显示在窗口界面的组件
//      tableWidget上
// 输入参数：空
// 返回值：空
//===========================================================
void SearchDialog::on_searchButton_clicked()
{
    cout<<"on_searchButton_clicked"<<endl;
    clock_t startTime = clock();

    // 如果记录为空 返回
    if(this->m_sortedRecords == nullptr || this->m_sortedRecords->next == nullptr)
        return;
    // 初始化结果头结点
    this->m_result = new sortedXf;
    // 搜索有效内容
    this->search();
    // 更新表
    this->updateTable();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：updateTable
// 功能：根据搜索结果保存链表，将其信息显示在窗口界面的组件tableWidget上
// 输入参数：空
// 返回值：空
//===========================================================
void SearchDialog::updateTable()
{
    auto CO = CardOpration::getinstance();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    ui->tableWidget->clear();
    // 设置头名称
    ui->tableWidget->setColumnCount(5);
    QStringList head;
    head<<codec->toUnicode("学号")
       <<codec->toUnicode("姓名")
      <<codec->toUnicode("日期")
     <<codec->toUnicode("时间")
    <<codec->toUnicode("消费金额");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    ui->tableWidget->setRowCount(0);
    int i=0;
    auto p = this->m_result->next;
    while(p!=nullptr){
        ui->tableWidget->setRowCount(++i);
        // 学号
        ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(QString::fromStdString(p->stuNum)));
        // 姓名
        string name = CO->students[p->stuNum].name;
        ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(codec->toUnicode(name.c_str())));
        // 日期
        string date = p->date.substr(0,4)+"-"+p->date.substr(4,2)+"-"+p->date.substr(6,2);
        ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(QString::fromStdString(date)));
        // 时间
        string time = p->time.substr(0,2)+":"+p->time.substr(2,2)+":"+p->time.substr(4,2)+"."+p->time.substr(6,2);
        ui->tableWidget->setItem(i-1,3,new QTableWidgetItem(QString::fromStdString(time)));
        // 消费金额
        ui->tableWidget->setItem(i-1,4,new QTableWidgetItem(QString::number(p->money,'f',2)));
        p = p->next;
    }
}

//===========================================================
// 函数名：search
// 功能：根据搜索条件的设定，遍历有序消费信息链表，搜索符合要求的信息，
//      插入到保存结果的链表中
// 输入参数：空
// 返回值：空
//===========================================================
void SearchDialog::search()
{
    string stuNum,name;
    int maxMoney,minMoney;

    // flag的数字代表组合方式
    // 0:无 1:金额 2:姓名 3:金额+姓名 4:学号
    // 5:学号+金额 6:学号+姓名 7:学号+姓名+金额
    int flag = 0;

    /* 如果查询有综合消费金额的话 */
    if(ui->moneyCBox->isChecked()){
        minMoney = QString::number(ui->minMoneyEdit->text().toFloat()*100,'f',0).toInt();
        maxMoney = QString::number(ui->maxMoneyEdit->text().toFloat()*100,'f',0).toInt();
        if(minMoney>maxMoney) return;
        flag += 1;
    }
    /* 如果查询有综合姓名的话 */
    if(ui->nameCBox->isChecked()){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        name = codec->fromUnicode(ui->nameEdit->text()).toStdString();
        if(name == "") return;
        flag += 2;
    }
    /* 如果查询有综合学号的话 */
    if(ui->stuNumCBox->isChecked()){
        stuNum = ui->stuNumEdit->text().toStdString();
        if(stuNum == "") return;
        flag += 4;
    }

    /* 如果查询有综合时间段的话 */
    if(ui->DTCBox->isChecked()){
        // 先定位到时间段
        string startDT = ui->startDTEdit->dateTime().toString("yyyyMMddHHmmss").toStdString()+"00";
        string endDT = ui->endDTEdit->dateTime().toString("yyyyMMddHHmmss").toStdString()+"00";

        // 链表的索引指针
        auto p = this->m_sortedRecords;

        // 如果时间下限大于时间上限 返回
        if(startDT > endDT) return;
        // 如果时间下限大于最大时间 返回
        else if(startDT > p->date) return;
        // 如果时间上限小于最小时间 返回
        else if(endDT < p->next->date+p->next->time)
            return;

        // 先定位到 p->date+p->time >= startDT 的p处
        p = p->next;
        while(p->date+p->time < startDT) p = p->next;

        this->jugeWithTime(this->m_sortedRecords->next,flag,maxMoney,minMoney,stuNum,name,endDT);

    }
    /* 如果查询没有有综合时间段的话 */
    else{
        this->jugeNoTime(this->m_sortedRecords->next,flag,maxMoney,minMoney,stuNum,name);
    }
}

//===========================================================
// 函数名：jugeNoTime
// 功能：搜索函数的辅助函数，在没有时间限制的情况下对有序链表进行搜索
// 输入参数：有序消费信息链表的起始搜索节点的指针；其他选择的综合标
//          志；最大金额；最小金额；学号；姓名
// 返回值：空
//===========================================================
void SearchDialog::jugeNoTime(sortedXf* p,int flag,
         int maxMoney,int minMoney,string stuNum,string name)
{
    switch(flag){
    case 0:
        break;
    case 1: // 金额
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 2: // 姓名
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            if(findOfName!=string::npos &&
                    findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 3: // 金额+姓名
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            auto findOfName = p->name.find(name);
            if(num<=maxMoney && num>=minMoney &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 4: // 学号
        while(p!=nullptr){
            if(p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 5: // 学号+金额
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 6: // 学号+姓名
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            if(p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 7: // 学号+金额+姓名
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    }
}

//===========================================================
// 函数名：jugeWithTime
// 功能：搜索函数的辅助函数，在有时间限制的情况下对有序链表进行搜索
// 输入参数：有序消费信息链表的头节点指针；其他选择的综合标志；最大金额；
//         最小金额；学号；姓名
// 返回值：空
//===========================================================
void SearchDialog::jugeWithTime(sortedXf *p, int flag, int maxMoney, int minMoney, string stuNum, string name, string endTime)
{
    switch(flag){
    case 0:
        while(p!=nullptr && p->date+p->time<=endTime){
            this->addToResult(p);
            p = p->next;
        }
        break;
    case 1: // 金额
        while(p!=nullptr && p->date+p->time<=endTime){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 2: // 姓名
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            if(findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 3: // 金额+姓名
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 4: // 学号
        while(p!=nullptr && p->date+p->time<=endTime){
            if(p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 5: // 学号+金额
        while(p!=nullptr && p->date+p->time<=endTime){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 6: // 学号+姓名
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            if(p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 7: // 学号+金额+姓名
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    }
}

//===========================================================
// 函数名：addToResult
// 功能：根据符合要求的消费节点信息，用头插法插入信息至结果链表中
// 输入参数：空
// 返回值：空
//===========================================================
void SearchDialog::addToResult(sortedXf *p)
{
    sortedXf *sp = new sortedXf(p->cardNum,p->stuNum,p->date,p->time,p->money,p->name,p->winNum);
    sp->next = this->m_result->next;
    this->m_result->next = sp;
}


