#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <qabstractspinbox.h>
#include <QTableWidgetItem>
#include <QStringList>
#include <QTextCodec>

//===========================================================
// ��������SearchDialog
// ���ܣ�SearchDialog��Ĺ��캯������ɳ�Ա����m_sortedRecords��
//      ��ʼ���������ô��ڽ�����ĸ�������������������
// �������������ʳ�����Ѽ�¼��Ϣ������ͷ�ڵ��ָ�룻�����ڲ�����ָ
//         �루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
SearchDialog::SearchDialog(sortedXf *xf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    this->m_sortedRecords = xf;

    // ����ѧ�������ֻ���������� ������볤����ʮλ eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator1);

    // �������������ֻ�����뺺�� ������볤�����ĸ�����
    ui->nameEdit->setMaxLength(4);
    QRegExp regx2("[\u4e00-\u9fa5]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->nameEdit);
    ui->nameEdit->setValidator(validator2);

    // �������ѽ������ֻ������0-99.99�ڵ���λ������
    ui->minMoneyEdit->setMaxLength(5);
    QRegExp regx3("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator3 = new QRegExpValidator(regx3,ui->minMoneyEdit);
    ui->minMoneyEdit->setValidator(validator3);

    // �������ѽ������ֻ������0-99.99�ڵ���λ������
    ui->maxMoneyEdit->setMaxLength(5);
    QRegExp regx4("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator4 = new QRegExpValidator(regx4,ui->maxMoneyEdit);
    ui->maxMoneyEdit->setValidator(validator4);

}

//===========================================================
// ��������~SearchDialog
// ���ܣ�SearchDialog���������������SearchDialog��Ķ����ͷ�ʱ
//      �ͷ�ui
// �����������
// ����ֵ����
//===========================================================
SearchDialog::~SearchDialog()
{
    delete ui;
}

//===========================================================
// ��������on_stuNumCBox_stateChanged
// ���ܣ����ݶ�ѧ�Ź�ѡ���ѡ������ѧ����������������
// ���������ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void SearchDialog::on_stuNumCBox_stateChanged(int arg1)
{
    // ѡ��
    if(arg1 == 2){
        ui->stuNumEdit->setReadOnly(false);
    }
    // δѡ��
    else if(arg1 == 0){
        ui->stuNumEdit->clear();
        ui->stuNumEdit->setReadOnly(true);
    }
}

//===========================================================
// ��������on_nameCBox_stateChanged
// ���ܣ����ݶ�������ѡ���ѡ������������������������
// ���������ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void SearchDialog::on_nameCBox_stateChanged(int arg1)
{
    // ѡ��
    if(arg1 == 2){
        ui->nameEdit->setReadOnly(false);
    }
    // δѡ��
    else if(arg1 == 0){
        ui->nameEdit->setReadOnly(true);
        ui->nameEdit->clear();
    }
}

//===========================================================
// ��������on_DTCBox_stateChanged
// ���ܣ����ݶ�����ʱ�乴ѡ���ѡ�����ö���������ʱ��༭���ı༭����
// ���������ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void SearchDialog::on_DTCBox_stateChanged(int arg1)
{
    // enum ButtonSymbols { UpDownArrows, PlusMinus, NoButtons };
    // ѡ��
    if(arg1 == 2){
        ui->startDTEdit->setReadOnly(false);
        ui->endDTEdit->setReadOnly(false);
        ui->startDTEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        ui->endDTEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    }
    // δѡ��
    else if(arg1 == 0){
        ui->startDTEdit->setReadOnly(true);
        ui->endDTEdit->setReadOnly(true);
        ui->startDTEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->endDTEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->startDTEdit->setDate(QDate::fromString("20210923","yyyyMMdd"));
        ui->startDTEdit->setTime(QTime::fromString("000000","HHmmss"));
        ui->endDTEdit->setDate(QDate::fromString("20210923","yyyyMMdd"));
        ui->endDTEdit->setTime(QTime::fromString("000000","HHmmss"));
    }
}

//===========================================================
// ��������on_moneyCBox_stateChanged
// ���ܣ����ݶ����ѽ�ѡ���ѡ���������������������������
// ���������ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void SearchDialog::on_moneyCBox_stateChanged(int arg1)
{
    // ѡ��
    if(arg1 == 2){
        ui->maxMoneyEdit->setReadOnly(false);
        ui->minMoneyEdit->setReadOnly(false);
    }
    // δѡ��
    else if(arg1 == 0){
        ui->maxMoneyEdit->setReadOnly(true);
        ui->minMoneyEdit->setReadOnly(true);
        ui->maxMoneyEdit->clear();
        ui->minMoneyEdit->clear();
    }
}

//===========================================================
// ��������on_backButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void SearchDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_searchButton_clicked
// ���ܣ����ര�ڽ����ϵ�������ť��click()�źŵĲۺ�����������������
//      ���趨����������������������Ϣ����������ʾ�ڴ��ڽ�������
//      tableWidget��
// �����������
// ����ֵ����
//===========================================================
void SearchDialog::on_searchButton_clicked()
{
    cout<<"on_searchButton_clicked"<<endl;
    clock_t startTime = clock();

    // �����¼Ϊ�� ����
    if(this->m_sortedRecords == nullptr || this->m_sortedRecords->next == nullptr)
        return;
    // ��ʼ�����ͷ���
    this->m_result = new sortedXf;
    // ������Ч����
    this->search();
    // ���±�
    this->updateTable();

    clock_t endTime = clock();
    cout<<"time of running the function: "<<double(endTime - startTime)/CLOCKS_PER_SEC<<"s"<<endl<<endl;
}

//===========================================================
// ��������updateTable
// ���ܣ��������������������������Ϣ��ʾ�ڴ��ڽ�������tableWidget��
// �����������
// ����ֵ����
//===========================================================
void SearchDialog::updateTable()
{
    auto CO = CardOpration::getinstance();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    ui->tableWidget->clear();
    // ����ͷ����
    ui->tableWidget->setColumnCount(5);
    QStringList head;
    head<<codec->toUnicode("ѧ��")
       <<codec->toUnicode("����")
      <<codec->toUnicode("����")
     <<codec->toUnicode("ʱ��")
    <<codec->toUnicode("���ѽ��");
    ui->tableWidget->setHorizontalHeaderLabels(head);

    ui->tableWidget->setRowCount(0);
    int i=0;
    auto p = this->m_result->next;
    while(p!=nullptr){
        ui->tableWidget->setRowCount(++i);
        // ѧ��
        ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(QString::fromStdString(p->stuNum)));
        // ����
        string name = CO->students[p->stuNum].name;
        ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(codec->toUnicode(name.c_str())));
        // ����
        string date = p->date.substr(0,4)+"-"+p->date.substr(4,2)+"-"+p->date.substr(6,2);
        ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(QString::fromStdString(date)));
        // ʱ��
        string time = p->time.substr(0,2)+":"+p->time.substr(2,2)+":"+p->time.substr(4,2)+"."+p->time.substr(6,2);
        ui->tableWidget->setItem(i-1,3,new QTableWidgetItem(QString::fromStdString(time)));
        // ���ѽ��
        ui->tableWidget->setItem(i-1,4,new QTableWidgetItem(QString::number(p->money,'f',2)));
        p = p->next;
    }
}

