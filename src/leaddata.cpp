#include "leaddata.h"

//===========================================================
// ��������LeadData
// ���ܣ�LeadData��Ĺ��캯������ɳ�Ա����m_sortedOp��m_opration
//      �ĳ�ʼ��������ʳ�ô��ڵ����Ѽ�¼�������д�������ļ�
// �����������
// ����ֵ����
//===========================================================
LeadData::LeadData()
{
    // ��ʼ��m_sortedOp (�����������������)
    this->m_sortedOp = new xfStr;
    this->m_sortedOp->next = NULL;

    this->init_m_opration();
}

//===========================================================
// ��������runTime
// ���ܣ����㴫�뺯��������ʱ�䲢��ʾ�ڿ���̨������
// ���������LeadData��ĺ���ָ��
// ����ֵ����
//===========================================================
void LeadData::runTime(LeadData::Func f)
{
    clock_t startTime = clock();
    (this->*f)();
    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// ��������initStu
// ���ܣ�����ͬĿ¼��students.txt�ļ��ڵ�ѧ����Ϣ����ѧ����������Ϣ
//      ���뵽ѧ����Ϣϵͳ��students������
// �����������
// ����ֵ����
//===========================================================
void LeadData::initStu()
{
    // �ڿ���̨����������ں����ĺ�����
    cout<<__FUNCTION__<<endl;

    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();

    char ch[MAXSIZE];
    string filePath = QDir::currentPath().toStdString() + "/students.txt";
    ifstream fp(filePath);
    if(!fp) {
        cout<<"students.txt��ʧ��!"<<endl;
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
// ��������init_m_opration
// ���ܣ���ʼ����Ա����m_opration
// �����������
// ����ֵ����
//===========================================================
void LeadData::init_m_opration()
{
    stringstream stream;
    string str = "";

    // ��ʼ����¼ ����1-99����(0-98)�����Ѳ��� �� ����У԰��(99)�Ĳ��� �������¼�ı�ͷ
    for(int i=0;i<WINNUM+1;i++){
        this->m_opration[i] = new xfStr;
        this->m_opration[i]->next = NULL;
        stream << i+1;
        stream >> str;
        // ��ͷ��str����¼��ǰ������������ĸ�����
        this->m_opration[i]->str = str;
        stream.clear();
        str.clear();
    }
}

//===========================================================
// ��������getOpration
// ���ܣ���xf.txt��cz.txt�ļ��л�ȡϵ�в������
// �����������
// ����ֵ����
//===========================================================
void LeadData::getOpration()
{
    // �����ǰ��������
    cout<<__FUNCTION__<<endl;

    // ��ȡ���ڵ����Ѽ�¼
    string filePath = QDir::currentPath().toStdString() + "/xf.txt";
    ifstream fp(filePath);
    char ch[MAXSIZE];
    if(!fp){
        cout<<"�ļ�xf.txt��ʧ��!"<<endl;
        return;
    }
    // ����·����ű�־
    int num = -1;
    string numStr;
    // βָ��
    xfStr* hailp = NULL;
    while(fp.getline(ch,MAXSIZE)){
        // �������W1��W2�Ⱦ�����·��
        if(strlen(ch) <= 5) {
            // �������
            num++;
            hailp = this->m_opration[num];
            numStr = hailp->str;
            continue;
        }
        // ��ָ��
        xfStr* p = new xfStr;
        p->str = string(ch,strlen(ch)-1)+","+ numStr + ";\n";
        p->next = NULL;
        // β�巨
        hailp->next = p;
        hailp = p;
    }
    fp.close();

    // ��ȡ��Ƭ����
    filePath = QDir::currentPath().toStdString() + "/cz.txt";
    fp.open(filePath);
    hailp = m_opration[99];
    string str;
    while(fp.getline(ch,MAXSIZE)){
        xfStr* p = new xfStr;
        str = string(ch,strlen(ch)) + "\n";
        // ��Ƭ�������ǰ����˸��ո���������������������ʱ���ַ����Ļ�ȡ
        p->str = "        " + str.substr(0,8)+","+str.substr(8);
        p->next = NULL;
        hailp->next = p;
        hailp = p;
    }
    fp.close();
}

//===========================================================
// ��������insert_vec
// ���ܣ������۰��������ÿ�β���һ��Ԫ�ص�����������(����str����)
// ������������ں�-1������ʱ���ַ������������������
// ����ֵ����
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
// ��������sortOpration
// ���ܣ������в������ù鲢������۰�����������ʱ����Ⱥ�������򣬽�
//      �����Ľ�����뵽��Ա����m_sortedOp��ָ��������
// �����������
// ����ֵ����
//===========================================================
void LeadData::sortOpration()
{
    // �����ǰ��������
    cout<<__FUNCTION__<<endl;

    // ��Ŵ�ÿ·�������л�ȡ��һ��Ԫ�� <����·������ţ��ַ���>
    vector<pair<int,string> > vec;

    // i��������� 0-98 99��ʾ��Ƭ����
    for(int i=0;i<WINNUM+1;i++){
        if(this->m_opration[i]->next!=NULL){
            // ����substr(8,17)��ȡ�ַ����е�ʱ������(�����׼)
            // insert_vec�ǽ���ȡ��Ԫ��������뵽vec��
            insert_vec(i,this->m_opration[i]->next->str.substr(8,17),vec);
        }
    }

    // vec��Ԫ�ظ���Ϊ0ʱ������ѭ������������ɣ�
    // vector<pair<int,string> > vec��Ԫ���ǰ�pair�е�string���������
    xfStr* p = NULL;
    xfStr* hailSp = this->m_sortedOp;
    while(vec.size() != 0){
        // ��ȡ����СԪ�ض�Ӧ�Ĵ������
        int dir = vec[0].first;
        p = this->m_opration[dir];
        // ��vec��ɾȥ��ȡ����Ԫ��
        vec.erase(vec.begin());

        // ��ȡ�����ļ�¼��� �����뵽��������Ķ�β
        xfStr *Sp = new xfStr;
        Sp->str = p->next->str;
        Sp->next = NULL;
        hailSp->next = Sp;
        hailSp = Sp;

        // �Ӵ�������¼������ɾȥ�Ѿ�������ɵļ�¼��Ӧ�Ľ��
        p->next = p->next->next;

        // ����ô�������»���Ԫ�ش��ţ���������뵽vec��
        if(p->next != NULL){
            insert_vec(dir,p->next->str.substr(8,17),vec);
        }
    }
}

//===========================================================
// ��������carryOutOp
// ���ܣ��������Ĳ�����ѧ�������ʳ��Ӧ��ϵͳ�н��д�����β��ʵʩ
// �����������
// ����ֵ�����һ��������<���ڣ�ʱ��>
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
        // ��ȡ�������
        str = p->str;
        str = str.substr(0,str.size()-2);
        // 3193355,20211005,07001172,8.25,56 (size:30)�����������
        // 20210903,08315316,��ʧ,2020760664 ��ʧ����ҡ������������������(ǰ�滹�а˸��ո��)
        // 20211014,00012813,��ֵ,2020030309,800 ��ֵ�������(ǰ�滹�а˸��ո��)
        switch(str.size()){
        case 32:
        case 33:
        case 34:
            // ���Ѳ���
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
            // ��ʧ����ҡ���������������
            if(res[2] == "��ʧ"){
                CO->lossSolution_1(date,time,res[3]);
            }
            else if("���" == res[2]){
                CO->lossSolution_2(date,time,res[3]);
            }
            else if("����" == res[2]){
                CO->makeUpCard(date,time,res[3]);
            }
            else if("����" == res[2]){
                CO->deleteStu(date,time,res[3]);
            }
            else if("��ֵ" == res[2]){
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

