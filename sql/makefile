ALL:main.cpp
	g++ -g -o main main.cpp -I /usr/include/mysql-cppconn/ -lmysqlcppconn
main2:main2.cpp
	g++ -g -o main2 main2.cpp -lodbc

clean: 
	rm main main2 *.o -rf
