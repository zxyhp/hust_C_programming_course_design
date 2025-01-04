#include "summarizedialog.h"
#include "ui_summarizedialog.h"
#include <QDir>

#define Y 24
#define S 18
#define F 5
#define KEYLENGTH 3
#define ALLLENGTH 26 // 把.看作10

#include <QMessageBox>
#include <QTextCodec>
#include <ctime>

//===========================================================
// 函数名：SummarizeDialog
// 功能：SummarizeDialog类的构造函数，完成成员变量m_COP的初始化，
//      并对食堂窗口的消费记录完成排序并写入数据文件
// 输入参数：食堂应用操作类的指针；父窗口部件的指针（没有父窗口则会传
//          入nullptr）
// 返回值：空
//===========================================================
SummarizeDialog::SummarizeDialog(CanteenOpration* COP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SummarizeDialog)
{
    ui->setupUi(this);

    m_COP = COP;

    string tipStr = "消费数据汇总排序花费时间：";

    this->init();

    clock_t startTime = clock();
    this->sortWinXf(this->m_allReacords);
    clock_t endTime = clock();

    this->writeWinXf();

    // 花费时间字符串
    stringstream ss;
    string timeStr;
    ss<<fixed<<setprecision(3)<<double(endTime - startTime)/CLOCKS_PER_SEC;
    ss>>timeStr;

    tipStr += timeStr + "s\n(不包含数据文件的写入时间)";

    // 弹窗显示花费时间
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QMessageBox::information(this,codec->toUnicode("提示"),
                         codec->toUnicode(tipStr.c_str()));

}

//===========================================================
// 函数名：~SummarizeDialog
// 功能：SummarizeDialog类的析构函数，在SummarizeDialog类的对象
//      释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
SummarizeDialog::~SummarizeDialog()
{
    delete ui;
}

//===========================================================
// 函数名：on_backButton_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_statistButton_clicked
// 功能：该类窗口界面上的统计按钮的click()信号的槽函数，显示统计窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::on_statistButton_clicked()
{
    StatisticsDialog *SD = new StatisticsDialog(this->m_sortedRecords);
    SD->exec();
}

