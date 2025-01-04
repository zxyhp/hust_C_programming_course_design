#include "analyzedialog.h"
#include "ui_analyzedialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// ��������AnalyzeDialog
// ���ܣ�AnalyzeDialog��Ĺ��캯������ʼ����Ա����m_allReacords��
//      m_sortedRecords��������ѧ����������������
// ����������������ڵ����Ѽ�¼��Ϣ����ڼ�ֵ�Լ��ϣ��������Ѽ�¼�����
//         ͷ�ڵ��ָ�룻�����ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
AnalyzeDialog::AnalyzeDialog(map<int,pair<int,winXfStr*>> m, sortedXf* sortedRecords, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeDialog)
{
    ui->setupUi(this);

    // ��ÿ�����ڶ�ȡ�ļ�¼map����
    this->m_allReacords = m;

    // �Ӵ��ڼ�¼��ȡ�������м�¼�����򼯺�
    this->m_sortedRecords = sortedRecords;

    // ����ѧ�������ֻ���������� ������볤����ʮλ eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// ��������~AnalyzeDialog
// ���ܣ�AnalyzeDialog���������������AnalyzeDialog��Ķ����ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
AnalyzeDialog::~AnalyzeDialog()
{
    delete ui;
}

//===========================================================
// ��������on_backBtn_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void AnalyzeDialog::on_backBtn_clicked()
{
    this->close();
}

//===========================================================
// ��������on_analyzeBtn_clicked
// ���ܣ����ര�ڽ����ϵķ�����ť��click()�źŵĲۺ�����������������
//      ѧ�Ŷ�Ӧѧ������һ����ʳ���������ѵ�ѧ���������ǵ���Ϣ�����
//      Ա����m_info��
// �����������
// ����ֵ����
//===========================================================
void AnalyzeDialog::on_analyzeBtn_clicked()
{
    cout<<"on_analyzeBtn_clicked"<<endl;
    clock_t startTime = clock();

    this->m_info.clear();

    // ѧ��
    string stuNum = ui->stuNumEdit->text().toStdString();

    if(stuNum.size()<10){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�������ʽ��ȷ��ѧ�ţ�"));
        return;
    }

    auto CO = CardOpration::getinstance();
    if(CO->students.find(stuNum) == CO->students.end()){
        return;
    }

    // ������һ������һ���Ŷӵ����
    auto end = this->m_allReacords.end();
    for(auto it=this->m_allReacords.begin();it!=end;it++){
        int num = it->second.first;
        // ���һ����Ϣ��ָ��λ��
        auto nowStu = it->second.second;
        string stu1,stu2;
        while(num>0){
            // ���p�ļ�¼ѧ��ΪstuNum
            if(nowStu->stuNum == stuNum){
                stu1 = nowStu->next->stuNum;
                // ���stu1ͬѧ��δ��pͬѧ�Ź���
                if(nowStu->next->date == nowStu->date && nowStu->next->time[0] == nowStu->time[0]){
                    if(m_info.find(stu1)==m_info.end()){
                        pair<int,string> p(1,nowStu->next->name);
                        m_info[stu1] = p;
                    }
                    else{
                        // ����Ź� �������һ
                        m_info[stu1].first++;
                    }
                }
                stu2 = nowStu->previous->stuNum;
                // ���stu2ͬѧ��δ��pͬѧ�Ź���
                if(nowStu->previous->date == nowStu->date && nowStu->previous->time[0] == nowStu->time[0]){
                    if(m_info.find(stu2)==m_info.end()){
                        pair<int,string> p(1,nowStu->previous->name);
                        m_info[stu2] = p;
                    }
                    else{
                        // ����Ź� �������һ
                        m_info[stu2].first++;
                    }
                }
            }
            nowStu = nowStu->previous;
            num--;
        }
    }


    // ���������ڴ��ڼ���ͬʱ���ѵ��˵����
    auto p = this->m_sortedRecords;
    // ������Ѽ�¼Ϊ��
    if(p->next == nullptr || p->next->next == nullptr){
        this->updateTable();
        clock_t endTime = clock();
        cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
        return;
    }

    auto pre_p = p;
    p = pre_p->next;
    auto next_p = p->next;

    while(next_p!=nullptr){
        // ���p��Ŀ�����
        if(p->stuNum == stuNum){
            int winNum = p->winNum;
            string date = p->date;

            // �ж�pre_p�Ƿ��������ѡ����
            // ��������ڴ���
            if(pre_p->winNum-winNum==-1 || pre_p->winNum-winNum==1){
                // �ж��Ƿ���ͬһ��
                if(pre_p->date == date){
                    // ���pre_pͬѧ��δ��pͬѧ�Ź���
                    if(m_info.find(pre_p->stuNum)==m_info.end()){
                        pair<int,string> pair_obj(1,pre_p->name);
                        m_info[pre_p->stuNum] = pair_obj;
                    }
                    // ����Ź� �������һ
                    else{
                        m_info[pre_p->stuNum].first++;
                    }
                }
            }

            // �ж�next_p�Ƿ��������ѡ����
            // ��������ڴ���
            if(next_p->winNum-winNum==-1 || next_p->winNum-winNum==1){
                // �ж��Ƿ���ͬһ��
                if(next_p->date == date){
                    // ���pre_pͬѧ��δ��pͬѧ�Ź���
                    if(m_info.find(next_p->stuNum)==m_info.end()){
                        pair<int,string> pair_obj(1,next_p->name);
                        m_info[next_p->stuNum] = pair_obj;
                    }
                    // ����Ź� �������һ
                    else{
                        m_info[next_p->stuNum].first++;
                    }
                }
            }

        }
        pre_p = p;  p = next_p; next_p = next_p->next;
    }

    // ����ʣ�����һ���ڵ�û���ж�
    if(p->stuNum == stuNum){
        int winNum = p->winNum;
        string date = p->date;

        // �ж�pre_p�Ƿ��������ѡ����
        // ��������ڴ���
        if(pre_p->winNum-winNum==-1 || pre_p->winNum-winNum==1){
            // �ж��Ƿ���ͬһ��
            if(pre_p->date == date){
                // ���pre_pͬѧ��δ��pͬѧ�Ź���
                if(m_info.find(pre_p->stuNum)==m_info.end()){
                    pair<int,string> pair_obj(1,pre_p->name);
                    m_info[pre_p->stuNum] = pair_obj;
                }
                // ����Ź� �������һ
                else{
                    m_info[pre_p->stuNum].first++;
                }
            }
        }
    }


    // ����п��кͱ���������ɾȥ��ͬ���ڶ�ȡ˫��ѭ������ʱ�п��ܷ�����
    if(this->m_info.find("")!= this->m_info.end()){
        this->m_info.erase("");
    }
    if(this->m_info.find(stuNum)!= this->m_info.end()){
        this->m_info.erase(stuNum);
    }

    // ������������µ����ڽ������ʾ�������
    this->updateTable();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;

}

//===========================================================
// ��������updateTable
// ���ܣ����³�Ա����m_info����Ϣ�����ڽ����tableWidget�����
// �����������
// ����ֵ����
//===========================================================
void AnalyzeDialog::updateTable()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    ui->tableWidget->clear();

    // ����ͷ��
    ui->tableWidget->setColumnCount(3);
    QStringList head;
    head<<codec->toUnicode("ѧ��")
       <<codec->toUnicode("����")
      <<codec->toUnicode("����");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    auto end = this->m_info.end();
    int i=1;
    for(auto it = this->m_info.begin();it!=end;it++){
        if(it->second.first <= 2) continue;
        // ��������
        ui->tableWidget->setRowCount(i);
        // ѧ��
        ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(QString::fromStdString(it->first)));
        // ����
        ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(codec->toUnicode(it->second.second.c_str())));
        // ����
        ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(QString::number(it->second.first)));

        i++;
    }

}
