#include "cardcontrol.h"
#include "ui_cardcontrol.h"

#include <QTextCodec>
#include <set>

//===========================================================
// ��������CardControl
// ���ܣ�CardControl��Ĺ��캯��������ѧ���������е�ѧ����Ϣ����
//      ���ڽ�������stuTable��ͬʱ���ʱ���������(dateTimeEdit)
//      �ĳ�ʼ��������µ�����
// ���������ʱ�����ڱ����������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CardControl::CardControl(QDateTime dt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardControl)
{
    ui->setupUi(this);

    // ����Table����
    auto CO = CardOpration::getinstance();
    this->updateTableWideget(CO->students);

    // ��ʼ��DateTimeEdit��ʱ��
    ui->dateTimeEdit->setDateTime(dt);
    // ����ʱ��仯����
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // ÿ1ms�ͻᷢ���ź�timeout()
}

//===========================================================
// ��������~CardControl
// ���ܣ�CardControl���������������CardControl��Ķ����ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CardControl::~CardControl()
{
    delete ui;
}

//===========================================================
// ��������updateTableWideget
// ���ܣ����¸���Ĵ��ڽ����ѧ����Ϣ��ʾ���stuTable������ʾ����Ϣ
// �����������Ҫ��ʾ�����е�ѧ����Ϣ�ļ�ֵ�Լ���
// ����ֵ����
//===========================================================
void CardControl::updateTableWideget(map<string,Student> students)
{
    // ��������ڴ��ڽ�������ʾ���������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ����ձ�ͷ
    ui->stuTable->clear();
    // ���ñ������
    ui->stuTable->setColumnCount(4);

    // ���ñ�ͷ����
    QStringList head;
    head<<codec->toUnicode("ѧ��")
       <<codec->toUnicode("����")
      <<codec->toUnicode("����")
     <<codec->toUnicode("�˻����");
    // �������stuTable�ı�ͷ
    ui->stuTable->setHorizontalHeaderLabels(head);

    // �������stuTable����ʾ��Ϣ����
    int size = students.size();
    ui->stuTable->setRowCount(size);

    int i=0;
    for(auto it = students.begin();i<size;it++,i++){
        // ���ѧ��������
        if(it->second.status){
            i--;
            size--;
            // ���������Ҫ��һ
            ui->stuTable->setRowCount(size);
            continue;
        }
        // ѧ��
        ui->stuTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it->first)));
        // ����
        ui->stuTable->setItem(i,1,new QTableWidgetItem(codec->toUnicode(it->second.name.c_str())));
        // ����
        if(it->second.hasCard){
            ui->stuTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it->second.cardhead->next->number)));
        }
        // �˻����
        ui->stuTable->setItem(i,3,new QTableWidgetItem(QString::number(it->second.money,'f',2)));
    }

}

//===========================================================
// ��������matchWithNum
// ���ܣ�ѧ��ѧ�ŵ�ģ��ƥ�䣬inputΪ�������ݣ�����*��������0-n��
//      �ַ�,?��������һ���ַ�����stuNumΪ���ȶԵ�ѧ�ţ�����input��
//      stuNum��ƥ�䲢����ƥ����
// ������������ȶ�ѧ�ţ���������
// ����ֵ��ƥ��ɹ����
//===========================================================
bool CardControl::matchWithNum(string stuNum, string input){
    // input�ַ����ĳ���
    int inputLength = input.size();
    // stuNum�ַ����ĳ���
    int stuNumLength = stuNum.size();
    int i=0; // stuNum������
    int j=0; // input������
    // stuNum���*�ı��
    int maski = -1;
    // input���*�ı��
    int maskj = -1;

    // ��stuNum��û�ж���
    while(i<stuNumLength){
        // ���input��û�ж��� �� (stuNum��input��ǰ�ַ���ͬ �� input��ǰ�ַ�Ϊ?)
        // ��ǰ�ַ�ƥ��ɹ�
        if(j!=inputLength && (stuNum[i] == input[j] || input[j] == '?')){
            i++;j++;
        }
        // ���input��û�ж��� �� input��ǰ�ַ�Ϊ*
        else if(j!=inputLength && (input[j] =='*')){
            // ���µ�ǰinput��stuNum�ȶԵ���λ��
            maskj = j;
            maski = i;
            // ��input*����һ���ַ���stuNum�ĵ�ǰ�ַ�����ƥ��
            j++;
        }
        // ƥ��ʧ��ʱ�����ǲ�Ϊ��
        else if(maskj!=-1){
            // *������ַ����ټ�һ
            i = maski + 1;
            j= maskj + 1;
            maski++;
        }
        // ƥ��ʧ���ұ��Ϊ��
        else{
            // ����ʧ��
            return false;
        }
    }

    // ���stuNum�Ѿ���ȡ��� input��δ��ȡ���
    // ���ҽ���input��ʣ��δ���ַ���Ϊ*ʱ����ƥ��ɹ�
    while(j<inputLength){
        if(input[j] =='*') j++;
        else return false;
    }
    return true;
}