//===========================================================
// ��������search
// ���ܣ����������������趨����������������Ϣ������������Ҫ�����Ϣ��
//      ���뵽��������������
// �����������
// ����ֵ����
//===========================================================
void SearchDialog::search()
{
    string stuNum,name;
    int maxMoney,minMoney;

    // flag�����ִ�����Ϸ�ʽ
    // 0:�� 1:��� 2:���� 3:���+���� 4:ѧ��
    // 5:ѧ��+��� 6:ѧ��+���� 7:ѧ��+����+���
    int flag = 0;

    /* �����ѯ���ۺ����ѽ��Ļ� */
    if(ui->moneyCBox->isChecked()){
        minMoney = QString::number(ui->minMoneyEdit->text().toFloat()*100,'f',0).toInt();
        maxMoney = QString::number(ui->maxMoneyEdit->text().toFloat()*100,'f',0).toInt();
        if(minMoney>maxMoney) return;
        flag += 1;
    }
    /* �����ѯ���ۺ������Ļ� */
    if(ui->nameCBox->isChecked()){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        name = codec->fromUnicode(ui->nameEdit->text()).toStdString();
        if(name == "") return;
        flag += 2;
    }
    /* �����ѯ���ۺ�ѧ�ŵĻ� */
    if(ui->stuNumCBox->isChecked()){
        stuNum = ui->stuNumEdit->text().toStdString();
        if(stuNum == "") return;
        flag += 4;
    }

    /* �����ѯ���ۺ�ʱ��εĻ� */
    if(ui->DTCBox->isChecked()){
        // �ȶ�λ��ʱ���
        string startDT = ui->startDTEdit->dateTime().toString("yyyyMMddHHmmss").toStdString()+"00";
        string endDT = ui->endDTEdit->dateTime().toString("yyyyMMddHHmmss").toStdString()+"00";

        // ���������ָ��
        auto p = this->m_sortedRecords;

        // ���ʱ�����޴���ʱ������ ����
        if(startDT > endDT) return;
        // ���ʱ�����޴������ʱ�� ����
        else if(startDT > p->date) return;
        // ���ʱ������С����Сʱ�� ����
        else if(endDT < p->next->date+p->next->time)
            return;

        // �ȶ�λ�� p->date+p->time >= startDT ��p��
        p = p->next;
        while(p->date+p->time < startDT) p = p->next;

        this->jugeWithTime(this->m_sortedRecords->next,flag,maxMoney,minMoney,stuNum,name,endDT);

    }
    /* �����ѯû�����ۺ�ʱ��εĻ� */
    else{
        this->jugeNoTime(this->m_sortedRecords->next,flag,maxMoney,minMoney,stuNum,name);
    }
}

