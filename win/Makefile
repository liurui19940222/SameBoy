# 指定编译器
CC=clang

SRC = $(wildcard ./src/*.cpp) 
SRC += $(wildcard ../gb-core/src/*.cpp)
SRC += $(wildcard ../gb-core/json/*.cpp)

Defines = "_CRT_SECURE_NO_DEPRECATE"

all: app clean

app: ${SRC}
	${CC} -D${Defines} -obin/SameBoy.exe ${SRC}

clean:
	rm -f *.obj

