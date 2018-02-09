# CEF3
MFC下使用CEF3

## demo说明

1.    使用的cef版本为3.2623.1395.g3034273，cef_binary的源码未放到github，如果需要联系我839505138
2.    此demo编译，VS2013 Update5
3.    实现了C++和js交互
4.    使用CEF时，加载的网页，如果里面有链接，指定target=”_blank”，则会弹出一个新的浏览窗口。如果想禁掉，让新页面在当前浏览窗口中显示，可以在browser进程的handler里重写CefLifeSpanHandler的OnBeforePopup方法，用browser的mainFrame加载popup请求的url
5.    使用flash的方法在我CSDN博客中，把代码加入进来就可以了。
6.    browser和render进程分别继承CefAPP
7.    test.html是网页测试用例 
8.    解决flash有黑框 : http://blog.csdn.net/zhuhongshu/article/details/77482985 
9.    实现下载功能（是网页上触发下载，并不是MFC触发下载）
10.   GO按钮增加了快捷键回车
11.   增加F5刷新
12.   增加前进和后退按钮
13.   增加开发者选项调试网页，但是未正确关闭这个Devtools窗口，存在一些问题

## 有什么意见请及时通知我，谢谢