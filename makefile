CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra -I.
BINDIR = bin

all: $(BINDIR) server client

$(BINDIR):
	mkdir -p $(BINDIR)

server: server.cpp libs/libtslog.hpp libs/chat_group.hpp libs/client_info.hpp libs/ticketLock.hpp libs/colors.hpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/server server.cpp

client: client.cpp libs/libtslog.hpp libs/colors.hpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/client client.cpp

clean:
	rm -rf $(BINDIR)

.PHONY: all clean