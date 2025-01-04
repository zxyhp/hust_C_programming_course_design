 #include "checkfiledialog.h"
#include "ui_checkfiledialog.h"

#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <fstream>
#include <sstream>

#define Y 24
#define S 18
#define F 5
#define KEYLENGTH 3
#define ALLLENGTH 26
#define MAXSIZE 64

//===========================================================
// 函数名：CheckFileDialog
// 功能：CheckFileDialog类的构造函数，完成成员变量m_sortXfStr的
//      初始化
// 输入参数：有序消费日志语句的链表头节点指针；父窗口部件的指针（没
//          有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CheckFileDialog::CheckFileDialog(xfStr* s,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckFileDialog)
{
    ui->setupUi(this);
    this->m_sortXfStr = s;
}

//===========================================================
// 函数名：~CheckFileDialog
// 功能：CheckFileDialog类的析构函数，在CanteenAppDialog类的对象
//      释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CheckFileDialog::~CheckFileDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_backBtn_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CheckFileDialog::on_backBtn_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_checkBtn_clicked
// 功能：该类窗口界面上的搜索按钮的click()信号的槽函数，搜索消费记录
//      保存文件内是否有增减一行或多行的改变
// 输入参数：空
// 返回值：空
//===========================================================
void CheckFileDialog::on_checkBtn_clicked()
{
    cout<<"on_checkBtn_clicked"<<endl;
    clock_t startTime = clock();

    // 辅助变量
    char ch[MAXSIZE];
    string xfStr = "";
    string moneyStr;
    string xfStrP = "";
    stringstream ss;

    // 保存被删除的语句
    vector<string> deleteVec;

    // 保存自行增加的语句
    vector<string> addVec;

    auto p = this->m_sortXfStr->next;

    string filePath = QDir::currentPath().toStdString() + "/all_xf.txt";
    ifstream fp(filePath);
    if(!fp){
        cout<<"all_xf.txt文件打开失败！"<<endl;
        return;
    }

    while(p!=nullptr){
        // 如果文件已经读取完毕 则跳出循环
        if(!fp.getline(ch,MAXSIZE)) break;
        // 获取到的解密后的语句 及 语句中的具体内容
        xfStr = this->getXfStr(string(ch,strlen(ch)));

        if(p->str != xfStr){

            // 从p2的下一条开始索引，看该条不服和条件的语句是增加的还是删除的
            auto p2 = p;

            while(p2->next!=nullptr){
                p2 = p2->next;
                // 说明找到了对应的语句 xfStr语句之前有语句被删除
                if(p2->str == xfStr) break;
            }

            if(p2->next == nullptr){
                // 没有找到该条语句 是被增加的
                addVec.push_back(xfStr);
                continue;
            }
            // 找到该条语句
            else{
                // p到p2之前对应的语句都是被删除的语句 p2的语句正常存放
                while(p!=p2){
                    deleteVec.push_back(p->str);
                    p = p->next;
                }
            }
        }
        p = p -> next;
    }

    // 如果p不为空就跳出了循环 则说明结尾还有语句被删除了
    if(p!=nullptr){
        while(p!=nullptr){
            deleteVec.push_back(p->str);
            p = p->next;
        }
    }
    // 如果数据文件还未读完就跳出了循环 则说明文件尾还有语句被增加
    else{
        while(fp.getline(ch,MAXSIZE)){
            xfStr = this->getXfStr(string(ch,strlen(ch)));
            addVec.push_back(xfStr);
        }
    }

    fp.close();

    updateTable(addVec,deleteVec);

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：getXfStr
// 功能：对传入字符串进行解密
// 输入参数：加密字符串
// 返回值：解密后的字符串
//===========================================================
string CheckFileDialog::getXfStr(string str)
{
    // 保存解密后的字符串
    string xfStr;
    static int key[]={Y,S,F}; // key[0]=24,key[1]=18,key[2]=5
    // 获得加密的起点(0、1、2)
    int checkNum = str[0]-'a';

    // 辅助变量
    char ch;
    int num;

    // 遍历加密字符串
    auto it = str.begin();
    it++;
    auto end = str.end();
    while(it!=end){
        num = (*it-'a')-key[checkNum];

        if(num<0) num += ALLLENGTH;

        // '0'-'9'对应 0-9 ；‘.’对应10
        if(num==10) ch='.';
        else ch = num+'0';

        // 获取下一个加密数字的下标
        checkNum = (checkNum+1)%KEYLENGTH;

        xfStr += ch;
        it++;
    }

    // 返回解密后的字符串
    return xfStr;
}

//===========================================================
// 函数名：updateTable
// 功能：对传入字符串进行解密
// 输入参数：被增加行的信息；被删除行的信息
// 返回值：空
//===========================================================
void CheckFileDialog::updateTable(vector<string> addVec, vector<string> deleteVec)
{
    ui->tableWidget->clear();

    // 解决中文字符在窗口界面乱码的问题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 先清空表头
    ui->tableWidget->clear();
    // 设置表的列数
    ui->tableWidget->setColumnCount(5);
    // 设置表头内容
    QStringList head;
    head<<codec->toUnicode("日期")
       <<codec->toUnicode("时间")
      <<codec->toUnicode("卡号")
     <<codec->toUnicode("消费金额")
    <<codec->toUnicode("错误原因");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    // 设置行数
    int lineNum = addVec.size() + deleteVec.size();
    ui->tableWidget->setRowCount(lineNum);


    int i=0;
    // 被增加字符容器的遍历
    auto it = addVec.begin();
    auto end = addVec.end();
    while(it!=end){
        // 日期
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it->substr(0,8))));
        // 时间
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(it->substr(8,8))));
        // 卡号
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it->substr(16,7))));
        // 消费金额
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(it->substr(23))));
        // 错误原因
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(codec->toUnicode("被增加")));
        i++;
        it++;
    }

    // 被删除字符容器的遍历
    auto it2 = deleteVec.begin();
    auto end2 = deleteVec.end();
    while(it2!=end2){
        // 日期
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it2->substr(0,8))));
        // 时间
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(it2->substr(8,8))));
        // 卡号
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it2->substr(16,7))));
        // 消费金额
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(it2->substr(23))));
        // 错误原因
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(codec->toUnicode("被删除")));
        i++;
        it2++;
    }
}


