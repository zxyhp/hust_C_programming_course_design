#include "summarizedialog.h"
#include "ui_summarizedialog.h"
#include <QDir>

#define Y 24
#define S 18
#define F 5
#define KEYLENGTH 3
#define ALLLENGTH 26 // ��.����10

#include <QMessageBox>
#include <QTextCodec>
#include <ctime>

//===========================================================
// ��������SummarizeDialog
// ���ܣ�SummarizeDialog��Ĺ��캯������ɳ�Ա����m_COP�ĳ�ʼ����
//      ����ʳ�ô��ڵ����Ѽ�¼�������д�������ļ�
// ���������ʳ��Ӧ�ò������ָ�룻�����ڲ�����ָ�루û�и�������ᴫ
//          ��nullptr��
// ����ֵ����
//===========================================================
SummarizeDialog::SummarizeDialog(CanteenOpration* COP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SummarizeDialog)
{
    ui->setupUi(this);

    m_COP = COP;

    string tipStr = "�������ݻ������򻨷�ʱ�䣺";

    this->init();

    clock_t startTime = clock();
    this->sortWinXf(this->m_allReacords);
    clock_t endTime = clock();

    this->writeWinXf();

    // ����ʱ���ַ���
    stringstream ss;
    string timeStr;
    ss<<fixed<<setprecision(3)<<double(endTime - startTime)/CLOCKS_PER_SEC;
    ss>>timeStr;

    tipStr += timeStr + "s\n(�����������ļ���д��ʱ��)";

    // ������ʾ����ʱ��
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QMessageBox::information(this,codec->toUnicode("��ʾ"),
                         codec->toUnicode(tipStr.c_str()));

}

//===========================================================
// ��������~SummarizeDialog
// ���ܣ�SummarizeDialog���������������SummarizeDialog��Ķ���
//      �ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
SummarizeDialog::~SummarizeDialog()
{
    delete ui;
}

//===========================================================
// ��������on_backButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_statistButton_clicked
// ���ܣ����ര�ڽ����ϵ�ͳ�ư�ť��click()�źŵĲۺ�������ʾͳ�ƴ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::on_statistButton_clicked()
{
    StatisticsDialog *SD = new StatisticsDialog(this->m_sortedRecords);
    SD->exec();
}

