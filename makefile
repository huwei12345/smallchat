TARGET = server
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
CXXFLAGS = -std=c++11 -g -I include -I third_party/spdlog/include -I /usr/include/mysql-cppconn/

$(TARGET):$(OBJS) Protocol.h
	g++ -o server -g $(OBJS) ./cache/friendCache.cpp -lmysqlcppconn -lpthread

%.o:%.cpp Protocol.h
	$(CXX) -c $(CXXFLAGS) $<

clean:
	rm -rf *.o $(TARGET)
