#include "cardoprotion.h"

CardOpration *CardOpration::ptrCO = nullptr;

//===========================================================
// 函数名：CardOpration
// 功能：CardOpration类的构造函数，初始化成员变量cardNum(流水卡号)
// 输入参数：空
// 返回值：空
//===========================================================
CardOpration::CardOpration(){
    this->cardNum = "12346";
}

//===========================================================
// 函数名：stuExist
// 功能：判断传入学号是否存在于students中
// 输入参数：学号
// 返回值：判断结果，false则不存在，true则存在
//===========================================================
bool CardOpration::stuExist(string studentNum){
    if(this->students.find(studentNum) == this->students.end()){
        return false;
    }
    return true;
}

//===========================================================
// 函数名：stuLeave
// 功能：判断传入学号对应的学生状态
// 输入参数：学号
// 返回值：判断结果，true则已注销，false则状态正常
//===========================================================
bool CardOpration::stuLeave(string studentNum){
    if(this->students[studentNum].status) {
        return true;
    }
    return false;
}

//===========================================================
// 函数名：createStu
// 功能：实现学生的开户功能
// 输入参数：日期；时间；学号；姓名
// 返回值：空
//===========================================================
void CardOpration::createStu(string date,string time,string studentNum, string name){

    // 开户操作日志语句样例
    // date,time,开户,学号,姓名,成功
    // date,time,开户,学号,姓名,失败,失败原因
    string opStr = date+","+time+","+"开户,"+studentNum+","+name+",";

    // 如果该学号已经存在，则开户失败
    if(this->students.find(studentNum) != this->students.end()){
        opStr += "失败,学号已存在\n";
        this->oplog.writeOpLog(opStr);
        return;
    }

    // 创建一个学生
    Student *stu = new Student;
    stu->name = name;
    stu->stuNumber = studentNum;
    stu->money = 0;
    stu->status = 0;
    stu->password = "8888";  // 初始密码为8888
    stu->numOfCard = 0;
    stu->hasCard = false;
    stu->cardhead = new CardNum;  //初始化卡号链表头
    stu->cardhead->next = NULL;
    stu->cardhead->number = "";

    // 将该学生加入到学生信息类的students容器中
    this->students[studentNum] = *stu;

    // 写入开户成功的操作日志
    opStr += "成功\n";
    this->oplog.writeOpLog(opStr);

    // 开户成功后对新用户发卡
    this->putUpCards(date,time,studentNum);
}