//===========================================================
// ��������init
// ���ܣ���ʼ�����99���������Ѽ�¼��Ϣ��ڵ����ף����ں�������
// �����������
// ����ֵ����
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
// ��������insert_vec
// ���ܣ������۰��������ÿ�β���һ��Ԫ�ص�����������(����str����)
// ������������ں�-1������ʱ���ַ������������������
// ����ֵ����
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
// ��������sortWinXf
// ���ܣ��ö�·�鲢����������ڵ���Ϣ������ʱ��������򣬲������������
//      m_sortedRecordsΪͷ�ڵ��������
// ���������������Ķ�·��Ϣ�ļ�ֵ�Լ���
// ����ֵ����
//===========================================================
void SummarizeDialog::sortWinXf(map<int,pair<int,winXfStr*>> allReacords)
{
    cout<<"sortWinXf"<<endl;
    clock_t startTime = clock();

    this->m_sortedRecords = new sortedXf;

    // ��Ŵ�ÿ·�������л�ȡ��һ��Ԫ�� <����·������ţ��ַ���>
    vector<pair<int,string> > vec;

    // ��ȡ��Ч���Ѳ��������һ��
    auto end = allReacords.end();
    winXfStr *xf = nullptr;
    for(auto it = allReacords.begin();it!=end;it++){
        xf = it->second.second;
        insert_vec(it->first,xf->date+xf->time,vec);
    }

    // ͷ�ڵ�����ڼ�¼���������¼����������
    if(vec.size()!=0)
        this->m_sortedRecords->date = (vec.end()-1)->second;

    // ��lines[99]��ÿһ��Ԫ�ض���0��ʱ���˳�ѭ������������ɣ�
    // vector<pair<int,string> > vec��Ԫ���ǰ�pair�е�string�ݼ������
    winXfStr* p = nullptr;
    sortedXf* sp = nullptr;
    while(vec.size() != 0){
        // ��ȡ�����Ԫ�ض�Ӧ�Ĵ������
        int dir = (vec.end()-1)->first;
        p = allReacords[dir].second;
        // ��vec��ɾȥ��ȡ����Ԫ��
        vec.erase(vec.end()-1);

        // ��ȡ�����ļ�¼��� �����뵽��������Ķ�ͷ
        sp = new sortedXf(p->cardNum,p->stuNum,p->date,p->time,p->money,p->name,dir+1);
        sp->next = this->m_sortedRecords->next;
        this->m_sortedRecords->next = sp;

        allReacords[dir].first--;
        allReacords[dir].second = p->previous;
        // ����ô�������»���Ԫ�ش��ţ���������뵽vec��
        if(allReacords[dir].first!=0){
            insert_vec(dir,p->previous->date+p->previous->time,vec);
        }
    }

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// ��������writeWinXf
// ���ܣ������������е���Ϣ��ϳ���䣬�����ܺ�д�������ļ�all_xf.txt��
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::writeWinXf()
{
    cout<<"writeWinXf"<<endl;
    clock_t startTime = clock();

    sortedXf *sp = this->m_sortedRecords; // �������������ָ��
    // ��ʼ��������������ͷ�ڵ�
    this->m_sortXfStr = new xfStr;

    // ��������
    string moneyStr; // ���ѽ�����
    string xfStrTrue; //��д���ļ���¼�����
    string infoStr=""; // д��all_xf.txt�ļ�������
    stringstream ss; // �ַ�������ת����
    auto p = this->m_sortXfStr;

    qsrand(time(NULL));
    // д��������Ϣ��ͬʱ���������������
    while(sp->next!=nullptr){
        sp=sp->next;
        // �õ��ַ����͵����ѽ��
        ss << fixed<<setprecision(2)<<sp->money;
        ss >> moneyStr;
        ss.clear();
        // �������
        xfStrTrue = sp->date+sp->time+sp->cardNum+moneyStr;
        // ������������������
        p->next = new xfStr(xfStrTrue);
        p=p->next;

        // �����У����
        int num = qrand()%3;
        char ch = 'a' + num;
        infoStr += ch + getCheckStr(num,xfStrTrue)+"\n";
    }

    // ������д��all_xf.txt�ļ���
    string filePath = QDir::currentPath().toStdString() + "/all_xf.txt";
    ofstream fp(filePath);
    fp << infoStr;
    fp.close();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// ��������getCheckStr
// ���ܣ��Դ�����ַ������м��ܣ������ؽ��
// ������������ܵ���ʼ��ţ������ܵ����
// ����ֵ�����ܺ���ַ���
//===========================================================
string SummarizeDialog::getCheckStr(int num,string str)
{
    // ��ż��ܺ���ַ���
    string checkStr = "";
    // ��Կ
    static int key[]={Y,S,F}; // key[0]=24,key[1]=18,key[2]=5
    // ��������
    char ch;
    // �����ַ���
    auto it = str.begin();
    auto end = str.end();
    while(it!=end){
        // �����'.'���Ӧ10��'0'-'9'��Ӧ0-9
        if(*it=='.') ch = (10+key[num])%ALLLENGTH+'a';
        else ch = (*it-'0'+key[num])%ALLLENGTH+'a';

        checkStr += ch;
        num = (num+1)%KEYLENGTH;
        it++;
    }
    // ���ؼ��ܺ���ַ���
    return checkStr;
}

//===========================================================
// ��������on_pushButton_clicked
// ���ܣ����ര�ڽ����ϵĲ�ѯ��ť��click()�źŵĲۺ�������ʾ��ѯ���ڽ���
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::on_pushButton_clicked()
{
    SearchDialog *SD = new SearchDialog(this->m_sortedRecords);
    SD->exec();
}

//===========================================================
// ��������on_analyzeButton_clicked
// ���ܣ����ര�ڽ����ϵķ�����ť��click()�źŵĲۺ�������ʾ�������ڽ���
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::on_analyzeButton_clicked()
{
    AnalyzeDialog* AD = new AnalyzeDialog(this->m_allReacords,this->m_sortedRecords);
    AD->exec();
}

//===========================================================
// ��������on_checkButton_clicked
// ���ܣ����ര�ڽ����ϵ�У�鰴ť��click()�źŵĲۺ�������ʾУ�鴰�ڽ���
// �����������
// ����ֵ����
//===========================================================
void SummarizeDialog::on_checkButton_clicked()
{
    CheckFileDialog CFD(this->m_sortXfStr);
    CFD.exec();
}
