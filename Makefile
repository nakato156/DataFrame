ifeq ($(OS),Windows_NT)
    TARGET = dataframe.exe
    RM = del /Q
else
    TARGET = dataframe
    RM = rm -f
endif

CXX = g++
CXXFLAGS = -std=c++17

SRCS = DataFrame.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)
	$(RM) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) $(OBJS)
