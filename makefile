TARGET = server
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
$(TARGET):$(OBJS) Protocol.h
	g++ -g -o server server.cpp MyProtocolStream.cpp RequestProcessor.cpp MysqlPool.cpp ./main.cpp -lmysqlcppconn settime.cpp

%.o:%.cpp Protocol.h
	$(CXX) -c $<  -I /usr/include/mysql-cppconn/ -lmysqlcppconn

clean:
	rm -rf *.o $(TARGET)
