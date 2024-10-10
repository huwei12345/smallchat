# smallchat
## 项目简介
  这是一款即时通讯软件，C/S模式，采用C++ + Linux服务器 + Mysql数据库 + Qt客户端的技术栈，目前处于基础功能开阶段与项目架构搭建截断，基础功能可用50%，正在开发，欢迎pull request，也欢迎加好友一起交流学习🤝，Q:531931740。

## 使用说明
### 环境
> `ubuntu22.04`  `Qt5.14.2` `MySQL8.0`  
> [Qt5.9 ~ 5.14.2]  
> (MYSQL安装在ubuntu)  
> `sudo apt install mysql`  
> `sudo apt-get install unixodbc unixodbc-dev`  

[**Server**]

`cd smallchat`  
`make`  
`./server`

[**Client**]  
`直接在Qt Creator里选择Qt5.14套件编译`  
`修改目录里的config.ini，修改默认服务器目录和FTP服务器目录为ubuntu的IP`  

[**FTP**]  
`ubuntu下sudo apt install vsftpd`  
将项目中的vsftpd.conf替换到/etc/vsftpd.conf，如果之前已有vsftp，请**谨慎替换**  
配置客户端配置文件FTP相关项，调试连通FTP。

[**MYSQL初步配置**]  
`正常配置mysql, root 密码: 123456`  
`编译sql/目录下的main.cpp，可以直接初始化项目数据库。`  
`调试接通数据库`  

[**MYSQL相关C++驱动**]  
```shell
sudo apt-get install unixodbc unixodbc-dev
sudo apt-get install odbcinst
https://blog.csdn.net/fusugongzi/article/details/134289583
https://dev.mysql.com/downloads/connector/cpp/
https://blog.csdn.net/w17396244624/article/details/136225841
isql -v myodbc root 123456
/etc/odbc.ini中的Driver可以写路径也可以写odbcinst.ini里的驱动名。
```
`安装sql目录下的压缩包里的所有dpkg包`  

---

### 运行效果
***登录界面***  
![登录](https://github.com/user-attachments/assets/af4859ee-7f9b-4e8d-aceb-3f15019d0cfd)  
  
***好友界面***  
![好友界面](https://github.com/user-attachments/assets/23273a09-0aaa-4ca2-9bd1-75924a3d34e6)  

---

## 项目架构与原理  
...
## 开发记录 
...
## 待改进与开发 
...
## 一些想法 
...