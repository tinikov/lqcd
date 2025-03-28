.SUFFIXES: .cpp .h .o

# 编译器和链接器选项
CXX = mpic++  # 使用 MPI 的 C++ 编译器
LD = mpic++   # 使用 MPI 的 C++ 链接器
CXXFLAGS += -Wall -std=c++17 # 添加头文件搜索路径和编译选项
LDLIBS  += -lmpi

# 源文件和头文件
headers = tiniqcd.hpp
sources = test.cpp tiniqcd.cpp

# 目标文件
objects = $(sources:%.cpp=%.o)

# 可执行文件名
program = test.out

# 默认目标
all: $(program)

# 链接目标文件生成可执行文件
$(program): $(objects)
	$(LD) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

# 编译源文件生成目标文件
%.o: %.cpp $(headers)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理目标文件和可执行文件
clean:
	rm -f $(program) $(objects)

# 运行程序 (示例)
run: $(program)
	mpirun -np 4 ./$(program) # 使用 4 个进程运行程序

