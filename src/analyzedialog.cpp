#include "analyzedialog.h"
#include "ui_analyzedialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// 函数名：AnalyzeDialog
// 功能：AnalyzeDialog类的构造函数，初始化成员变量m_allReacords、
//      m_sortedRecords，并设置学号输入框的输入限制
// 输入参数：各个窗口的消费记录信息的入口键值对集合；有序消费记录链表的
//         头节点的指针；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
AnalyzeDialog::AnalyzeDialog(map<int,pair<int,winXfStr*>> m, sortedXf* sortedRecords, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeDialog)
{
    ui->setupUi(this);

    // 从每个窗口读取的记录map集合
    this->m_allReacords = m;

    // 从窗口记录读取到的所有记录的有序集合
    this->m_sortedRecords = sortedRecords;

    // 设置学号输入框只能输入数字 最大输入长度是十位 eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// 函数名：~AnalyzeDialog
// 功能：AnalyzeDialog类的析构函数，在AnalyzeDialog类的对象释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
AnalyzeDialog::~AnalyzeDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_backBtn_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void AnalyzeDialog::on_backBtn_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_analyzeBtn_clicked
// 功能：该类窗口界面上的分析按钮的click()信号的槽函数，分析出与输入
//      学号对应学生经常一起在食堂排序消费的学生，将他们的信息存入成
//      员变量m_info中
// 输入参数：空
// 返回值：空
//===========================================================
void AnalyzeDialog::on_analyzeBtn_clicked()
{
    cout<<"on_analyzeBtn_clicked"<<endl;
    clock_t startTime = clock();

    this->m_info.clear();

    // 学号
    string stuNum = ui->stuNumEdit->text().toStdString();

    if(stuNum.size()<10){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("提示"),
                             codec->toUnicode("请输入格式正确的学号！"));
        return;
    }

    auto CO = CardOpration::getinstance();
    if(CO->students.find(stuNum) == CO->students.end()){
        return;
    }

    // 考虑在一个窗口一起排队的情况
    auto end = this->m_allReacords.end();
    for(auto it=this->m_allReacords.begin();it!=end;it++){
        int num = it->second.first;
        // 最后一条信息的指针位置
        auto nowStu = it->second.second;
        string stu1,stu2;
        while(num>0){
            // 如果p的记录学号为stuNum
            if(nowStu->stuNum == stuNum){
                stu1 = nowStu->next->stuNum;
                // 如果stu1同学还未和p同学排过队
                if(nowStu->next->date == nowStu->date && nowStu->next->time[0] == nowStu->time[0]){
                    if(m_info.find(stu1)==m_info.end()){
                        pair<int,string> p(1,nowStu->next->name);
                        m_info[stu1] = p;
                    }
                    else{
                        // 如果排过 则次数加一
                        m_info[stu1].first++;
                    }
                }
                stu2 = nowStu->previous->stuNum;
                // 如果stu2同学还未和p同学排过队
                if(nowStu->previous->date == nowStu->date && nowStu->previous->time[0] == nowStu->time[0]){
                    if(m_info.find(stu2)==m_info.end()){
                        pair<int,string> p(1,nowStu->previous->name);
                        m_info[stu2] = p;
                    }
                    else{
                        // 如果排过 则次数加一
                        m_info[stu2].first++;
                    }
                }
            }
            nowStu = nowStu->previous;
            num--;
        }
    }


    // 考虑在相邻窗口几乎同时消费的人的情况
    auto p = this->m_sortedRecords;
    // 如果消费记录为空
    if(p->next == nullptr || p->next->next == nullptr){
        this->updateTable();
        clock_t endTime = clock();
        cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
        return;
    }

    auto pre_p = p;
    p = pre_p->next;
    auto next_p = p->next;

    while(next_p!=nullptr){
        // 如果p是目标对象
        if(p->stuNum == stuNum){
            int winNum = p->winNum;
            string date = p->date;

            // 判断pre_p是否可以纳入选择中
            // 如果是相邻窗口
            if(pre_p->winNum-winNum==-1 || pre_p->winNum-winNum==1){
                // 判断是否是同一天
                if(pre_p->date == date){
                    // 如果pre_p同学还未和p同学排过队
                    if(m_info.find(pre_p->stuNum)==m_info.end()){
                        pair<int,string> pair_obj(1,pre_p->name);
                        m_info[pre_p->stuNum] = pair_obj;
                    }
                    // 如果排过 则次数加一
                    else{
                        m_info[pre_p->stuNum].first++;
                    }
                }
            }

            // 判断next_p是否可以纳入选择中
            // 如果是相邻窗口
            if(next_p->winNum-winNum==-1 || next_p->winNum-winNum==1){
                // 判断是否是同一天
                if(next_p->date == date){
                    // 如果pre_p同学还未和p同学排过队
                    if(m_info.find(next_p->stuNum)==m_info.end()){
                        pair<int,string> pair_obj(1,next_p->name);
                        m_info[next_p->stuNum] = pair_obj;
                    }
                    // 如果排过 则次数加一
                    else{
                        m_info[next_p->stuNum].first++;
                    }
                }
            }

        }
        pre_p = p;  p = next_p; next_p = next_p->next;
    }

    // 还会剩下最后一个节点没有判断
    if(p->stuNum == stuNum){
        int winNum = p->winNum;
        string date = p->date;

        // 判断pre_p是否可以纳入选择中
        // 如果是相邻窗口
        if(pre_p->winNum-winNum==-1 || pre_p->winNum-winNum==1){
            // 判断是否是同一天
            if(pre_p->date == date){
                // 如果pre_p同学还未和p同学排过队
                if(m_info.find(pre_p->stuNum)==m_info.end()){
                    pair<int,string> pair_obj(1,pre_p->name);
                    m_info[pre_p->stuNum] = pair_obj;
                }
                // 如果排过 则次数加一
                else{
                    m_info[pre_p->stuNum].first++;
                }
            }
        }
    }


    // 如果有空行和本身被算入则删去（同窗口读取双向循环链表时有可能发生）
    if(this->m_info.find("")!= this->m_info.end()){
        this->m_info.erase("");
    }
    if(this->m_info.find(stuNum)!= this->m_info.end()){
        this->m_info.erase(stuNum);
    }

    // 将搜索结果更新到窗口界面的显示表组件上
    this->updateTable();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;

}

//===========================================================
// 函数名：updateTable
// 功能：更新成员变量m_info的信息至窗口界面的tableWidget组件上
// 输入参数：空
// 返回值：空
//===========================================================
void AnalyzeDialog::updateTable()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    ui->tableWidget->clear();

    // 设置头行
    ui->tableWidget->setColumnCount(3);
    QStringList head;
    head<<codec->toUnicode("学号")
       <<codec->toUnicode("姓名")
      <<codec->toUnicode("次数");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    auto end = this->m_info.end();
    int i=1;
    for(auto it = this->m_info.begin();it!=end;it++){
        if(it->second.first <= 2) continue;
        // 设置行数
        ui->tableWidget->setRowCount(i);
        // 学号
        ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(QString::fromStdString(it->first)));
        // 姓名
        ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(codec->toUnicode(it->second.second.c_str())));
        // 次数
        ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(QString::number(it->second.first)));

        i++;
    }

}
