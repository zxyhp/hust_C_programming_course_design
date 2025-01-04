#include "cardcontrol.h"
#include "ui_cardcontrol.h"

#include <QTextCodec>
#include <set>

//===========================================================
// 函数名：CardControl
// 功能：CardControl类的构造函数，根据学生管理类中的学生信息更新
//      窗口界面的组件stuTable，同时完成时间日期组件(dateTimeEdit)
//      的初始化及其更新的设置
// 输入参数：时间日期变量；父窗口部件的指针（没有父窗口则会传入nullptr）
// 返回值：空
//===========================================================
CardControl::CardControl(QDateTime dt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardControl)
{
    ui->setupUi(this);

    // 更新Table内容
    auto CO = CardOpration::getinstance();
    this->updateTableWideget(CO->students);

    // 初始化DateTimeEdit的时间
    ui->dateTimeEdit->setDateTime(dt);
    // 建立时间变化设置
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // 每1ms就会发射信号timeout()
}

//===========================================================
// 函数名：~CardControl
// 功能：CardControl类的析构函数，在CardControl类的对象释放时释放ui
// 输入参数：空
// 返回值：空
//===========================================================
CardControl::~CardControl()
{
    delete ui;
}

//===========================================================
// 函数名：updateTableWideget
// 功能：更新该类的窗口界面的学生信息显示组件stuTable中所显示的信息
// 输入参数：需要显示到表中的学生信息的键值对集合
// 返回值：空
//===========================================================
void CardControl::updateTableWideget(map<string,Student> students)
{
    // 解决中文在窗口界面上显示乱码的问题
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // 先清空表头
    ui->stuTable->clear();
    // 设置表的列数
    ui->stuTable->setColumnCount(4);

    // 设置表头内容
    QStringList head;
    head<<codec->toUnicode("学号")
       <<codec->toUnicode("姓名")
      <<codec->toUnicode("卡号")
     <<codec->toUnicode("账户余额");
    // 设置组件stuTable的表头
    ui->stuTable->setHorizontalHeaderLabels(head);

    // 设置组件stuTable的显示信息行数
    int size = students.size();
    ui->stuTable->setRowCount(size);

    int i=0;
    for(auto it = students.begin();i<size;it++,i++){
        // 如果学生已销户
        if(it->second.status){
            i--;
            size--;
            // 组件的行数要减一
            ui->stuTable->setRowCount(size);
            continue;
        }
        // 学号
        ui->stuTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(it->first)));
        // 姓名
        ui->stuTable->setItem(i,1,new QTableWidgetItem(codec->toUnicode(it->second.name.c_str())));
        // 卡号
        if(it->second.hasCard){
            ui->stuTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(it->second.cardhead->next->number)));
        }
        // 账户余额
        ui->stuTable->setItem(i,3,new QTableWidgetItem(QString::number(it->second.money,'f',2)));
    }

}

//===========================================================
// 函数名：matchWithNum
// 功能：学生学号的模糊匹配，input为输入内容（其中*代表任意0-n个
//      字符,?代表任意一个字符），stuNum为待比对的学号，进行input和
//      stuNum的匹配并返回匹配结果
// 输入参数：待比对学号；输入内容
// 返回值：匹配成功与否
//===========================================================
bool CardControl::matchWithNum(string stuNum, string input){
    // input字符串的长度
    int inputLength = input.size();
    // stuNum字符串的长度
    int stuNumLength = stuNum.size();
    int i=0; // stuNum的索引
    int j=0; // input的索引
    // stuNum针对*的标记
    int maski = -1;
    // input针对*的标记
    int maskj = -1;

    // 当stuNum还没有读完
    while(i<stuNumLength){
        // 如果input还没有读完 且 (stuNum和input当前字符相同 或 input当前字符为?)
        // 则当前字符匹配成功
        if(j!=inputLength && (stuNum[i] == input[j] || input[j] == '?')){
            i++;j++;
        }
        // 如果input还没有读完 且 input当前字符为*
        else if(j!=inputLength && (input[j] =='*')){
            // 记下当前input、stuNum比对到的位置
            maskj = j;
            maski = i;
            // 将input*的下一个字符和stuNum的当前字符进行匹配
            j++;
        }
        // 匹配失败时如果标记不为空
        else if(maskj!=-1){
            // *代表的字符数再加一
            i = maski + 1;
            j= maskj + 1;
            maski++;
        }
        // 匹配失败且标记为空
        else{
            // 返回失败
            return false;
        }
    }

    // 如果stuNum已经读取完毕 input还未读取完毕
    // 当且仅当input的剩余未读字符都为*时才能匹配成功
    while(j<inputLength){
        if(input[j] =='*') j++;
        else return false;
    }
    return true;
}

