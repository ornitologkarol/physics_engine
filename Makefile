CXX = g++
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/main.cpp src/basic_math.cpp src/entities.cpp src/collision.cpp src/game.cpp src/space.cpp src/clipping.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(SFMLFLAGS)

	

clean:
	rm -f $(TARGET)
