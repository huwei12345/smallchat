TARGET = client
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
$(TARGET):$(OBJS)
	$(CXX) -g -o $@ $^ ../MyProtocolStream.cpp -lpthread

%.o:%.cpp
	$(CXX) -g -c $< 

clean:
	rm -rf *.o $(TARGET)
