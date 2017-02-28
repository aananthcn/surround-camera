.DEFAULT_GOAL := all

# constants

MKDIR = mkdir -p
COPY = cp -r

IFLAGS = -I. -I./src -I./src/esUtil
CFLAGS = ${IFLAGS} 
#LFLAGS = -lpng -lm -lGLESv2 -lEGL -lglut -lGLU -lGL -lX11
LFLAGS = -lpng -lm -lGLESv2 -lEGL -lGLU -lGL -lX11


TARGET = surround-camera

# objects
objs = src/main.o \
       src/init.o \
       src/render.o \
       src/helper.o \
       src/esUtil/esUtil.o \
       src/esUtil/esShapes.o \
       src/esUtil/esShader.o \
       src/esUtil/esTransform.o \
       src/esUtil/esUtil_TGA.o \
       src/esUtil/esUtil_x11_linux.o

#       src/esUtil/esUtil_fblinux.o
#       src/esUtil/esUtil_null_linux.o

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@



main_object: ${objs}
	$(CC) -o ${TARGET} $^ $(LFLAGS)

.PHONY: install clean


clean:
	$(RM) ${objs} 
	$(RM) ${TARGET}


debug: CFLAGS += -g -DDEBUG
debug: LFLAGS += -g -DDEBUG
debug: main_object 


all: main_object

