CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I src

SRCS = src/LanguageModel.cpp src/TextGenerator.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = slm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe