#include "ui_canteenappdialog.h"
#include "canteenappdialog.h"

//===========================================================
// ��������CanteenAppDialog
// ���ܣ�CanteenAppDialog��Ĺ��캯�������ʱ���������(dateTimeEdit)
//      �ĳ�ʼ����������µ�����
// ���������ʱ�����ڱ����������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CanteenAppDialog::CanteenAppDialog(QDateTime dt,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanteenAppDialog)
{
    ui->setupUi(this);

    // ��ʼ��DateTimeEdit��ʱ��
    ui->dateTimeEdit->setDateTime(dt);
    // ����ʱ��仯����
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // ÿ1ms�ͻᷢ���ź�timeout()
}

//===========================================================
// ��������~CanteenAppDialog
// ���ܣ�CanteenAppDialog���������������CanteenAppDialog��Ķ���
//      �ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CanteenAppDialog::~CanteenAppDialog()
{
    delete ui;
}

//===========================================================
// ��������on_goBackButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CanteenAppDialog::on_goBackButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_winTable_cellDoubleClicked
// ���ܣ����ര�ڽ����ϵ�winTable����ĵ�Ԫ��˫���źŵĲۺ�����������
//      �򿪵�Ԫ���Ӧ��ʳ�����Ѵ��ڽ���
// �����������ѡ�е�Ԫ������±꣬��ѡ�е�Ԫ������±�
// ����ֵ����
//===========================================================
void CanteenAppDialog::on_winTable_cellDoubleClicked(int row, int column)
{
    // ��ȡ��ѡ�е�Ԫ���Ӧ�Ĵ��ں�
    int winNum = row*9 + column + 1;
    // ����ʳ�����Ѵ��������
    WindowDialog m_win(winNum,ui->dateTimeEdit->dateTime());
    // ��ʳ�����Ѵ��ڽ���
    m_win.exec();
}

//===========================================================
// ��������timeUpdate
// ���ܣ����´��ڽ����е�dateTimeEdit�������ʾ����ʱ�䣬ÿ�θ��¼�1ms
// �����������
// ����ֵ����
//===========================================================
void CanteenAppDialog::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}
