#include "canteenopration.h"

#include <QDir>

CanteenOpration *CanteenOpration::ptrCOP = nullptr;

//===========================================================
// 函数名：CanteenOpration
// 功能：CanteenOpration类的构造函数，初始化部分成员变量
// 输入参数：空
// 返回值：空
//===========================================================
CanteenOpration::CanteenOpration()
{
    // 初始化窗口 结合wz.txt文件
    this->initWin();

    // 初始化m_winXfRecords容器（存放99*60000条记录的容器）
    this->initWinXfRecords();
}

//===========================================================
// 函数名：split
// 功能：以delim为间隔，将传入的str字符串拆分成若干个字符串
// 输入参数：待切割的字符串；切割字符串的分隔字符串
// 返回值：由str切割而成的若干个字符串的集合
//===========================================================
vector<string> CanteenOpration::split(const string &str, const string &delim)
{
    vector<string> res;
    // 如果str为空则返回空容器
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.size() + 1];
    strcpy(strs, str.c_str());
    char *d = new char[delim.size() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs,d);
    while(p){
        string s = p;
        res.push_back(s);
        p=strtok(NULL,d);
    }
    return res;
}

//===========================================================
// 函数名：consume
// 功能：实现传入卡号对应学生的消费操作
// 输入参数：是否为批量操作的标志；窗口号-1；卡号；消费金额；日期；时间
// 返回值：空
//===========================================================
void CanteenOpration::consume(bool isPl, int winNum, string cardNum, float conMoney, string date, string time)
{
    // 获取卡片操作的指针 内部包含学生和卡片的所有信息
    auto CO = CardOpration::getinstance();

    // 操作日志对象 用于后序写入操作的日志
    OprationLog oplog;

    // 声明ss变量用于后续字符串和数字的转换
    stringstream ss;

    // 获取窗口号的字符串形式，写入winNumStr
    ss << winNum+1;
    string winNumStr;
    ss >> winNumStr;
    ss.clear();

    // 消费操作日志语句样例
    // date,time,消费,卡号,消费窗口,学号,消费余额,消费金额,消费后余额,成功
    // date,time,消费,卡号,消费窗口,学号,消费余额,消费金额,失败,失败原因
    // date,time,消费,卡号,消费窗口,失败,失败原因
    string opStr = date+","+time+","+"消费"+","+cardNum+","+winNumStr+",";

    // 先根据传入数据更新日期时间
    if(this->updateTime(isPl,date,time) == false){
        opStr += "失败,消费时间不能回退\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 如果是个人消费 isPl==false
    if(!isPl && this->m_timeFrame == 4){
        opStr += "失败,未到食堂消费时段\n";
        oplog.writeOpLog(opStr);
        return;
    }

    //判断卡号是否存在
    if(CO->cards.find(cardNum) != CO->cards.end()){
        // 判断卡号是否处于使用状态
        if(CO->cards[cardNum].status == true){
            opStr += "失败,校园卡已挂失\n";
            oplog.writeOpLog(opStr);
            return;
        }
    }
    else{
        opStr += "失败,校园卡不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 根据卡号索引到学生号
    string stuNum = CO->cards[cardNum].stuNumber;

    // 向opStr添加学号
    opStr += stuNum + ",";

    // 用于存储一些金额的字符串形式
    string moneyStr;

    // money为账户余额
    float money = CO->students[stuNum].money;

    // 向opStr添加账户消费钱余额
    ss << fixed << setprecision(2) <<money;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",";

    // 向opStr添加账户消费金额
    ss << fixed << setprecision(2) <<conMoney;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",";

    // 如果账户余额小于消费金额
    if(money < conMoney){
        opStr += "失败,账户余额不足\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 如果是个人消费则需要考虑密码问题
    if(!isPl){
        // 判断该卡号在该时段是否消费过
        // 若尚未消费过 则创建容器元素赋0值
        if(m_csMoneyInFrame.find(stuNum) == m_csMoneyInFrame.end()) {
            m_csMoneyInFrame[stuNum] = 0;
        }

        // 若该时段消费金额大于20 则需要输入密码
        if(m_csMoneyInFrame[stuNum] + conMoney > 20) {

            // 密码输入窗口
            PasswordDialog* PD = new PasswordDialog;
            // 连接槽 如果输入密码并确认 则 this->passWord就会获取到输入的密码
            connect(PD,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
            PD->exec();

            // 如果密码输入错误
            if(this->m_password.toStdString() != CO->students[stuNum].password){
                opStr += "失败,密码输入有误\n";
                oplog.writeOpLog(opStr);
                return;
            }

            // 如果密码输入正确 时段消费金额零
            m_csMoneyInFrame[stuNum] = 0;
        }
        // 若该时段消费金额小于等于20 则更新时段消费金额
        else{
            m_csMoneyInFrame[stuNum] += conMoney;
        }
    }

    // 至此一下表示消费成功的操作

    // 窗口当日总消费次数和总金额增加
    this->win[winNum].consumeTimes++;
    this->win[winNum].consumeMoney += conMoney;

    // 消费成功后将消费记录加入到消费目录中
    // 将其填入到窗口消费记录中
    this->writeXfRecords(winNum,cardNum,stuNum,date,time,conMoney);

    // 账户余额减去消费金额
    CO->students[stuNum].money -= conMoney;

    // 向opStr添加账户消费后余额
    ss << fixed << setprecision(2) <<money - conMoney;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",成功\n";

    // 写入操作日志语句
    oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：updateTFrame
// 功能：根据传入的时间更新类的时段变量m_timeFrame
// 输入参数：更新后的新时间
// 返回值：空
//===========================================================
void CanteenOpration::updateTFrame(int time){
    if(time>=7000000 && time<=9000000)
        this->m_timeFrame = 1; // 7点到9点早饭时间
    else if(time>=11000000 && time<=13000000)
        this->m_timeFrame = 2; // 11点到13点午饭时间
    else if(time>=17000000 && time<=19000000)
        this->m_timeFrame = 3; // 17点到19点晚饭时间
    else this->m_timeFrame = 4;
}

//===========================================================
// 函数名：writeXfRecords
// 功能：将一条消费记录信息写入到对应窗口的消费记录链表的对应节点中
// 输入参数：窗口号-1；卡号；学号；日期；时间；消费金额
// 返回值：空
//===========================================================
void CanteenOpration::writeXfRecords(int winNum,string cardNum
             ,string stuNum,string date,string time,float conMoney)
{
    // 获取信息的写入节点的指针
    winXfStr *p = this->m_winXfRecords[winNum].now;
    // 写入相应信息
    p->cardNum = cardNum;
    p->stuNum = stuNum;
    p->date = date;
    p->time = time;
    p->money = conMoney;
    p->name = CardOpration::getinstance()->students[stuNum].name;
    // 更新入口中保存的最新写入节点
    this->m_winXfRecords[winNum].now = p->next;
    // 如果记录条数没有大于最大跳数 则记录条数加一
    if(this->m_winXfRecords[winNum].num != FILELINES)
        this->m_winXfRecords[winNum].num++;
    // 更新入口中保存的最新写入节点对应的行数
    int lineNum = this->win[winNum].fileLine;
    this->win[winNum].fileLine = (lineNum+1)%FILELINES;
}

//===========================================================
// 函数名：getWinConsume
// 功能：用于类外获取私有成员变量窗口中记录的消费总次数和消费总金额
// 输入参数：窗口号-1
// 返回值：<窗口的当日总消费次数，窗口的当日总消费金额>
//===========================================================
pair<int, float> CanteenOpration::getWinConsume(int winnum)
{
    pair<int, float> p(this->win[winnum].consumeTimes,this->win[winnum].consumeMoney);
    return p;
}

//===========================================================
// 函数名：getWinLine
// 功能：用于类外获取私有成员变量窗口的新写入行数
// 输入参数：窗口号-1
// 返回值：窗口消费记录的下一条记录的写入行数
//===========================================================
int CanteenOpration::getWinLine(int winnum)
{
    return this->win[winnum].fileLine;
}

//===========================================================
// 函数名：getXfRecords
// 功能：用于类外获取某个窗口的消费记录的入口
// 输入参数：窗口号-1
// 返回值：窗口的消费记录的入口变量
//===========================================================
winXfHead CanteenOpration::getXfRecords(int num)
{
    return this->m_winXfRecords[num];
}

//===========================================================
// 函数名：receiveData
// 功能：用于接收密码输入窗口中输入框内输入的字符串至成员变量m_password中
// 输入参数：密码输入窗口中输入框内输入的字符串
// 返回值：空
//===========================================================
void CanteenOpration::receiveData(QString data)
{
    this->m_password = data;
}

//===========================================================
// 函数名：updateTime
// 功能：更新成员变量m_date和m_time
// 输入参数：是否为批量操作的标志；新日期；新时间
// 返回值：日期时间更新成功与否
//===========================================================
bool CanteenOpration::updateTime(bool isPl, string date, string time)
{
    // 如果新日期小于当前记录日期 则更新失败
    if(this->m_date > date) return false;
    // 如果新日期等于当前记录日期
    else if(this->m_date == date){
        // 如果新时间小于当前记录时间 则更新失败
        if(this->m_time > time) return false;

        // 如果新时间大于等于当前记录时间
        else{
            // 更新时间
            this->m_time = time;

            // 如果是个人操作
            if(!isPl){
                // 获取int型的新时间
                stringstream stream;
                int newTime;
                stream << time;
                stream >> newTime;
                stream.clear();

                // 更新时段前的旧时段
                int oldTFrame = this->m_timeFrame;
                this->updateTFrame(newTime);
                // 判断是否跨时段（跨时段则需更新m_csMoneyInFrame）
                if(oldTFrame!=this->m_timeFrame)
                    m_csMoneyInFrame.clear();
            }
        }
    }
    // 如果新日期大于当前记录日期 则更新日期
    else{
        // 更新日期则需要将所有窗口的当日消费次数和消费总金额清零
        this->updateWin();
        // 更新日期和时间
        this->m_date = date;
        this->m_time = time;

        // 如果是个人操作
        if(!isPl){
            // 获取int型的新时间
            stringstream stream;
            int newTime;
            stream << time;
            stream >> newTime;
            stream.clear();

            // 更新时段和csMoneyInFrame容器
            this->updateTFrame(newTime);
            m_csMoneyInFrame.clear();
        }
    }

    return true;
}

//===========================================================
// 函数名：initWinXfRecords
// 功能：初始化成员变量m_winXfRecords[0-98]（99个窗口的消费记录入口）
// 输入参数：空
// 返回值：空
//===========================================================
void CanteenOpration::initWinXfRecords()
{
    // 给每个窗口初始化一个60000个结点的双向循环链表
    for(int i=0;i<WINNUM;i++){
        // 当前建立的链表的入口对象
        winXfHead hp;

        // 双向循环链表的建立
        winXfStr *head = new winXfStr;
        winXfStr *p1 = head; // 前一个结点(相对p2)
        winXfStr *p2 = nullptr; // 后一个节点(相对p1)
        for(int j=0;j<FILELINES-1;j++){
            p2 = new winXfStr;
            p1->next = p2;
            p2->previous = p1;
            p1=p2;
        }
        // 链表的头尾相连
        p1->next = head;
        head->previous = p1;

        // 将链表入口的指针传给入口对象
        hp.now = head;
        // 向入口对象加入到m_winXfRecords中
        this->m_winXfRecords[i] = hp;
    }
}

//===========================================================
// 函数名：initWin
// 功能：初始化成员变量m_win[0-98]（99个窗口）
// 输入参数：空
// 返回值：空
//===========================================================
void CanteenOpration::initWin()
{
    // 辅助变量
    stringstream ss;
    vector<string> res;
    char ch[MAXSIZE];

    // 打开位置文件(wz.txt)读取每个记录开始储存的初始位置
    string filePath = QDir::currentPath().toStdString() + "/wz.txt";
    ifstream fp(filePath);
    for(int i=0;i<WINNUM;i++){
        // 初始消费次数和消费金额都为0
        this->win[i].consumeMoney = 0;
        this->win[i].consumeTimes = 0;
        // 从fp文件中获取窗口记录的初始记录位置（字符串型）
        fp.getline(ch,MAXSIZE);
        res = split(string(ch,strlen(ch)-1),",");
        // 将记录位置转为int型写入win[i].fileLine中
        ss << res[1];
        ss >> this->win[i].fileLine;
        ss.clear();
    }
    fp.close();
}

//===========================================================
// 函数名：updateWin
// 功能：对99个窗口的当日消费总金额和总次数清零
// 输入参数：空
// 返回值：空
//===========================================================
void CanteenOpration::updateWin()
{
    for(int i=0;i<WINNUM;i++){
        this->win[i].consumeMoney = 0;
        this->win[i].consumeTimes = 0;
    }
}


