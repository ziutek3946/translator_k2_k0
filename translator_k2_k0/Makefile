CXX = g++
CXXFLAGS = -fdiagnostics-color -Wall -Wextra -O2 -g -std=c++11
LDFLAGS = -fdiagnostics-color -lmosquittopp -lelcc
TARGET = konsola_k2_k0
SRCS = Cfg_reader.cpp Parser.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DBJS = $(SRCS:.cpp=.d)

.PHONY: all
all: $(TARGET)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< > $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

-include $(DBJS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) $(DBJS)
