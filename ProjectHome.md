在IE工具栏上制作一个抓取当前页面内容为jpg图片文件的按钮,使用 ATL 和 MFC ，VC 编写、IDE使用VS2010 。

原理是就是获取页面句柄后，使用PrintWindow截图，并且配合使用HTML的scrollTop和scrollLeft卷动页面，完成全部页面的截图，使用BitBlt拼接到CImage中。

增加 切换代理的按钮，支持 windows7 ，直接使用 InternetQueryOption 和 InternetSetOptionW设置代理 ,不使用直接修改注册表的方式。

并且，自动检测一个名为 c:\proxy.exe 的程序是否存在，如果存在，切换为代理状态下，自动运行这个程序，关闭代理，会自动杀掉这个程序的进程。