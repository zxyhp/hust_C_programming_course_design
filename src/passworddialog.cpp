#include "passworddialog.h"
#include "ui_passworddialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// ��������PasswordDialog
// ���ܣ�PasswordDialog��Ĺ��캯�������ô��ڽ����������������������
// ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    // ��������ֻ�������� 4λ����
    ui->passwordEdit->setMaxLength(4);
    QRegExp regx("[0-9]+$");
    QValidator* validator = new QRegExpValidator(regx,ui->passwordEdit);
    ui->passwordEdit->setValidator(validator);
}

//===========================================================
// ��������~PasswordDialog
// ���ܣ�PasswordDialog���������������PasswordDialog��Ķ����ͷ�ʱ
//      �ͷ�ui
// �����������
// ����ֵ����
//===========================================================
PasswordDialog::~PasswordDialog()
{
    delete ui;
}

//===========================================================
// ��������changeTitle
// ���ܣ��ı䴰�����ͱ����ǩ������
// ����������´��������±����ǩ����
// ����ֵ����
//===========================================================
void PasswordDialog::changeTitle(QString winTitle, QString labelStr)
{
    this->setWindowTitle(winTitle);
    ui->titleLable->setText(winTitle);
    ui->passwordLabel->setText(labelStr);
}

//===========================================================
// ��������on_cancelButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void PasswordDialog::on_cancelButton_clicked()
{
    sendData("");
    ui->passwordEdit->clear();
    this->close();
}

//===========================================================
// ��������on_okButton_clicked
// ���ܣ�������������ݽ��������봫�������ڵĳ�Ա����
// �����������
// ����ֵ����
//===========================================================
void PasswordDialog::on_okButton_clicked()
{
    // ����������볤��С��4
    if(ui->passwordEdit->text().size()<4){
        // ��������ַ��ڴ��ڽ��������������
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�������ʽ��ȷ�����룡"));
        return;
    }

    // �������ݵ��ź� ����Ӧ�Ĳۿ��Ըı丸�����е������Ա����
    sendData(ui->passwordEdit->text());
    ui->passwordEdit->clear();
    this->close();
}
