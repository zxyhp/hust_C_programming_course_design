## 程序设计综合课程设计

### 智慧校园之校园卡食堂应用模拟实现

1. 可直接运行可执行程序包（bin）内的CampusCard.exe程序。
2. 程序涉及的几个导入数据包（students.txt，wz.txt，cz.txt，xf.txt）需要放在程序的同级目录下。
3. 运行程序后产生的几个数据文件也在该程序同级目录下：oplog.txt（操作日志数据文件），xf_all.txt（汇总分析模块运行后的有序消费记录数据文件(加密)）。
4. 建议进行其他功能测试前，先导入批量数据，此处需要等待1-2分钟。
5. 学生开户、销户、解挂、挂失、补卡、充值功能在卡片管理模块，双击学生行可以查看学生的信息（主要可查看学生的历代卡片信息，修改学生消费密码，初始密码“8888”）。
6. 卡片管理模块还提供模糊查询功能，需要进行模糊查询前先勾选学号或姓名查询。学号查询时输入框仅可输入*、?、0-9，此处匹配模式即通配符匹配；姓名查询时输入框仅可输入汉字和?，此处匹配为含?则一对一匹配，?代表一个汉字，若不含?则为子串匹配。（仅可输入的意思不是认为控制，输入框会限制输入信息，即当选择姓名匹配时，无法输入英文等字符）。
7. 进入食堂应用窗口后双击任意窗口格则会弹出窗口界面，可进行消费和查看消费记录功能。食堂消费只能在限制时段内才能消费成功（早上:6-8点；中午:11-13点；晚上:17-19点）。
8. 点击汇总分析模块，打开其窗口会停顿一小会儿（进行消费记录的排序和数据文件的写入），里面提供查询、统计、分析、校验功能。
9. 校验功能的使用为，到程序的同级目录下找到xf_all.txt文件，从里面任意删去或增加行后保存，再进行检测。
10. 查询功能中的学号匹配和姓名匹配都是简单的子串匹配。
11. 全局的时间仅可在初始窗口界面的日期时间组件上修改（不建议将时间回改）。
12. 所有准确学号输入框都设置了只能输入0-9，且最大长度为10；所有的准确姓名输入框都设置了只能输入汉字，最多输入四个汉字。
13. 充值金额输入框设置了只能输入1-999的整数；消费金额输入框设置了只能输入0.00-99.99的两位小数的浮点数。
14. 运行一个窗口后，不能在对其他窗口进行操作，除非关闭该窗口，则可对其父窗口进行操作（窗口运行用的都是exec()不是show()）。
