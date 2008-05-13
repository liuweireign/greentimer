1、如何使用GreenTimer？
答：
GreenTimer是绿色软件，无需安装。要使用它，只要下载GreenTimer（http://code.google.com/p/greentimer/downloads/list），然后双击运行其中的GreenTimer.exe文件即可。
为了能让它开机自动运行，建议将它拖放到启动菜单中（“开始”-->“程序”-->“启动”）。
GreenTimer没有主界面的，启动时仅会在托盘区显示一个图标。遇到整点的时候，它会自动在托盘区显示一个提醒气球。这个气球在十秒钟之后自动关闭。

2、如何编译GreenTimer（如果你不是程序开发人员，那么不必看这一节）
答：
GreenTimer使用VC++2005开发，所以你必须要安装有VC++2005。（vc其它版本也行，不过暂时没有提供工程文件，你可能需要hack一下）。
然后，你需要安装WTL。
   * WTL可以在开发包中的compile_resource目录下找到。下载后，双击自解压到任意一个目录。然后把到WTL下的include目录添加为VC++的include目录即可（工具->选项->项目->vc++目录->包含文件）。

配置好之后，直接按F5就可以编译运行了，祝你顺利！
