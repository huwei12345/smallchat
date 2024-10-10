TARGET = server
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
$(TARGET):$(OBJS) Protocol.h
	g++ -o server $(OBJS) ./cache/friendCache.cpp -lmysqlcppconn

%.o:%.cpp Protocol.h
	$(CXX) -c $<  -I /usr/include/mysql-cppconn/ -lmysqlcppconn

clean:
	rm -rf *.o $(TARGET)