//===========================================================
// ��������jugeNoTime
// ���ܣ����������ĸ�����������û��ʱ�����Ƶ�����¶����������������
// �������������������Ϣ�������ʼ�����ڵ��ָ�룻����ѡ����ۺϱ�
//          ־��������С��ѧ�ţ�����
// ����ֵ����
//===========================================================
void SearchDialog::jugeNoTime(sortedXf* p,int flag,
         int maxMoney,int minMoney,string stuNum,string name)
{
    switch(flag){
    case 0:
        break;
    case 1: // ���
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 2: // ����
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            if(findOfName!=string::npos &&
                    findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 3: // ���+����
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            auto findOfName = p->name.find(name);
            if(num<=maxMoney && num>=minMoney &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 4: // ѧ��
        while(p!=nullptr){
            if(p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 5: // ѧ��+���
        while(p!=nullptr){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 6: // ѧ��+����
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            if(p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 7: // ѧ��+���+����
        while(p!=nullptr){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    }
}

//===========================================================
// ��������jugeWithTime
// ���ܣ����������ĸ�������������ʱ�����Ƶ�����¶����������������
// �������������������Ϣ�����ͷ�ڵ�ָ�룻����ѡ����ۺϱ�־������
//         ��С��ѧ�ţ�����
// ����ֵ����
//===========================================================
void SearchDialog::jugeWithTime(sortedXf *p, int flag, int maxMoney, int minMoney, string stuNum, string name, string endTime)
{
    switch(flag){
    case 0:
        while(p!=nullptr && p->date+p->time<=endTime){
            this->addToResult(p);
            p = p->next;
        }
        break;
    case 1: // ���
        while(p!=nullptr && p->date+p->time<=endTime){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 2: // ����
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            if(findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 3: // ���+����
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 4: // ѧ��
        while(p!=nullptr && p->date+p->time<=endTime){
            if(p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 5: // ѧ��+���
        while(p!=nullptr && p->date+p->time<=endTime){
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 6: // ѧ��+����
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            if(p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    case 7: // ѧ��+���+����
        while(p!=nullptr && p->date+p->time<=endTime){
            auto findOfName = p->name.find(name);
            int num = QString::number(p->money*100,'f',0).toInt();
            if(num<=maxMoney && num>=minMoney &&
                    p->stuNum.find(stuNum)!=string::npos &&
                    findOfName!=string::npos && findOfName%2==0){
                this->addToResult(p);
            }
            p = p->next;
        }
        break;
    }
}

//===========================================================
// ��������addToResult
// ���ܣ����ݷ���Ҫ������ѽڵ���Ϣ����ͷ�巨������Ϣ�����������
// �����������
// ����ֵ����
//===========================================================
void SearchDialog::addToResult(sortedXf *p)
{
    sortedXf *sp = new sortedXf(p->cardNum,p->stuNum,p->date,p->time,p->money,p->name,p->winNum);
    sp->next = this->m_result->next;
    this->m_result->next = sp;
}


