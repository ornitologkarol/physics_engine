CXX = g++
CXXFLAGS =-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wold-style-cast -Wunused -Wconversion
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp basic_math.cpp entities.cpp collision.cpp game.cpp space.cpp clipping.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(SFMLFLAGS)

debug:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(SFMLFLAGS)

	

clean:
	rm -f $(TARGET)