//===========================================================
// 函数名：matchWithName
// 功能：学生姓名的模糊匹配，input为输入内容（其中?代表任意一个字符），
//      name为待比对的姓名，进行input和stuNum的匹配并返回匹配结果
// 输入参数：待比对姓名；输入内容
// 返回值：匹配成功与否
//===========================================================
bool CardControl::matchWithName(string name, string input)
{
    // 如果字符串不包含?号 只要input是name的子串即可
    if(input.find('?') == string::npos){
        if(name.find(input)!=string::npos && name.find(input)%2==0){
            return true;
        }
        else return false;
    }
    // 如果字符串包含?号 只能一对一(?代表任意一个汉字)
    else{
        int i=0; // name的索引
        int j=0; // input的索引
        int nameLength = name.size(); // name的长度
        int inputLength = input.size(); // input的长度
        // 当name还未读取完时
        while(i<nameLength){
            // 如果input还没有读完 且 input当前字符为*
            if(j<inputLength && input[j]=='?')
            {
                j++;
                i+=2;
            }
            // 如果input还未读完 且 input当前字符与name的当前字符相同
            else if(j<inputLength && input.substr(j,2) == name.substr(i,2))
            {
                i+=2;
                j+=2;
            }
            // 其他匹配h失败
            else return false;
        }
        // 如果input已经读完 而 name还没有读完 则匹配失败
        if(j<inputLength) return false;
        // 反之匹配成功
        else return true;
    }
}

//===========================================================
// 函数名：on_goBack_clicked
// 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_goBack_clicked()
{
    this->close();
}