//===========================================================
// ��������matchWithName
// ���ܣ�ѧ��������ģ��ƥ�䣬inputΪ�������ݣ�����?��������һ���ַ�����
//      nameΪ���ȶԵ�����������input��stuNum��ƥ�䲢����ƥ����
// ������������ȶ���������������
// ����ֵ��ƥ��ɹ����
//===========================================================
bool CardControl::matchWithName(string name, string input)
{
    // ����ַ���������?�� ֻҪinput��name���Ӵ�����
    if(input.find('?') == string::npos){
        if(name.find(input)!=string::npos && name.find(input)%2==0){
            return true;
        }
        else return false;
    }
    // ����ַ�������?�� ֻ��һ��һ(?��������һ������)
    else{
        int i=0; // name������
        int j=0; // input������
        int nameLength = name.size(); // name�ĳ���
        int inputLength = input.size(); // input�ĳ���
        // ��name��δ��ȡ��ʱ
        while(i<nameLength){
            // ���input��û�ж��� �� input��ǰ�ַ�Ϊ*
            if(j<inputLength && input[j]=='?')
            {
                j++;
                i+=2;
            }
            // ���input��δ���� �� input��ǰ�ַ���name�ĵ�ǰ�ַ���ͬ
            else if(j<inputLength && input.substr(j,2) == name.substr(i,2))
            {
                i+=2;
                j+=2;
            }
            // ����ƥ��hʧ��
            else return false;
        }
        // ���input�Ѿ����� �� name��û�ж��� ��ƥ��ʧ��
        if(j<inputLength) return false;
        // ��֮ƥ��ɹ�
        else return true;
    }
}

//===========================================================
// ��������on_goBack_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_goBack_clicked()
{
    this->close();
}

