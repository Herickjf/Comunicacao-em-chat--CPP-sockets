CXX = g++
CXXFLAGS = -std=c++11 -pthread -fsanitize=address -g
INCLUDES = -I./libs

SRCDIR = server
BINDIR = $(SRCDIR)
TARGET = $(BINDIR)/server

SRC = $(SRCDIR)/server.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
