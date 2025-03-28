CXX = mpic++
CXXFLAGS = -Wall -std=c++17
TARGET = hello_world.out

all: $(TARGET)

$(TARGET): hello_world.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) hello_world.cpp $(LDLIBS)

clean:
	rm -f $(TARGET)

run:
	mpirun -np 4 ./$(TARGET)