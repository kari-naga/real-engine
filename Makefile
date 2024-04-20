EXECUTABLE	= game_engine_linux
CXX			= clang++
INCLUDE		= -I./include/ -I./lib/windows/include/ -I./lib/src/
LIB			= -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.4
CXXFLAGS	= -std=c++17 -O3 $(INCLUDE) $(LIB)
SOURCES		= $(wildcard src/*.cpp include/lua/*.c lib/src/collision/*.cpp lib/src/common/*.cpp lib/src/dynamics/*.cpp lib/src/rope/*.cpp)

main:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)
