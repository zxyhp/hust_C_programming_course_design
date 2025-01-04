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
// ��������CheckFileDialog
// ���ܣ�CheckFileDialog��Ĺ��캯������ɳ�Ա����m_sortXfStr��
//      ��ʼ��
// �������������������־��������ͷ�ڵ�ָ�룻�����ڲ�����ָ�루û
//          �и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CheckFileDialog::CheckFileDialog(xfStr* s,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckFileDialog)
{
    ui->setupUi(this);
    this->m_sortXfStr = s;
}

//===========================================================
// ��������~CheckFileDialog
// ���ܣ�CheckFileDialog���������������CanteenAppDialog��Ķ���
//      �ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CheckFileDialog::~CheckFileDialog()
{
    delete ui;
}

//===========================================================
// ��������on_backBtn_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CheckFileDialog::on_backBtn_clicked()
{
    this->close();
}

//===========================================================
// ��������on_checkBtn_clicked
// ���ܣ����ര�ڽ����ϵ�������ť��click()�źŵĲۺ������������Ѽ�¼
//      �����ļ����Ƿ�������һ�л���еĸı�
// �����������
// ����ֵ����
//===========================================================
void CheckFileDialog::on_checkBtn_clicked()
{
    cout<<"on_checkBtn_clicked"<<endl;
    clock_t startTime = clock();

    // ��������
    char ch[MAXSIZE];
    string xfStr = "";
    string moneyStr;
    string xfStrP = "";
    stringstream ss;

    // ���汻ɾ�������
    vector<string> deleteVec;

    // �����������ӵ����
    vector<string> addVec;

    auto p = this->m_sortXfStr->next;

    string filePath = QDir::currentPath().toStdString() + "/all_xf.txt";
    ifstream fp(filePath);
    if(!fp){
        cout<<"all_xf.txt�ļ���ʧ�ܣ�"<<endl;
        return;
    }

    while(p!=nullptr){
        // ����ļ��Ѿ���ȡ��� ������ѭ��
        if(!fp.getline(ch,MAXSIZE)) break;
        // ��ȡ���Ľ��ܺ����� �� ����еľ�������
        xfStr = this->getXfStr(string(ch,strlen(ch)));

        if(p->str != xfStr){

            // ��p2����һ����ʼ��������������������������������ӵĻ���ɾ����
            auto p2 = p;

            while(p2->next!=nullptr){
                p2 = p2->next;
                // ˵���ҵ��˶�Ӧ����� xfStr���֮ǰ����䱻ɾ��
                if(p2->str == xfStr) break;
            }

            if(p2->next == nullptr){
                // û���ҵ�������� �Ǳ����ӵ�
                addVec.push_back(xfStr);
                continue;
            }
            // �ҵ��������
            else{
                // p��p2֮ǰ��Ӧ����䶼�Ǳ�ɾ������� p2������������
                while(p!=p2){
                    deleteVec.push_back(p->str);
                    p = p->next;
                }
            }
        }
        p = p -> next;
    }

    // ���p��Ϊ�վ�������ѭ�� ��˵����β������䱻ɾ����
    if(p!=nullptr){
        while(p!=nullptr){
            deleteVec.push_back(p->str);
            p = p->next;
        }
    }
    // ��������ļ���δ�����������ѭ�� ��˵���ļ�β������䱻����
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
// ��������getXfStr
// ���ܣ��Դ����ַ������н���
// ��������������ַ���
// ����ֵ�����ܺ���ַ���
//===========================================================
string CheckFileDialog::getXfStr(string str)
{
    // ������ܺ���ַ���
    string xfStr;
    static int key[]={Y,S,F}; // key[0]=24,key[1]=18,key[2]=5
    // ��ü��ܵ����(0��1��2)
    int checkNum = str[0]-'a';

    // ��������
    char ch;
    int num;

    // ���������ַ���
    auto it = str.begin();
    it++;
    auto end = str.end();
    while(it!=end){
        num = (*it-'a')-key[checkNum];

        if(num<0) num += ALLLENGTH;

        // '0'-'9'��Ӧ 0-9 ����.����Ӧ10
        if(num==10) ch='.';
        else ch = num+'0';

        // ��ȡ��һ���������ֵ��±�
        checkNum = (checkNum+1)%KEYLENGTH;

        xfStr += ch;
        it++;
    }

    // ���ؽ��ܺ���ַ���
    return xfStr;
}

//===========================================================
// ��������updateTable
// ���ܣ��Դ����ַ������н���
// ����������������е���Ϣ����ɾ���е���Ϣ
// ����ֵ����
//===========================================================
void CheckFileDialog::updateTable(vector<string> addVec, vector<string> deleteVec)
{
    ui->tableWidget->clear();

    // ��������ַ��ڴ��ڽ������������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ����ձ�ͷ
    ui->tableWidget->clear();
    // ���ñ������
    ui->tableWidget->setColumnCount(5);
    // ���ñ�ͷ����
    QStringList head;
    head<<codec->toUnicode("����")
       <<codec->toUnicode("ʱ��")
      <<codec->toUnicode("����")
     <<codec->toUnicode("���ѽ��")
    <<codec->toUnicode("����ԭ��");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    // ��������
    int lineNum = addVec.size() + deleteVec.size();
    ui->tableWidget->setRowCount(lineNum);


    int i=0;
    // �������ַ������ı���
    auto it = addVec.begin();
    auto end = addVec.end();
    while(it!=end){
        // ����
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it->substr(0,8))));
        // ʱ��
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(it->substr(8,8))));
        // ����
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it->substr(16,7))));
        // ���ѽ��
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(it->substr(23))));
        // ����ԭ��
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(codec->toUnicode("������")));
        i++;
        it++;
    }

    // ��ɾ���ַ������ı���
    auto it2 = deleteVec.begin();
    auto end2 = deleteVec.end();
    while(it2!=end2){
        // ����
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it2->substr(0,8))));
        // ʱ��
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(it2->substr(8,8))));
        // ����
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it2->substr(16,7))));
        // ���ѽ��
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(it2->substr(23))));
        // ����ԭ��
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(codec->toUnicode("��ɾ��")));
        i++;
        it2++;
    }
}


