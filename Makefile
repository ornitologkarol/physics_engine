CXX = g++
CXXFLAGS = 
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp basic_math.cpp entities.cpp collision.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(SFMLFLAGS)

clean:
	rm -f $(TARGET)
