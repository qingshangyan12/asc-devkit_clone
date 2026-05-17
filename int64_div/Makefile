CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET   := test_int64_div
SRCS     := int64_div.cpp test_int64_div.cpp

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRCS) int64_div.hpp Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
