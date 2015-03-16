# 文档简介 #
绿色时间管理内部数据库Task.db的结构，方便开发者理解GreenTimer的实现

# 概述 #
绿色时间管理将所有数据都保存在一个名为Task.db的桌面数据库中。此文件与主执行程序在同一目录下。在托盘右键菜单中点击 “打开task.db所在目录” ，可以找到此文件。

Task.db是一个使用Sqlite3创建的桌面数据库，可以直接使用第三方软件如sqlite3.exe等打开和编辑。sqlite3.exe在svn中能找到。

Task.db数据库内部包含四张表,分别名为：设置表，日志表，提醒表，任务安排表。其完整SQL定义如下：

  * CREATE TABLE T\_Setting(key integer , value char[1024](1024.md));

  * CREATE TABLE T\_log(id integer PRIMARY KEY AUTOINCREMENT, modal char[128](128.md),time\_log integer, type integer, value1 integer, value2 integer, message char[1024](1024.md));

  * CREATE TABLE T\_task(id integer PRIMARY KEY AUTOINCREMENT, type integer, time char[32](32.md), last\_run\_time char[32](32.md), task\_create\_time char[32](32.md), tip char[2048](2048.md) );

  * CREATE TABLE T\_todo2(id integer PRIMARY KEY AUTOINCREMENT, title char[128](128.md), priority integer, state integer, time\_create integer, time\_start integer, time\_finish integer, remark char[64\*1024]);

这四张表是互相完全独立的，没有互相引用关系，可以放心修改一张表内的数据而不会引起其它表数据的损坏。

关于索引：日志、提醒、任务表都有一个id字段，是表的主键，按SQLite3的特性，是内部自动建立有索引的；另外为了提高效率，手工创建了三个索引：
  * CREATE INDEX idx\_id on T\_Setting(key);
  * CREATE INDEX idx\_time on T\_log(time\_log);
  * CREATE INDEX idx\_todo2\_time\_create on T\_todo2(time\_create);

# T\_Setting #
保存设置数据。

相当于一个注册表，保存用户设置的参数，如窗口大小等等。

注意这里的key只是一个无意义的整数。key所代表的意义是在程序中另行定义的。这样，可以添加任意的配置值，而不必再修改数据库的结构，也意味着可以设计出一个通用的“设置保存器”模块。

# T\_log #
保存日志数据。

没什么好说的，日志表，几个字段分别为id、模块名、时间、日志类型、数值1、数值2、详细信息。
数值1、数值2的含义同样是由应用程序自行确定（是否有点过度设计了-_-!）。_

# T\_task #
保存定时提醒数据。

注意，由于历史原因，task在绿色时间管理中的含义是“定时提醒”。（计划任务的英文用todo表示）。

T\_task各个字段分别是：id、提醒类型、提醒时间、最后运行时间、提醒创建时间、提醒语句。

提醒类型有有一次提醒、每日提醒、每周提醒、每月提醒等等，在应用程序中定义。

这里要注意的是，对于每一类提醒，对“提醒时间”字段的解释都有些不同（为什么？当作思考题，自己想想吧）。具体定义看代码的实现。

# T\_todo2 #
保存计划任务数据。

T\_todo2这个名字后面为什么有个“2”？没什么特别的，历史原因。程序发布之后，数据库结构是不能随意修改的，所以保留至今。

字段说明：id，任务标题，重要度，状态，创建时间，任务开始时间，任务完成时间，详细信息（即内容）

这里要注意的是，任务状态里有一个状态是“删除”。用户在任务列表界面点击“删除任务”的时候，其实任务并没有真正删除掉，只是将它的状态改为“删除”而已。

用户在历史任务窗口里点击“永久删除”的时候，任务才会真正地从数据库中移除。