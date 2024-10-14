# 记录
## 表情

> #a0xb31 编码
在TextEdit里，如何将插入的图片解析出来。
1. 使用Document，需要进一步学习
2.继承TextEdit，重写textChang函数，维护一个QString作为当前文字的编码替换。
3.使用Html语言，这样也方便使用文件、图片、视频等。

抖动窗口也是根据以上不同进行设计 #a0x301 抖动

## 文件和图片
Ftp传输先行，ftp传输完成后，再添加到聊天窗口。大的可以先传预览图。

文件也是：Ftp传输先行，传输过程可用进度条表示，传输完成，可打开文件或者目录。

### 显示最好是用QItemList这种，每个消息进行分割

文图片表情是在一块的。文图文将被拆分。视频文件都需要拖拽或者对话框选取。

## User表加一项，将好友分组管理。
又是一个数据库表，也可不做，微信就没有

## 线程池
目前线程池基本是等于线程Manager，因为没有Task，每个线程都是Epoll_wait。


## 通知机制
nofify : to person
            to all
            to group
            
非数据notify:
系统notify和用户级notify


## 分组、分包


## 消息确认机制

## 文件存储（亮点）
维护每个用户一个文件树，文件在服务器平铺存储。

数据库维护结构

需要一个版本管理机制。

但是与单独存储又有矛盾，所以某些文件类型为Version型，或者解析Git也可以。

主要的宗旨是方便办公，与平时使用需求结合。

## Email


## T
![](vx_images/335715846454578.png)


## TodoList

1. //TODO:ServerSocket设置成EPOLLET需要一些适配
2. //TODO：每个EventLoop分别管理连接和session还是总体一起管理？
3. //TODO:一次性全部写出，不太好，有缓冲区后修改
4. //检查磁盘用量和用户限制磁盘使用量 //TODO:checkDisk
5. //TODO:发出去的消息，相应需要带ID吗？
6. //TODO: 联合查询，用户的名字、email等返回



客户端
1. //TODO:是否需要切换目录，还是直接带目录发送就可以
2. //TODO:使用id串联整个文件传输流程
3. //TODO:在deserial里CRC校验
4. //TODO: 可能存在写不完全的情况？
5. //TODO: 交给业务线程，或者直接处理
6. //TODO:最好是在FileSuccess的响应返回后再进行业务，因为可能需要MD5校验和数据库操作等
7. //TODO://FtpSender::GetInstance()->removeFile(info);
8. //TODO:右下角图标添加几个提示
9. //TODO:闪烁几个窗口，右下角图标添加几个提示
10. //TODO:要写一些把Json全部读出来，把表情放Tableview里，点击后，把图片放入输入框里，//在输入框点发送后，将textEdit里读出来，然后编码为QString，或者Qt有提供其他机制。//发送，然后在把内容拷贝到textEdit2里，注意解码为表情，进行渲染。发送到服务器后就是QString,//到客户端就要进行解码渲染。
11. //TODO: 这里plain是没法解出的，需要再去考虑
12. //TODO:验证文件类型、大小限制以及文件路径的安全性
13. //TODO：closeConnection后续删除是在此处删除，还是在移除从Epoll中移除时再删除？
14. //TODO:这里时间应该重新设置
15. //TODO:是用friendConn还是用conn?应该是friendConn
16. //TODO:发出去的消息，相应需要带ID吗？
17. //TODO:相当于同意替换默认头像，在SQL中直接修改users表头像路径
18. //TODO究竟是由于服务器业务上不需要发送文件，还是服务器出现问题需要进一步通信信息说明
19. //TODO:申请态
20. //TODO: 联合查询，用户的名字、email等返回
21. //TODO: 联合查询，用户的名字、email等返回
22. //TODO:发出去的消息，相应需要带ID吗？