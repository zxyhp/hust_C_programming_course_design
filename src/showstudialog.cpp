#include "setdatetime.h"
#include "showstudialog.h"
#include "ui_showstudialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// ��������ShowStuDialog
// ���ܣ�ShowStuDialog��Ĺ��캯������ʼ����Ա����m_stuNum�����ݴ�
//      ��ѧ��ѧ�Ÿ��´��ڽ�����Ϣ
// ���������ѧ��ѧ�ţ������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
ShowStuDialog::ShowStuDialog(string stuNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowStuDialog)
{
    ui->setupUi(this);
    this->m_stuNum = stuNum;
    this->updateTableWidget();
}

//===========================================================
// ��������~ShowStuDialog
// ���ܣ�ShowStuDialog���������������ShowStuDialog��Ķ����ͷ�
//      ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
ShowStuDialog::~ShowStuDialog()
{
    delete ui;
}

//===========================================================
// ��������updateTableWidget
// ���ܣ����ݴ���ѧ�Ÿ��´��ڽ����������ʾ�Ĵ�����Ϣ
// �����������
// ����ֵ����
//===========================================================
void ShowStuDialog::updateTableWidget(){

    // ��������ַ��ڴ������������������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ��ȡ����ѧ�ŵ�ѧ����������Ϣ
    auto CO = CardOpration::getinstance();
    string name = CO->students[m_stuNum].name;
    float money = CO->students[m_stuNum].money;
    int numOfCard = CO->students[m_stuNum].numOfCard;

    // ������Ϣ��ǩ����
    ui->stuNumLabel->setText(QString::fromStdString(m_stuNum));
    ui->nameLabel->setText(codec->toUnicode(name.c_str()));
    ui->moneyLabel->setText(QString::number(money,'f',2));

    // ���ÿ�Ƭ��ʾ��ı�ͷ
    ui->cardTable->clear();
    ui->cardTable->setColumnCount(2);
    QStringList head;
    head<<codec->toUnicode("����")<<codec->toUnicode("״̬");
    ui->cardTable->setHorizontalHeaderLabels(head);

    // ���ÿ�Ƭ��ʾ�������
    ui->cardTable->setRowCount(numOfCard);

    // ����ѧ���������������� ��������ӵ���Ƭ��ʾ����
    int i=0;
    CardNum *p = CO->students[m_stuNum].cardhead;
    QString status[]={codec->toUnicode("ʹ����"),codec->toUnicode("������")};
    while(p->next!=NULL){
        p = p->next;
        ui->cardTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(p->number)));
        ui->cardTable->setItem(i,1,new QTableWidgetItem(status[CO->cards[p->number].status]));
        i++;
    }
}

//===========================================================
// ��������on_closeButton_clicked
// ���ܣ����ര�ڽ����ϵĹرհ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void ShowStuDialog::on_closeButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_ChangePasswordBtn_clicked
// ���ܣ����ര�ڽ����ϵ��޸����밴ť��click()�źŵĲۺ���������
//      PasswordDialog���Ӧ�Ĵ������޸�����
// �����������
// ����ֵ����
//===========================================================
void ShowStuDialog::on_ChangePasswordBtn_clicked()
{
    // ��ֹ��������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ��ȡʱ��
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // ������־���� ���ں���д���������־
    OprationLog oplog;
    // �޸����������־�������
    // date,time,�޸�����,ѧ��,�ɹ�
    // date,time,�޸�����,ѧ��,ʧ��,ʧ��ԭ��
    string oplogStr = date + "," + time + "," + "�޸�����"
            + "," + this->m_stuNum + ",";

    // �������봰��
    PasswordDialog* PD = new PasswordDialog;
    PD->changeTitle(codec->toUnicode("������֤"),codec->toUnicode("������"));
    // ���Ӳ� ����������벢ȷ�� �� this->m_password�ͻ��ȡ�����������
    connect(PD,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    PD->exec();

    auto CO = CardOpration::getinstance();

    // ���ȡ�����޸�
    if(this->m_password == "") ;
    // ����������������
    else if(this->m_password != CO->students[this->m_stuNum].password){
        oplogStr += "ʧ��,��������֤����\n";
        oplog.writeOpLog(oplogStr);
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�����޸�ʧ��!"));
    }
    // ���������������ȷ
    else{
        // ��ȡ������
        PD->changeTitle(codec->toUnicode("�޸�����"),codec->toUnicode("������"));
        PD->exec();
        // �����ȡ����������
        if(this->m_password != ""){
            oplogStr += "�ɹ�\n";
            oplog.writeOpLog(oplogStr);
            // �޸�����
            CO->students[this->m_stuNum].password = this->m_password;
            QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�����޸ĳɹ�!"));
        }
    }
}

//===========================================================
// ��������receiveData
// ���ܣ����ڽ����������봰�����������������ַ�������Ա����m_password��
// ����������������봰�����������������ַ���
// ����ֵ����
//===========================================================
void ShowStuDialog::receiveData(QString date)
{
    this->m_password = date.toStdString();
}
