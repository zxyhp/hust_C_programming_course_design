#include "cardoprotion.h"

CardOpration *CardOpration::ptrCO = nullptr;

//===========================================================
// ��������CardOpration
// ���ܣ�CardOpration��Ĺ��캯������ʼ����Ա����cardNum(��ˮ����)
// �����������
// ����ֵ����
//===========================================================
CardOpration::CardOpration(){
    this->cardNum = "12346";
}

//===========================================================
// ��������stuExist
// ���ܣ��жϴ���ѧ���Ƿ������students��
// ���������ѧ��
// ����ֵ���жϽ����false�򲻴��ڣ�true�����
//===========================================================
bool CardOpration::stuExist(string studentNum){
    if(this->students.find(studentNum) == this->students.end()){
        return false;
    }
    return true;
}

//===========================================================
// ��������stuLeave
// ���ܣ��жϴ���ѧ�Ŷ�Ӧ��ѧ��״̬
// ���������ѧ��
// ����ֵ���жϽ����true����ע����false��״̬����
//===========================================================
bool CardOpration::stuLeave(string studentNum){
    if(this->students[studentNum].status) {
        return true;
    }
    return false;
}

//===========================================================
// ��������createStu
// ���ܣ�ʵ��ѧ���Ŀ�������
// ������������ڣ�ʱ�䣻ѧ�ţ�����
// ����ֵ����
//===========================================================
void CardOpration::createStu(string date,string time,string studentNum, string name){

    // ����������־�������
    // date,time,����,ѧ��,����,�ɹ�
    // date,time,����,ѧ��,����,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"����,"+studentNum+","+name+",";

    // �����ѧ���Ѿ����ڣ��򿪻�ʧ��
    if(this->students.find(studentNum) != this->students.end()){
        opStr += "ʧ��,ѧ���Ѵ���\n";
        this->oplog.writeOpLog(opStr);
        return;
    }

    // ����һ��ѧ��
    Student *stu = new Student;
    stu->name = name;
    stu->stuNumber = studentNum;
    stu->money = 0;
    stu->status = 0;
    stu->password = "8888";  // ��ʼ����Ϊ8888
    stu->numOfCard = 0;
    stu->hasCard = false;
    stu->cardhead = new CardNum;  //��ʼ����������ͷ
    stu->cardhead->next = NULL;
    stu->cardhead->number = "";

    // ����ѧ�����뵽ѧ����Ϣ���students������
    this->students[studentNum] = *stu;

    // д�뿪���ɹ��Ĳ�����־
    opStr += "�ɹ�\n";
    this->oplog.writeOpLog(opStr);

    // �����ɹ�������û�����
    this->putUpCards(date,time,studentNum);
}

