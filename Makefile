#Reserved names
.PHONY: clean, mrproper

#Variables
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++14 -O2
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system
EXEC=Cheap-8
SRCDIR=src/
SRC=$(wildcard $(SRCDIR)*.cpp)
OBJ=$(SRC:$(SRCDIR)%.cpp=$(SRCDIR)%.o)

#Main
all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CXX) $^ -o $(EXEC) $(CXXFLAGS) $(LDFLAGS)

#Object files
$(SRCDIR)%.o: $(SRCDIR)%.cpp
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

#Clean
clean:
	rm -rf $(SRCDIR)*.o $(SRCDIR)*~

#Mrproper
mrproper: clean
	rm -rf $(EXEC)