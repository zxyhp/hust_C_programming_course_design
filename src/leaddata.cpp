#include "leaddata.h"

//===========================================================
// 函数名：LeadData
// 功能：LeadData类的构造函数，完成成员变量m_sortedOp、m_opration
//      的初始化，并对食堂窗口的消费记录完成排序并写入数据文件
// 输入参数：空
// 返回值：空
//===========================================================
LeadData::LeadData()
{
    // 初始化m_sortedOp (存放排序后的批量操作)
    this->m_sortedOp = new xfStr;
    this->m_sortedOp->next = NULL;

    this->init_m_opration();
}

//===========================================================
// 函数名：runTime
// 功能：计算传入函数的运行时间并显示在控制台窗口中
// 输入参数：LeadData类的函数指针
// 返回值：空
//===========================================================
void LeadData::runTime(LeadData::Func f)
{
    clock_t startTime = clock();
    (this->*f)();
    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// 函数名：initStu
// 功能：根据同目录下students.txt文件内的学生信息，将学生开户，信息
//      导入到学生信息系统的students容器中
// 输入参数：空
// 返回值：空
//===========================================================
void LeadData::initStu()
{
    // 在控制台窗口输出所在函数的函数名
    cout<<__FUNCTION__<<endl;

    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();

    char ch[MAXSIZE];
    string filePath = QDir::currentPath().toStdString() + "/students.txt";
    ifstream fp(filePath);
    if(!fp) {
        cout<<"students.txt打开失败!"<<endl;
        return;
    }
    string str;
    string stuNumber;
    string name;
    string dtStr;
    while(fp.getline(ch,MAXSIZE)){
        str = string(ch,strlen(ch)-1);
        stuNumber = str.substr(0,10);
        name = str.substr(11);
        dtStr = DT->toString("yyyyMMdd hhmmsszzz").toStdString();
        CO->createStu(dtStr.substr(0,8),dtStr.substr(9,8),stuNumber,name);
    }
    fp.close();
}

//===========================================================
// 函数名：init_m_opration
// 功能：初始化成员变量m_opration
// 输入参数：空
// 返回值：空
//===========================================================
void LeadData::init_m_opration()
{
    stringstream stream;
    string str = "";

    // 初始化记录 来自1-99窗口(0-98)的消费操作 和 对于校园卡(99)的操作 的链表记录的表头
    for(int i=0;i<WINNUM+1;i++){
        this->m_opration[i] = new xfStr;
        this->m_opration[i]->next = NULL;
        stream << i+1;
        stream >> str;
        // 表头的str处记录当前链表操作属于哪个窗口
        this->m_opration[i]->str = str;
        stream.clear();
        str.clear();
    }
}

//===========================================================
// 函数名：getOpration
// 功能：从xf.txt和cz.txt文件中获取系列操作语句
// 输入参数：空
// 返回值：空
//===========================================================
void LeadData::getOpration()
{
    // 输出当前函数名称
    cout<<__FUNCTION__<<endl;

    // 读取窗口的消费记录
    string filePath = QDir::currentPath().toStdString() + "/xf.txt";
    ifstream fp(filePath);
    char ch[MAXSIZE];
    if(!fp){
        cout<<"文件xf.txt打开失败!"<<endl;
        return;
    }
    // 待排路数序号标志
    int num = -1;
    string numStr;
    // 尾指针
    xfStr* hailp = NULL;
    while(fp.getline(ch,MAXSIZE)){
        // 如果读到W1、W2等就是新路数
        if(strlen(ch) <= 5) {
            // 更新序号
            num++;
            hailp = this->m_opration[num];
            numStr = hailp->str;
            continue;
        }
        // 新指针
        xfStr* p = new xfStr;
        p->str = string(ch,strlen(ch)-1)+","+ numStr + ";\n";
        p->next = NULL;
        // 尾插法
        hailp->next = p;
        hailp = p;
    }
    fp.close();

    // 读取卡片操作
    filePath = QDir::currentPath().toStdString() + "/cz.txt";
    fp.open(filePath);
    hailp = m_opration[99];
    string str;
    while(fp.getline(ch,MAXSIZE)){
        xfStr* p = new xfStr;
        str = string(ch,strlen(ch)) + "\n";
        // 卡片操作语句前加入八个空格符，方便排序操作中日期时间字符串的获取
        p->str = "        " + str.substr(0,8)+","+str.substr(8);
        p->next = NULL;
        hailp->next = p;
        hailp = p;
    }
    fp.close();
}

//===========================================================
// 函数名：insert_vec
// 功能：利用折半插入排序，每次插入一个元素到有序序列中(按照str升序)
// 输入参数：窗口号-1，日期时间字符串，有序待插入容器
// 返回值：空
//===========================================================
void LeadData::insert_vec(int i, string str, vector<pair<int, string> > &vec)
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
// 函数名：sortOpration
// 功能：将所有操作利用归并排序和折半插入排序根据时间的先后进行排序，将
//      排序后的结果存入到成员变量m_sortedOp所指的链表中
// 输入参数：空
// 返回值：空
//===========================================================
void LeadData::sortOpration()
{
    // 输出当前函数名称
    cout<<__FUNCTION__<<endl;

    // 存放从每路待排序列获取的一个元素 <来自路数的序号，字符串>
    vector<pair<int,string> > vec;

    // i代表窗口序号 0-98 99表示卡片操作
    for(int i=0;i<WINNUM+1;i++){
        if(this->m_opration[i]->next!=NULL){
            // 利用substr(8,17)获取字符串中的时间日期(排序标准)
            // insert_vec是将获取的元素有序插入到vec中
            insert_vec(i,this->m_opration[i]->next->str.substr(8,17),vec);
        }
    }

    // vec内元素个数为0时，跳出循环（即排序完成）
    // vector<pair<int,string> > vec内元素是按pair中的string递增排序的
    xfStr* p = NULL;
    xfStr* hailSp = this->m_sortedOp;
    while(vec.size() != 0){
        // 获取到最小元素对应的窗口序号
        int dir = vec[0].first;
        p = this->m_opration[dir];
        // 从vec中删去获取到的元素
        vec.erase(vec.begin());

        // 获取完整的记录语句 并插入到有序操作的队尾
        xfStr *Sp = new xfStr;
        Sp->str = p->next->str;
        Sp->next = NULL;
        hailSp->next = Sp;
        hailSp = Sp;

        // 从待操作记录链表中删去已经排序完成的记录对应的结点
        p->next = p->next->next;

        // 如果该窗口序号下还有元素待排，则继续插入到vec中
        if(p->next != NULL){
            insert_vec(dir,p->next->str.substr(8,17),vec);
        }
    }
}

//===========================================================
// 函数名：carryOutOp
// 功能：将排序后的操作在学生管理和食堂应用系统中进行从首至尾的实施
// 输入参数：空
// 返回值：最后一条操作的<日期，时间>
//===========================================================
pair<string,string> LeadData::carryOutOp()
{
    cout<<__FUNCTION__<<endl;
    clock_t startTime = clock();

    auto CO = CardOpration::getinstance();
    auto COP = CanteenOpration::getinstance();

    string str;
    float money;
    int winNum;
    vector<string> res;
    stringstream stream;
    xfStr *p = this->m_sortedOp;
    string date;
    string time;

    while(p->next!=NULL){
        p = p->next;
        // 获取操作语句
        str = p->str;
        str = str.substr(0,str.size()-2);
        // 3193355,20211005,07001172,8.25,56 (size:30)消费语句样例
        // 20210903,08315316,挂失,2020760664 挂失、解挂、补卡、销户语句样例(前面还有八个空格符)
        // 20211014,00012813,充值,2020030309,800 充值语句样例(前面还有八个空格符)
        switch(str.size()){
        case 32:
        case 33:
        case 34:
            // 消费操作
            res = COP->split(str,",");
            stream << res[3];
            stream >> money;
            stream.clear();
            stream << res[4];
            stream >> winNum;
            stream.clear();
            date = res[1];
            time = res[2];
            COP->consume(true,winNum-1,res[0],money,date,time);
            break;
        default:
            str = str.substr(8);
            res = COP->split(str,",");
            date = res[0];
            time = res[1];
            // 挂失、解挂、补卡、销户操作
            if(res[2] == "挂失"){
                CO->lossSolution_1(date,time,res[3]);
            }
            else if("解挂" == res[2]){
                CO->lossSolution_2(date,time,res[3]);
            }
            else if("补卡" == res[2]){
                CO->makeUpCard(date,time,res[3]);
            }
            else if("销户" == res[2]){
                CO->deleteStu(date,time,res[3]);
            }
            else if("充值" == res[2]){
                stream << res[4].substr(0,res[3].size()-2);
                stream >> money;
                stream.clear();
                CO->recharge(date,time,res[3],money);
            }
            break;
        }
    }

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;

    return *new pair<string,string>(date,time);
}

