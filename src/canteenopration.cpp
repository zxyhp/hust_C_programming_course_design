#include "canteenopration.h"

#include <QDir>

CanteenOpration *CanteenOpration::ptrCOP = nullptr;

//===========================================================
// ��������CanteenOpration
// ���ܣ�CanteenOpration��Ĺ��캯������ʼ�����ֳ�Ա����
// �����������
// ����ֵ����
//===========================================================
CanteenOpration::CanteenOpration()
{
    // ��ʼ������ ���wz.txt�ļ�
    this->initWin();

    // ��ʼ��m_winXfRecords���������99*60000����¼��������
    this->initWinXfRecords();
}

//===========================================================
// ��������split
// ���ܣ���delimΪ������������str�ַ�����ֳ����ɸ��ַ���
// ������������и���ַ������и��ַ����ķָ��ַ���
// ����ֵ����str�и���ɵ����ɸ��ַ����ļ���
//===========================================================
vector<string> CanteenOpration::split(const string &str, const string &delim)
{
    vector<string> res;
    // ���strΪ���򷵻ؿ�����
    if("" == str) return res;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����
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
// ��������consume
// ���ܣ�ʵ�ִ��뿨�Ŷ�Ӧѧ�������Ѳ���
// ����������Ƿ�Ϊ���������ı�־�����ں�-1�����ţ����ѽ����ڣ�ʱ��
// ����ֵ����
//===========================================================
void CanteenOpration::consume(bool isPl, int winNum, string cardNum, float conMoney, string date, string time)
{
    // ��ȡ��Ƭ������ָ�� �ڲ�����ѧ���Ϳ�Ƭ��������Ϣ
    auto CO = CardOpration::getinstance();

    // ������־���� ���ں���д���������־
    OprationLog oplog;

    // ����ss�������ں����ַ��������ֵ�ת��
    stringstream ss;

    // ��ȡ���ںŵ��ַ�����ʽ��д��winNumStr
    ss << winNum+1;
    string winNumStr;
    ss >> winNumStr;
    ss.clear();

    // ���Ѳ�����־�������
    // date,time,����,����,���Ѵ���,ѧ��,�������,���ѽ��,���Ѻ����,�ɹ�
    // date,time,����,����,���Ѵ���,ѧ��,�������,���ѽ��,ʧ��,ʧ��ԭ��
    // date,time,����,����,���Ѵ���,ʧ��,ʧ��ԭ��
    string opStr = date+","+time+","+"����"+","+cardNum+","+winNumStr+",";

    // �ȸ��ݴ������ݸ�������ʱ��
    if(this->updateTime(isPl,date,time) == false){
        opStr += "ʧ��,����ʱ�䲻�ܻ���\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ����Ǹ������� isPl==false
    if(!isPl && this->m_timeFrame == 4){
        opStr += "ʧ��,δ��ʳ������ʱ��\n";
        oplog.writeOpLog(opStr);
        return;
    }

    //�жϿ����Ƿ����
    if(CO->cards.find(cardNum) != CO->cards.end()){
        // �жϿ����Ƿ���ʹ��״̬
        if(CO->cards[cardNum].status == true){
            opStr += "ʧ��,У԰���ѹ�ʧ\n";
            oplog.writeOpLog(opStr);
            return;
        }
    }
    else{
        opStr += "ʧ��,У԰��������\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ���ݿ���������ѧ����
    string stuNum = CO->cards[cardNum].stuNumber;

    // ��opStr���ѧ��
    opStr += stuNum + ",";

    // ���ڴ洢һЩ�����ַ�����ʽ
    string moneyStr;

    // moneyΪ�˻����
    float money = CO->students[stuNum].money;

    // ��opStr����˻�����Ǯ���
    ss << fixed << setprecision(2) <<money;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",";

    // ��opStr����˻����ѽ��
    ss << fixed << setprecision(2) <<conMoney;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",";

    // ����˻����С�����ѽ��
    if(money < conMoney){
        opStr += "ʧ��,�˻�����\n";
        oplog.writeOpLog(opStr);
        return;
    }

    // ����Ǹ�����������Ҫ������������
    if(!isPl){
        // �жϸÿ����ڸ�ʱ���Ƿ����ѹ�
        // ����δ���ѹ� �򴴽�����Ԫ�ظ�0ֵ
        if(m_csMoneyInFrame.find(stuNum) == m_csMoneyInFrame.end()) {
            m_csMoneyInFrame[stuNum] = 0;
        }

        // ����ʱ�����ѽ�����20 ����Ҫ��������
        if(m_csMoneyInFrame[stuNum] + conMoney > 20) {

            // �������봰��
            PasswordDialog* PD = new PasswordDialog;
            // ���Ӳ� ����������벢ȷ�� �� this->passWord�ͻ��ȡ�����������
            connect(PD,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
            PD->exec();

            // ��������������
            if(this->m_password.toStdString() != CO->students[stuNum].password){
                opStr += "ʧ��,������������\n";
                oplog.writeOpLog(opStr);
                return;
            }

            // �������������ȷ ʱ�����ѽ����
            m_csMoneyInFrame[stuNum] = 0;
        }
        // ����ʱ�����ѽ��С�ڵ���20 �����ʱ�����ѽ��
        else{
            m_csMoneyInFrame[stuNum] += conMoney;
        }
    }

    // ����һ�±�ʾ���ѳɹ��Ĳ���

    // ���ڵ��������Ѵ������ܽ������
    this->win[winNum].consumeTimes++;
    this->win[winNum].consumeMoney += conMoney;

    // ���ѳɹ������Ѽ�¼���뵽����Ŀ¼��
    // �������뵽�������Ѽ�¼��
    this->writeXfRecords(winNum,cardNum,stuNum,date,time,conMoney);

    // �˻�����ȥ���ѽ��
    CO->students[stuNum].money -= conMoney;

    // ��opStr����˻����Ѻ����
    ss << fixed << setprecision(2) <<money - conMoney;
    ss >> moneyStr;
    ss.clear();
    opStr += moneyStr + ",�ɹ�\n";

    // д�������־���
    oplog.writeOpLog(opStr);
}

//===========================================================
// ��������updateTFrame
// ���ܣ����ݴ����ʱ��������ʱ�α���m_timeFrame
// ������������º����ʱ��
// ����ֵ����
//===========================================================
void CanteenOpration::updateTFrame(int time){
    if(time>=7000000 && time<=9000000)
        this->m_timeFrame = 1; // 7�㵽9���緹ʱ��
    else if(time>=11000000 && time<=13000000)
        this->m_timeFrame = 2; // 11�㵽13���緹ʱ��
    else if(time>=17000000 && time<=19000000)
        this->m_timeFrame = 3; // 17�㵽19����ʱ��
    else this->m_timeFrame = 4;
}

//===========================================================
// ��������writeXfRecords
// ���ܣ���һ�����Ѽ�¼��Ϣд�뵽��Ӧ���ڵ����Ѽ�¼����Ķ�Ӧ�ڵ���
// ������������ں�-1�����ţ�ѧ�ţ����ڣ�ʱ�䣻���ѽ��
// ����ֵ����
//===========================================================
void CanteenOpration::writeXfRecords(int winNum,string cardNum
             ,string stuNum,string date,string time,float conMoney)
{
    // ��ȡ��Ϣ��д��ڵ��ָ��
    winXfStr *p = this->m_winXfRecords[winNum].now;
    // д����Ӧ��Ϣ
    p->cardNum = cardNum;
    p->stuNum = stuNum;
    p->date = date;
    p->time = time;
    p->money = conMoney;
    p->name = CardOpration::getinstance()->students[stuNum].name;
    // ��������б��������д��ڵ�
    this->m_winXfRecords[winNum].now = p->next;
    // �����¼����û�д���������� ���¼������һ
    if(this->m_winXfRecords[winNum].num != FILELINES)
        this->m_winXfRecords[winNum].num++;
    // ��������б��������д��ڵ��Ӧ������
    int lineNum = this->win[winNum].fileLine;
    this->win[winNum].fileLine = (lineNum+1)%FILELINES;
}

//===========================================================
// ��������getWinConsume
// ���ܣ����������ȡ˽�г�Ա���������м�¼�������ܴ����������ܽ��
// ������������ں�-1
// ����ֵ��<���ڵĵ��������Ѵ��������ڵĵ��������ѽ��>
//===========================================================
pair<int, float> CanteenOpration::getWinConsume(int winnum)
{
    pair<int, float> p(this->win[winnum].consumeTimes,this->win[winnum].consumeMoney);
    return p;
}

//===========================================================
// ��������getWinLine
// ���ܣ����������ȡ˽�г�Ա�������ڵ���д������
// ������������ں�-1
// ����ֵ���������Ѽ�¼����һ����¼��д������
//===========================================================
int CanteenOpration::getWinLine(int winnum)
{
    return this->win[winnum].fileLine;
}

//===========================================================
// ��������getXfRecords
// ���ܣ����������ȡĳ�����ڵ����Ѽ�¼�����
// ������������ں�-1
// ����ֵ�����ڵ����Ѽ�¼����ڱ���
//===========================================================
winXfHead CanteenOpration::getXfRecords(int num)
{
    return this->m_winXfRecords[num];
}

//===========================================================
// ��������receiveData
// ���ܣ����ڽ����������봰�����������������ַ�������Ա����m_password��
// ����������������봰�����������������ַ���
// ����ֵ����
//===========================================================
void CanteenOpration::receiveData(QString data)
{
    this->m_password = data;
}

//===========================================================
// ��������updateTime
// ���ܣ����³�Ա����m_date��m_time
// ����������Ƿ�Ϊ���������ı�־�������ڣ���ʱ��
// ����ֵ������ʱ����³ɹ����
//===========================================================
bool CanteenOpration::updateTime(bool isPl, string date, string time)
{
    // ���������С�ڵ�ǰ��¼���� �����ʧ��
    if(this->m_date > date) return false;
    // ��������ڵ��ڵ�ǰ��¼����
    else if(this->m_date == date){
        // �����ʱ��С�ڵ�ǰ��¼ʱ�� �����ʧ��
        if(this->m_time > time) return false;

        // �����ʱ����ڵ��ڵ�ǰ��¼ʱ��
        else{
            // ����ʱ��
            this->m_time = time;

            // ����Ǹ��˲���
            if(!isPl){
                // ��ȡint�͵���ʱ��
                stringstream stream;
                int newTime;
                stream << time;
                stream >> newTime;
                stream.clear();

                // ����ʱ��ǰ�ľ�ʱ��
                int oldTFrame = this->m_timeFrame;
                this->updateTFrame(newTime);
                // �ж��Ƿ��ʱ�Σ���ʱ���������m_csMoneyInFrame��
                if(oldTFrame!=this->m_timeFrame)
                    m_csMoneyInFrame.clear();
            }
        }
    }
    // ��������ڴ��ڵ�ǰ��¼���� ���������
    else{
        // ������������Ҫ�����д��ڵĵ������Ѵ����������ܽ������
        this->updateWin();
        // �������ں�ʱ��
        this->m_date = date;
        this->m_time = time;

        // ����Ǹ��˲���
        if(!isPl){
            // ��ȡint�͵���ʱ��
            stringstream stream;
            int newTime;
            stream << time;
            stream >> newTime;
            stream.clear();

            // ����ʱ�κ�csMoneyInFrame����
            this->updateTFrame(newTime);
            m_csMoneyInFrame.clear();
        }
    }

    return true;
}

//===========================================================
// ��������initWinXfRecords
// ���ܣ���ʼ����Ա����m_winXfRecords[0-98]��99�����ڵ����Ѽ�¼��ڣ�
// �����������
// ����ֵ����
//===========================================================
void CanteenOpration::initWinXfRecords()
{
    // ��ÿ�����ڳ�ʼ��һ��60000������˫��ѭ������
    for(int i=0;i<WINNUM;i++){
        // ��ǰ�������������ڶ���
        winXfHead hp;

        // ˫��ѭ������Ľ���
        winXfStr *head = new winXfStr;
        winXfStr *p1 = head; // ǰһ�����(���p2)
        winXfStr *p2 = nullptr; // ��һ���ڵ�(���p1)
        for(int j=0;j<FILELINES-1;j++){
            p2 = new winXfStr;
            p1->next = p2;
            p2->previous = p1;
            p1=p2;
        }
        // �����ͷβ����
        p1->next = head;
        head->previous = p1;

        // ��������ڵ�ָ�봫����ڶ���
        hp.now = head;
        // ����ڶ�����뵽m_winXfRecords��
        this->m_winXfRecords[i] = hp;
    }
}

//===========================================================
// ��������initWin
// ���ܣ���ʼ����Ա����m_win[0-98]��99�����ڣ�
// �����������
// ����ֵ����
//===========================================================
void CanteenOpration::initWin()
{
    // ��������
    stringstream ss;
    vector<string> res;
    char ch[MAXSIZE];

    // ��λ���ļ�(wz.txt)��ȡÿ����¼��ʼ����ĳ�ʼλ��
    string filePath = QDir::currentPath().toStdString() + "/wz.txt";
    ifstream fp(filePath);
    for(int i=0;i<WINNUM;i++){
        // ��ʼ���Ѵ��������ѽ�Ϊ0
        this->win[i].consumeMoney = 0;
        this->win[i].consumeTimes = 0;
        // ��fp�ļ��л�ȡ���ڼ�¼�ĳ�ʼ��¼λ�ã��ַ����ͣ�
        fp.getline(ch,MAXSIZE);
        res = split(string(ch,strlen(ch)-1),",");
        // ����¼λ��תΪint��д��win[i].fileLine��
        ss << res[1];
        ss >> this->win[i].fileLine;
        ss.clear();
    }
    fp.close();
}

//===========================================================
// ��������updateWin
// ���ܣ���99�����ڵĵ��������ܽ����ܴ�������
// �����������
// ����ֵ����
//===========================================================
void CanteenOpration::updateWin()
{
    for(int i=0;i<WINNUM;i++){
        this->win[i].consumeMoney = 0;
        this->win[i].consumeTimes = 0;
    }
}