//===========================================================
// ��������deleteStu
// ���ܣ�ʵ��ѧ������������
// ������������ڣ�ʱ�䣻ѧ��
// ����ֵ����
//===========================================================
void CardOpration::deleteStu(string date, string time, string studentNum){
    // ����������־�������
    // date,time,����,ѧ��,�ɹ�
    // date,time,����,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"����"+","+studentNum+",";

    // ѧ���Ƿ������ϵͳ��
    if(!this->stuExist(studentNum)){
        opStr += "ʧ��,�û�������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ�����
    if(this->stuLeave(studentNum)) {
        opStr += "ʧ��,�û���ע��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ��ȡѧ�������¿���
    string cardNum = this->students[studentNum].cardhead->next->number;

    // ����������ѧ�������»���ʹ���еĿ� �򶳽�(��ʧ)
    if(!this->cards[cardNum].status){
        this->cards[cardNum].status = 1;
        this->students[studentNum].hasCard = false;
        string opStr2 = date+","+time+","+"��ʧ"+","+studentNum+","+cardNum+",�ɹ�\n";
        oplog.writeOpLog(opStr2);
    }

    this->students[studentNum].status = 1; // ����״̬��Ϊ����
    opStr += "�ɹ�\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// ��������addCard
// ���ܣ����Ӧѧ�ŵ�ѧ���������һ�ſ�Ƭ�������ÿ���ӵ�ѧ����Ϣϵͳ��
//      cards������
// ���������ѧ��
// ����ֵ�������ſ�Ƭ�Ŀ���
//===========================================================
string CardOpration::addCard(string studentNum){
    // ��ʼ��һ�ſ��Ļ�����Ϣ
    Card *card = new Card;
    card->stuNumber = studentNum;
    card->name = this->students[studentNum].name;
    card->status = 0;

    // ��ȡ����У����
    int X = 3;
    for(int i=0;i<5;i++){
        X += this->cardNum[i]-'0';
    }
    X = 9 - X%10;

    //��Xת��Ϊstring����
    stringstream sStream;
    sStream << X;
    string strX;
    sStream >> strX;

    // ����
    card->cardNumber = '3' + this->cardNum + strX;

    // �����ŵĿ����뵽ϵͳ��
    this->cards[card->cardNumber] = *card;

    this->students[studentNum].numOfCard++;
    // �����ż��뵽ѧ����Ϣ�еĿ���������
    CardNum *p = new CardNum;
    p->number = card->cardNumber;
    p->next = this->students[studentNum].cardhead->next;
    this->students[studentNum].cardhead->next = p;

    // ������ ��ѧ�����¾���������ʹ���еĿ�
    this->students[studentNum].hasCard = true;

    // ������һ�ż���Ŀ��Ŀ���
    sStream.clear();
    sStream<<this->cardNum;
    int num=0;
    sStream>>num;
    num++;
    sStream.clear();
    sStream<<num;
    sStream>>this->cardNum;

    // �����������Ŀ���
    return card->cardNumber;
}

//===========================================================
// ��������putUpCards
// ���ܣ����¿�����ѧ�����з����Ĺ���
// ������������ڣ�ʱ�䣻ѧ��
// ����ֵ����
//===========================================================
void CardOpration::putUpCards(string date, string time, string studentNum){
    // ����������־�������
    // date,time,����,ѧ��,����,�ɹ�
    // date,time,����,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"����"+","+studentNum+",";

    // �ж�ѧ���Ƿ��Ѿ�����
    if(!this->stuExist(studentNum)) {
        opStr += "ʧ��,�û�������\n";
        this->oplog.writeOpLog(opStr);
        return;
    }
    string cardNum = this->addCard(studentNum);
    opStr += cardNum + ",�ɹ�\n";
    this->oplog.writeOpLog(opStr);
}

//===========================================================
// ��������lossSolution_1
// ���ܣ���ѧ��У԰���Ĺ�ʧ����
// ������������ڣ�ʱ�䣻ѧ��
// ����ֵ����
//===========================================================
void CardOpration::lossSolution_1(string date, string time, string studentNum){

    // ��ʧ������־�������
    // date,time,��ʧ,ѧ��,����,�ɹ�
    // date,time,��ʧ,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"��ʧ"+","+studentNum+",";

    // �ж�ѧ���Ƿ��Ѿ�����
    if(!this->stuExist(studentNum)) {
        opStr += "ʧ��,�û�������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ�����
    if(this->stuLeave(studentNum)) {
        opStr += "ʧ��,�û���ע��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �����û�����û������ʹ���еĿ�
    if(!this->students[studentNum].hasCard){
        opStr += "ʧ��,�û����п����ѹ�ʧ\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ��ȡ��ѧ�����µ�У԰������
    string cardNum = this->students[studentNum].cardhead->next->number;
    this->students[studentNum].hasCard = false;
    this->cards[cardNum].status = 1;
    opStr += cardNum + ",�ɹ�\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// ��������lossSolution_2
// ���ܣ���ѧ��У԰���Ľ�ҹ���
// ������������ڣ�ʱ�䣻ѧ��
// ����ֵ����
//===========================================================
void CardOpration::lossSolution_2(string date, string time, string studentNum,string cardNum){
    // ��Ҳ�����־�������
    // date,time,���,ѧ��,����,�ɹ�
    // date,time,���,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"���"+","+studentNum+",";

    // �ж�ѧ���Ƿ��Ѿ�������
    if(!this->stuExist(studentNum)){
        opStr += "ʧ��,�û�������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ�����
    if(this->stuLeave(studentNum)){
        opStr += "ʧ��,�û���ע��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ��ѧ�������Ѿ�������ʹ���еĿ��� ���ʧ��
    if(this->students[studentNum].hasCard){
        opStr += "ʧ��,�û����п�����ʹ��״̬\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ��ȡ��ѧ�����µ�У԰������
    cardNum = this->students[studentNum].cardhead->next->number;
    this->cards[cardNum].status = 0;
    this->students[studentNum].hasCard = true;
    opStr += cardNum + ",�ɹ�\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// ��������makeUpCard
// ���ܣ���ѧ��У԰���Ĳ�������
// ������������ڣ�ʱ�䣻ѧ��
// ����ֵ����
//===========================================================
void CardOpration::makeUpCard(string date, string time, string studentNum){
    // ����������־�������
    // date,time,����,ѧ��,����,�ɹ�
    // date,time,����,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"����"+","+studentNum+",";

    // �ж�ѧ���Ƿ��Ѿ�����
    if(!this->stuExist(studentNum)){
        opStr += "ʧ��,�û�������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ�����
    if(this->stuLeave(studentNum)){
        opStr += "ʧ��,�û���ע��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ���������100��
    if(this->students[studentNum].numOfCard>100){
        opStr += "ʧ��,�û����������ѳ�����\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ��ȡ��ѧ�����µ�У԰������
    string cardNum = this->students[studentNum].cardhead->next->number;

    // �����µĿ�����ʹ���У��������й�ʧ
    if(cards[cardNum].status == 0){
        cards[cardNum].status = 1;
        this->students[studentNum].hasCard = false;
        string opStr2 = date+","+time+","+"��ʧ"+","+studentNum+","+cardNum+",�ɹ�\n";
        oplog.writeOpLog(opStr2);
    }

    string newCardNum = this->addCard(studentNum);
    opStr += newCardNum + ",�ɹ�\n";
    oplog.writeOpLog(opStr);
}

//===========================================================
// ��������recharge
// ���ܣ���ѧ��У԰���ĳ�ֵ����
// ������������ڣ�ʱ�䣻ѧ�ţ���ֵ���
// ����ֵ����
//===========================================================
void CardOpration::recharge(string date, string time, string studentNum, float addMoney){
    // ��ֵ������־�������
    // date,time,��ֵ,ѧ��,��ֵǰ���,��ֵ���,��ֵ�����,�ɹ�
    // date,time,��ֵ,ѧ��,��ֵǰ���,��ֵ���,ʧ��,ʧ��ԭ��
    // date,time,��ֵ,ѧ��,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"��ֵ"+","+studentNum+",";

    // �ж�ѧ���Ƿ��Ѿ�����
    if(!this->stuExist(studentNum)){
        opStr += "ʧ��,�û�������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // �ж�ѧ���Ƿ��Ѿ�����
    if(this->stuLeave(studentNum)){
        opStr += "ʧ��,�û���ע��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    string moneyStr;
    float money = this->students[studentNum].money;
    stringstream sStream;

    // ����־������ӳ�ֵǰ���
    sStream <<fixed<<setprecision(2)<< money;
    sStream >> moneyStr;
    sStream.clear();
    opStr += moneyStr +",";
    // ����־������ӳ�ֵ���
    sStream <<fixed<<setprecision(2)<< addMoney;
    sStream >> moneyStr;
    sStream.clear();
    opStr += moneyStr +",";

    if(money + addMoney >= MAXMONEY){
        opStr += "ʧ��,������ֵ���޽��\n";
        oplog.writeOpLog(opStr);
    }
    else{
        this->students[studentNum].money += addMoney;
        sStream <<fixed<<setprecision(2)<< money + addMoney;
        sStream >> moneyStr;
        sStream.clear();
        opStr += moneyStr +",�ɹ�\n";
        oplog.writeOpLog(opStr);
    }
}