//===========================================================
// 函数名：on_createStu_clicked
// 功能：该类窗口界面上的开户按钮的click()信号的槽函数，显示开户窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_createStu_clicked()
{
    // 显示开户窗口界面
    CreateStu createstu;
    createstu.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_deleteStu_clicked
// 功能：该类窗口界面上的开户按钮的click()信号的槽函数，显示销户窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_deleteStu_clicked()
{
    // 显示销户窗口界面
    inPutStuNumDialog inputStuNum(4);
    inputStuNum.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_recharge_clicked
// 功能：该类窗口界面上的充值按钮的click()信号的槽函数，显示充值窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_recharge_clicked()
{
    // 显示充值窗口界面
    RechargeDialog recharge;
    recharge.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_reportLoss_clicked
// 功能：该类窗口界面上的挂失按钮的click()信号的槽函数，显示挂失窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_reportLoss_clicked()
{
    // 显示挂失窗口界面
    inPutStuNumDialog inputStuNum(2);
    inputStuNum.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_addCard_clicked
// 功能：该类窗口界面上的补卡按钮的click()信号的槽函数，显示补卡窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_addCard_clicked()
{
    // 显示补卡窗口界面
    inPutStuNumDialog inputStuNum(1);
    inputStuNum.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_cancleFreeze_clicked
// 功能：该类窗口界面上的解挂按钮的click()信号的槽函数，显示解挂窗口界面
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_cancleFreeze_clicked()
{
    // 显示解挂窗口界面
    inPutStuNumDialog inputStuNum(3);
    inputStuNum.exec();
    // 更新学生信息显示组件上的学生信息
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_stuTable_itemDoubleClicked
// 功能：该类窗口界面上的stuTable组件的单元格双击信号的槽函数，显示
//      双击所在行的学生的信息显示界面
// 输入参数：被双击的单元格的指针
// 返回值：空
//===========================================================
void CardControl::on_stuTable_itemDoubleClicked(QTableWidgetItem *item)
{
    // 选中单元格的行数
    int i = item->row();
    // 获取选中行的的学生学号
    string stuNum = ui->stuTable->item(i,0)->text().toStdString();
    // 显示学生信息界面
    ShowStuDialog show(stuNum);
    show.exec();
}

//===========================================================
// 函数名：timeUpdate
// 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加1ms
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}

//===========================================================
// 函数名：on_restoreBtn_clicked
// 功能：恢复窗口组件stuTable上的显示信息为系统内所有学生的信息
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_restoreBtn_clicked()
{
    auto CO = CardOpration::getinstance();
    updateTableWideget(CO->students);
}

//===========================================================
// 函数名：on_stuNumCBox_stateChanged
// 功能：根据学号勾选框的选择更新输入框、搜索按钮和姓名勾选框的设置
// 输入参数：选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void CardControl::on_stuNumCBox_stateChanged(int arg1)
{
    // 清空编辑行
    ui->lineEdit->clear();

    // 选中
    if(arg1 == 2){
        // 设置编辑行可以输入
        ui->lineEdit->setReadOnly(false);

        // 设置编辑行只能输入数字
        ui->lineEdit->setMaxLength(10);
        QRegExp regx1("[0-9?*]+$");
        QValidator *validator1 = new QRegExpValidator(regx1,ui->lineEdit);
        ui->lineEdit->setValidator(validator1);

        // 如果姓名被勾选则取消它的勾选
        if(ui->nameCBox->isChecked()){
            ui->nameCBox->setCheckState(Qt::CheckState(0));
        }
    }
    // 未选中
    else if(arg1 == 0){
        // 如果姓名也未勾选 设置编辑行不能输入 还原学生表
        if(!ui->nameCBox->isChecked()){
            ui->lineEdit->setReadOnly(true);
            updateTableWideget(CardOpration::getinstance()->students);
        }
    }
}

//===========================================================
// 函数名：on_nameCBox_stateChanged
// 功能：根据姓名勾选框的选择更新输入框、搜索按钮和学号勾选框的设置
// 输入参数： 选择信号（2表示选择，0表示取消选择）
// 返回值：空
//===========================================================
void CardControl::on_nameCBox_stateChanged(int arg1)
{
    // 清空编辑行
    ui->lineEdit->clear();

    // 选中
    if(arg1 == 2){
        // 设置编辑行可以输入
        ui->lineEdit->setReadOnly(false);

        // 设置编辑行只能输入中文
        ui->lineEdit->setMaxLength(4);
        QRegExp regx2("[\u4e00-\u9fa5?]+$");
        QValidator *validator2 = new QRegExpValidator(regx2,ui->lineEdit);
        ui->lineEdit->setValidator(validator2);

        // 如果学号被勾选则取消它的勾选
        if(ui->stuNumCBox->isChecked()){
            ui->stuNumCBox->setCheckState(Qt::CheckState(0));
        }
    }
    // 未选中
    else if(arg1 == 0){
        // 如果学号也未勾选 设置编辑行不能输入 并还原学生表
        if(!ui->stuNumCBox->isChecked()){
            ui->lineEdit->setReadOnly(true);
            updateTableWideget(CardOpration::getinstance()->students);
        }
    }
}

//===========================================================
// 函数名：on_searchBtn_clicked
// 功能：该类窗口界面上的搜索按钮的click()信号的槽函数，搜索出与lineEdit
//      输入内容匹配的学生并将其显示在窗口组件stuTable上
// 输入参数：空
// 返回值：空
//===========================================================
void CardControl::on_searchBtn_clicked()
{
    // 符合要求的信息 <学号,学生>
    map<string,Student> info;

    // 获取全局学生管理信息类的指针
    auto CO = CardOpration::getinstance();

    // 如果按学号搜索
    if(ui->stuNumCBox->isChecked()){
        // 获取lineEdit上的输入信息（学号）
        string stuNum = ui->lineEdit->text().toStdString();
        // 遍历学生管理系统中的所有学生
        auto it = CO->students.begin();
        auto end = CO->students.end();
        while(it!=end){
            // 如果学号匹配成功 则将学生信息加入info中
            if(matchWithNum(it->first,stuNum)){
                info[it->first] = it->second;
            }
            it++;
        }
        // 更新窗口界面组件stuTable上显示的学生为符合搜索要求的学生
        updateTableWideget(info);
    }

    // 如果按姓名搜索
    else if(ui->nameCBox->isChecked()){
        // 解决在窗口显示界面上乱码的问题
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        // 获取lineEdit上的输入信息（姓名）
        string name = codec->fromUnicode(ui->lineEdit->text()).toStdString();
        // 遍历学生管理系统中的所有学生
        auto it = CO->students.begin();
        auto end = CO->students.end();
        while(it!=end){
            // 如果姓名匹配成功 则将学生信息加入info中
            if(matchWithName(it->second.name,name)){
                info[it->first] = it->second;
            }
            it++;
        }
        // 更新窗口界面组件stuTable上显示的学生为符合搜索要求的学生
        updateTableWideget(info);
    }
    // 如果两个都未勾选 搜索按键无效
}
