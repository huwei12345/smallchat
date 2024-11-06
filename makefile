TARGET = server
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
$(TARGET):$(OBJS) Protocol.h
	g++ -o server -g $(OBJS) ./cache/friendCache.cpp -lmysqlcppconn

%.o:%.cpp Protocol.h
	$(CXX) -c -g $<  -I /usr/include/mysql-cppconn/ -lmysqlcppconn

clean:
	rm -rf *.o $(TARGET)