//===========================================================
// ��������on_createStu_clicked
// ���ܣ����ര�ڽ����ϵĿ�����ť��click()�źŵĲۺ�������ʾ�������ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_createStu_clicked()
{
    // ��ʾ�������ڽ���
    CreateStu createstu;
    createstu.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_deleteStu_clicked
// ���ܣ����ര�ڽ����ϵĿ�����ť��click()�źŵĲۺ�������ʾ�������ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_deleteStu_clicked()
{
    // ��ʾ�������ڽ���
    inPutStuNumDialog inputStuNum(4);
    inputStuNum.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_recharge_clicked
// ���ܣ����ര�ڽ����ϵĳ�ֵ��ť��click()�źŵĲۺ�������ʾ��ֵ���ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_recharge_clicked()
{
    // ��ʾ��ֵ���ڽ���
    RechargeDialog recharge;
    recharge.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_reportLoss_clicked
// ���ܣ����ര�ڽ����ϵĹ�ʧ��ť��click()�źŵĲۺ�������ʾ��ʧ���ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_reportLoss_clicked()
{
    // ��ʾ��ʧ���ڽ���
    inPutStuNumDialog inputStuNum(2);
    inputStuNum.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_addCard_clicked
// ���ܣ����ര�ڽ����ϵĲ�����ť��click()�źŵĲۺ�������ʾ�������ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_addCard_clicked()
{
    // ��ʾ�������ڽ���
    inPutStuNumDialog inputStuNum(1);
    inputStuNum.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_cancleFreeze_clicked
// ���ܣ����ര�ڽ����ϵĽ�Ұ�ť��click()�źŵĲۺ�������ʾ��Ҵ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_cancleFreeze_clicked()
{
    // ��ʾ��Ҵ��ڽ���
    inPutStuNumDialog inputStuNum(3);
    inputStuNum.exec();
    // ����ѧ����Ϣ��ʾ����ϵ�ѧ����Ϣ
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_stuTable_itemDoubleClicked
// ���ܣ����ര�ڽ����ϵ�stuTable����ĵ�Ԫ��˫���źŵĲۺ�������ʾ
//      ˫�������е�ѧ������Ϣ��ʾ����
// �����������˫���ĵ�Ԫ���ָ��
// ����ֵ����
//===========================================================
void CardControl::on_stuTable_itemDoubleClicked(QTableWidgetItem *item)
{
    // ѡ�е�Ԫ�������
    int i = item->row();
    // ��ȡѡ���еĵ�ѧ��ѧ��
    string stuNum = ui->stuTable->item(i,0)->text().toStdString();
    // ��ʾѧ����Ϣ����
    ShowStuDialog show(stuNum);
    show.exec();
}

//===========================================================
// ��������timeUpdate
// ���ܣ����´��ڽ����е�dateTimeEdit�������ʾ����ʱ�䣬ÿ�θ��¼�1ms
// �����������
// ����ֵ����
//===========================================================
void CardControl::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}

//===========================================================
// ��������on_restoreBtn_clicked
// ���ܣ��ָ��������stuTable�ϵ���ʾ��ϢΪϵͳ������ѧ������Ϣ
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_restoreBtn_clicked()
{
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// ��������on_stuNumCBox_stateChanged
// ���ܣ�����ѧ�Ź�ѡ���ѡ����������������ť��������ѡ�������
// ���������ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void CardControl::on_stuNumCBox_stateChanged(int arg1)
{
    // ��ձ༭��
    ui->lineEdit->clear();

    // ѡ��
    if(arg1 == 2){
        // ���ñ༭�п�������
        ui->lineEdit->setReadOnly(false);

        // ���ñ༭��ֻ����������
        ui->lineEdit->setMaxLength(10);
        QRegExp regx1("[0-9?*]+$");
        QValidator *validator1 = new QRegExpValidator(regx1,ui->lineEdit);
        ui->lineEdit->setValidator(validator1);

        // �����������ѡ��ȡ�����Ĺ�ѡ
        if(ui->nameCBox->isChecked()){
            ui->nameCBox->setCheckState(Qt::CheckState(0));
        }
    }
    // δѡ��
    else if(arg1 == 0){
        // �������Ҳδ��ѡ ���ñ༭�в������� ��ԭѧ����
        if(!ui->nameCBox->isChecked()){
            ui->lineEdit->setReadOnly(true);
            updateTableWideget(CardOpration::getinstance()->students);
        }
    }
}

//===========================================================
// ��������on_nameCBox_stateChanged
// ���ܣ�����������ѡ���ѡ����������������ť��ѧ�Ź�ѡ�������
// ��������� ѡ���źţ�2��ʾѡ��0��ʾȡ��ѡ��
// ����ֵ����
//===========================================================
void CardControl::on_nameCBox_stateChanged(int arg1)
{
    // ��ձ༭��
    ui->lineEdit->clear();

    // ѡ��
    if(arg1 == 2){
        // ���ñ༭�п�������
        ui->lineEdit->setReadOnly(false);

        // ���ñ༭��ֻ����������
        ui->lineEdit->setMaxLength(4);
        QRegExp regx2("[\u4e00-\u9fa5?]+$");
        QValidator *validator2 = new QRegExpValidator(regx2,ui->lineEdit);
        ui->lineEdit->setValidator(validator2);

        // ���ѧ�ű���ѡ��ȡ�����Ĺ�ѡ
        if(ui->stuNumCBox->isChecked()){
            ui->stuNumCBox->setCheckState(Qt::CheckState(0));
        }
    }
    // δѡ��
    else if(arg1 == 0){
        // ���ѧ��Ҳδ��ѡ ���ñ༭�в������� ����ԭѧ����
        if(!ui->stuNumCBox->isChecked()){
            ui->lineEdit->setReadOnly(true);
            updateTableWideget(CardOpration::getinstance()->students);
        }
    }
}

//===========================================================
// ��������on_searchBtn_clicked
// ���ܣ����ര�ڽ����ϵ�������ť��click()�źŵĲۺ�������������lineEdit
//      ��������ƥ���ѧ����������ʾ�ڴ������stuTable��
// �����������
// ����ֵ����
//===========================================================
void CardControl::on_searchBtn_clicked()
{
    // ����Ҫ�����Ϣ <ѧ��,ѧ��>
    map<string,Student> info;

    // ��ȡȫ��ѧ��������Ϣ���ָ��
    auto CO = CardOpration::getinstance();

    // �����ѧ������
    if(ui->stuNumCBox->isChecked()){
        // ��ȡlineEdit�ϵ�������Ϣ��ѧ�ţ�
        string stuNum = ui->lineEdit->text().toStdString();
        // ����ѧ������ϵͳ�е�����ѧ��
        auto it = CO->students.begin();
        auto end = CO->students.end();
        while(it!=end){
            // ���ѧ��ƥ��ɹ� ��ѧ����Ϣ����info��
            if(matchWithNum(it->first,stuNum)){
                info[it->first] = it->second;
            }
            it++;
        }
        // ���´��ڽ������stuTable����ʾ��ѧ��Ϊ��������Ҫ���ѧ��
        updateTableWideget(info);
    }

    // �������������
    else if(ui->nameCBox->isChecked()){
        // ����ڴ�����ʾ���������������
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        // ��ȡlineEdit�ϵ�������Ϣ��������
        string name = codec->fromUnicode(ui->lineEdit->text()).toStdString();
        // ����ѧ������ϵͳ�е�����ѧ��
        auto it = CO->students.begin();
        auto end = CO->students.end();
        while(it!=end){
            // �������ƥ��ɹ� ��ѧ����Ϣ����info��
            if(matchWithName(it->second.name,name)){
                info[it->first] = it->second;
            }
            it++;
        }
        // ���´��ڽ������stuTable����ʾ��ѧ��Ϊ��������Ҫ���ѧ��
        updateTableWideget(info);
    }
    // ���������δ��ѡ ����������Ч
}