//===========================================================
// 函数名：deleteStu
// 功能：实现学生的销户功能
// 输入参数：日期；时间；学号
// 返回值：空
//===========================================================
void CardOpration::deleteStu(string date, string time, string studentNum){
    // 销户操作日志语句样例
    // date,time,销户,学号,成功
    // date,time,销户,学号,失败,失败原因
    string opStr = date+","+time+","+"销户"+","+studentNum+",";

    // 学生是否存在于系统中
    if(!this->stuExist(studentNum)){
        opStr += "失败,用户不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经销户
    if(this->stuLeave(studentNum)) {
        opStr += "失败,用户已注销\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 获取学生的最新卡号
    string cardNum = this->students[studentNum].cardhead->next->number;

    // 若待销户的学生的名下还有使用中的卡 则冻结(挂失)
    if(!this->cards[cardNum].status){
        this->cards[cardNum].status = 1;
        this->students[studentNum].hasCard = false;
        string opStr2 = date+","+time+","+"挂失"+","+studentNum+","+cardNum+",成功\n";
        oplog.writeOpLog(opStr2);
    }

    this->students[studentNum].status = 1; // 调整状态即为销户
    opStr += "成功\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：addCard
// 功能：向对应学号的学生名下添加一张卡片，并将该卡添加到学生信息系统的
//      cards容器中
// 输入参数：学号
// 返回值：被发放卡片的卡号
//===========================================================
string CardOpration::addCard(string studentNum){
    // 初始化一张卡的基本信息
    Card *card = new Card;
    card->stuNumber = studentNum;
    card->name = this->students[studentNum].name;
    card->status = 0;

    // 获取卡号校验码
    int X = 3;
    for(int i=0;i<5;i++){
        X += this->cardNum[i]-'0';
    }
    X = 9 - X%10;

    //将X转换为string类型
    stringstream sStream;
    sStream << X;
    string strX;
    sStream >> strX;

    // 卡号
    card->cardNumber = '3' + this->cardNum + strX;

    // 将发放的卡加入到系统中
    this->cards[card->cardNumber] = *card;

    this->students[studentNum].numOfCard++;
    // 将卡号加入到学生信息中的卡号链表中
    CardNum *p = new CardNum;
    p->number = card->cardNumber;
    p->next = this->students[studentNum].cardhead->next;
    this->students[studentNum].cardhead->next = p;

    // 发卡后 该学生名下就有了正在使用中的卡
    this->students[studentNum].hasCard = true;

    // 更新下一张加入的卡的卡号
    sStream.clear();
    sStream<<this->cardNum;
    int num=0;
    sStream>>num;
    num++;
    sStream.clear();
    sStream<<num;
    sStream>>this->cardNum;

    // 返回所发卡的卡号
    return card->cardNumber;
}

//===========================================================
// 函数名：putUpCards
// 功能：对新开户的学生进行发卡的功能
// 输入参数：日期；时间；学号
// 返回值：空
//===========================================================
void CardOpration::putUpCards(string date, string time, string studentNum){
    // 发卡操作日志语句样例
    // date,time,发卡,学号,卡号,成功
    // date,time,发卡,学号,失败,失败原因
    string opStr = date+","+time+","+"发卡"+","+studentNum+",";

    // 判断学生是否已经开户
    if(!this->stuExist(studentNum)) {
        opStr += "失败,用户不存在\n";
        this->oplog.writeOpLog(opStr);
        return;
    }
    string cardNum = this->addCard(studentNum);
    opStr += cardNum + ",成功\n";
    this->oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：lossSolution_1
// 功能：对学生校园卡的挂失功能
// 输入参数：日期；时间；学号
// 返回值：空
//===========================================================
void CardOpration::lossSolution_1(string date, string time, string studentNum){

    // 挂失操作日志语句样例
    // date,time,挂失,学号,卡号,成功
    // date,time,挂失,学号,失败,失败原因
    string opStr = date+","+time+","+"挂失"+","+studentNum+",";

    // 判断学生是否已经开户
    if(!this->stuExist(studentNum)) {
        opStr += "失败,用户不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经销户
    if(this->stuLeave(studentNum)) {
        opStr += "失败,用户已注销\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 若该用户名下没有正在使用中的卡
    if(!this->students[studentNum].hasCard){
        opStr += "失败,用户所有卡均已挂失\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 获取该学生最新的校园卡卡号
    string cardNum = this->students[studentNum].cardhead->next->number;
    this->students[studentNum].hasCard = false;
    this->cards[cardNum].status = 1;
    opStr += cardNum + ",成功\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：lossSolution_2
// 功能：对学生校园卡的解挂功能
// 输入参数：日期；时间；学号
// 返回值：空
//===========================================================
void CardOpration::lossSolution_2(string date, string time, string studentNum,string cardNum){
    // 解挂操作日志语句样例
    // date,time,解挂,学号,卡号,成功
    // date,time,解挂,学号,失败,失败原因
    string opStr = date+","+time+","+"解挂"+","+studentNum+",";

    // 判断学生是否已经开过户
    if(!this->stuExist(studentNum)){
        opStr += "失败,用户不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经销户
    if(this->stuLeave(studentNum)){
        opStr += "失败,用户已注销\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 若学生名下已经有正在使用中的卡则 解挂失败
    if(this->students[studentNum].hasCard){
        opStr += "失败,用户已有卡处于使用状态\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 获取该学生最新的校园卡卡号
    cardNum = this->students[studentNum].cardhead->next->number;
    this->cards[cardNum].status = 0;
    this->students[studentNum].hasCard = true;
    opStr += cardNum + ",成功\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：makeUpCard
// 功能：对学生校园卡的补卡功能
// 输入参数：日期；时间；学号
// 返回值：空
//===========================================================
void CardOpration::makeUpCard(string date, string time, string studentNum){
    // 补卡操作日志语句样例
    // date,time,补卡,学号,卡号,成功
    // date,time,补卡,学号,失败,失败原因
    string opStr = date+","+time+","+"补卡"+","+studentNum+",";

    // 判断学生是否已经开户
    if(!this->stuExist(studentNum)){
        opStr += "失败,用户不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经销户
    if(this->stuLeave(studentNum)){
        opStr += "失败,用户已注销\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经补卡超过100次
    if(this->students[studentNum].numOfCard>100){
        opStr += "失败,用户补卡次数已超上限\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 获取该学生最新的校园卡卡号
    string cardNum = this->students[studentNum].cardhead->next->number;

    // 若最新的卡还在使用中，则对其进行挂失
    if(cards[cardNum].status == 0){
        cards[cardNum].status = 1;
        this->students[studentNum].hasCard = false;
        string opStr2 = date+","+time+","+"挂失"+","+studentNum+","+cardNum+",成功\n";
        oplog.writeOpLog(opStr2);
    }

    string newCardNum = this->addCard(studentNum);
    opStr += newCardNum + ",成功\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// 函数名：recharge
// 功能：对学生校园卡的充值功能
// 输入参数：日期；时间；学号；充值金额
// 返回值：空
//===========================================================
void CardOpration::recharge(string date, string time, string studentNum, float addMoney){
    // 充值操作日志语句样例
    // date,time,充值,学号,充值前余额,充值金额,充值后余额,成功
    // date,time,充值,学号,充值前余额,充值金额,失败,失败原因
    // date,time,充值,学号,失败,失败原因
    string opStr = date+","+time+","+"充值"+","+studentNum+",";

    // 判断学生是否已经开户
    if(!this->stuExist(studentNum)){
        opStr += "失败,用户不存在\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // 判断学生是否已经销户
    if(this->stuLeave(studentNum)){
        opStr += "失败,用户已注销\n";
        oplog.writeOpLog(opStr);
        return;
    }

    string moneyStr;
    float money = this->students[studentNum].money;
    stringstream sStream;

    // 对日志语句增加充值前余额
    sStream <<fixed<<setprecision(2)<< money;
    sStream >> moneyStr;
    sStream.clear();
    opStr += moneyStr +",";
    // 对日志语句增加充值金额
    sStream <<fixed<<setprecision(2)<< addMoney;
    sStream >> moneyStr;
    sStream.clear();
    opStr += moneyStr +",";

    if(money + addMoney >= MAXMONEY){
        opStr += "失败,超出充值上限金额\n";
        oplog.writeOpLog(opStr);
    }
    else{
        this->students[studentNum].money += addMoney;
        sStream <<fixed<<setprecision(2)<< money + addMoney;
        sStream >> moneyStr;
        sStream.clear();
        opStr += moneyStr +",成功\n";
        oplog.writeOpLog(opStr);
    }
}