//===========================================================
// 函数名：init
// 功能：初始化存放99个窗口消费记录信息入口的容易，便于后续排序
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::init()
{
    cout<<"init"<<endl;
    clock_t startTime = clock();

    winXfHead hp;
    pair<int,winXfStr*> p;
    for(int i=0;i<WINNUM;i++){
        hp = this->m_COP->getXfRecords(i);
        if(hp.num == 0) continue;
        p.first = hp.num;
        p.second = hp.now->previous;
        this->m_allReacords[i] = p;
    }

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：insert_vec
// 功能：利用折半插入排序，每次插入一个元素到有序序列中(按照str升序)
// 输入参数：窗口号-1，日期时间字符串，有序待插入容器
// 返回值：空
//===========================================================
void SummarizeDialog::insert_vec(int i, string str, vector<pair<int, string> > &vec)
{
    int low = 0;
    int high = vec.size() - 1;
    int mid;
    while(low<=high){
        mid = (low+high)/2;
        if(vec[mid].second < str){
            low = mid + 1;
        }
        else high = mid - 1;
    }
    pair<int,string> p(i,str);
    vec.insert(vec.begin()+high+1,p);
}

//===========================================================
// 函数名：sortWinXf
// 功能：用多路归并将传入参数内的信息按日期时间进行排序，并将排序结果存进
//      m_sortedRecords为头节点的链表中
// 输入参数：待排序的多路信息的键值对集合
// 返回值：空
//===========================================================
void SummarizeDialog::sortWinXf(map<int,pair<int,winXfStr*>> allReacords)
{
    cout<<"sortWinXf"<<endl;
    clock_t startTime = clock();

    this->m_sortedRecords = new sortedXf;

    // 存放从每路待排序列获取的一个元素 <来自路数的序号，字符串>
    vector<pair<int,string> > vec;

    // 获取有效消费操作的最后一条
    auto end = allReacords.end();
    winXfStr *xf = nullptr;
    for(auto it = allReacords.begin();it!=end;it++){
        xf = it->second.second;
        insert_vec(it->first,xf->date+xf->time,vec);
    }

    // 头节点的日期记录整个有序记录中最大的日期
    if(vec.size()!=0)
        this->m_sortedRecords->date = (vec.end()-1)->second;

    // 当lines[99]中每一个元素都是0的时候退出循环（即排序完成）
    // vector<pair<int,string> > vec内元素是按pair中的string递减排序的
    winXfStr* p = nullptr;
    sortedXf* sp = nullptr;
    while(vec.size() != 0){
        // 获取到最大元素对应的窗口序号
        int dir = (vec.end()-1)->first;
        p = allReacords[dir].second;
        // 从vec中删去获取到的元素
        vec.erase(vec.end()-1);

        // 获取完整的记录语句 并插入到有序操作的队头
        sp = new sortedXf(p->cardNum,p->stuNum,p->date,p->time,p->money,p->name,dir+1);
        sp->next = this->m_sortedRecords->next;
        this->m_sortedRecords->next = sp;

        allReacords[dir].first--;
        allReacords[dir].second = p->previous;
        // 如果该窗口序号下还有元素待排，则继续插入到vec中
        if(allReacords[dir].first!=0){
            insert_vec(dir,p->previous->date+p->previous->time,vec);
        }
    }

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：writeWinXf
// 功能：将有序链表中的信息结合成语句，并加密后写入数据文件all_xf.txt中
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::writeWinXf()
{
    cout<<"writeWinXf"<<endl;
    clock_t startTime = clock();

    sortedXf *sp = this->m_sortedRecords; // 有序链表的索引指针
    // 初始化明文语句链表的头节点
    this->m_sortXfStr = new xfStr;

    // 辅助变量
    string moneyStr; // 消费金额语句
    string xfStrTrue; //　写入文件记录的语句
    string infoStr=""; // 写入all_xf.txt文件的内容
    stringstream ss; // 字符串数字转换流
    auto p = this->m_sortXfStr;

    qsrand(time(NULL));
    // 写入密文信息的同时生成明文语句链表
    while(sp->next!=nullptr){
        sp=sp->next;
        // 得到字符串型的消费金额
        ss << fixed<<setprecision(2)<<sp->money;
        ss >> moneyStr;
        ss.clear();
        // 明文语句
        xfStrTrue = sp->date+sp->time+sp->cardNum+moneyStr;
        // 将明文语句加入链表中
        p->next = new xfStr(xfStrTrue);
        p=p->next;

        // 计算出校验码
        int num = qrand()%3;
        char ch = 'a' + num;
        infoStr += ch + getCheckStr(num,xfStrTrue)+"\n";
    }

    // 将内容写进all_xf.txt文件中
    string filePath = QDir::currentPath().toStdString() + "/all_xf.txt";
    ofstream fp(filePath);
    fp << infoStr;
    fp.close();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：getCheckStr
// 功能：对传入的字符串进行加密，并返回结果
// 输入参数：加密的起始序号；待加密的语句
// 返回值：加密后的字符串
//===========================================================
string SummarizeDialog::getCheckStr(int num,string str)
{
    // 存放加密后的字符串
    string checkStr = "";
    // 密钥
    static int key[]={Y,S,F}; // key[0]=24,key[1]=18,key[2]=5
    // 辅助变量
    char ch;
    // 遍历字符串
    auto it = str.begin();
    auto end = str.end();
    while(it!=end){
        // 如果是'.'则对应10，'0'-'9'对应0-9
        if(*it=='.') ch = (10+key[num])%ALLLENGTH+'a';
        else ch = (*it-'0'+key[num])%ALLLENGTH+'a';

        checkStr += ch;
        num = (num+1)%KEYLENGTH;
        it++;
    }
    // 返回加密后的字符串
    return checkStr;
}

//===========================================================
// 函数名：on_pushButton_clicked
// 功能：该类窗口界面上的查询按钮的click()信号的槽函数，显示查询窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::on_pushButton_clicked()
{
    SearchDialog *SD = new SearchDialog(this->m_sortedRecords);
    SD->exec();
}

//===========================================================
// 函数名：on_analyzeButton_clicked
// 功能：该类窗口界面上的分析按钮的click()信号的槽函数，显示分析窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::on_analyzeButton_clicked()
{
    AnalyzeDialog* AD = new AnalyzeDialog(this->m_allReacords,this->m_sortedRecords);
    AD->exec();
}

//===========================================================
// 函数名：on_checkButton_clicked
// 功能：该类窗口界面上的校验按钮的click()信号的槽函数，显示校验窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void SummarizeDialog::on_checkButton_clicked()
{
    CheckFileDialog CFD(this->m_sortXfStr);
    CFD.exec();
}
