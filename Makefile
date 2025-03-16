# Makefile cho dự án SDL2 sử dụng C++23, bật cảnh báo và tối ưu hóa -O2

# Trình biên dịch và các cờ
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++23 -I./include -ID:\sdl2\SDL2-2.28.5\x86_64-w64-mingw32\include
LDFLAGS = -LD:\sdl2\SDL2-2.28.5\x86_64-w64-mingw32\lib -lmingw32  -lSDL2main -lSDL2 -SDL2_image 

# Thư mục chứa mã nguồn và thư mục chứa file đối tượng
SRCDIR = src
OBJDIR = obj

# Tìm tất cả các file .cpp trong thư mục SRCDIR
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
# Chuyển đổi tên file .cpp thành file .o tương ứng nằm trong thư mục OBJDIR
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Tên file thực thi
TARGET = main.exe

# Mục tiêu mặc định: build project
all: $(TARGET)

# Liên kết các file đối tượng để tạo file thực thi
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Biên dịch từng file .cpp thành file .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Xoá file thực thi và file đối tượng
clean:
	del /Q $(OBJDIR)\*.o
	del /Q $(TARGET)

.PHONY: all clean
