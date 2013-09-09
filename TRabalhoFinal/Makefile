.c.o:   $*.h
	$(CC) $(CFLAGS) -c $*.c

.cpp.o:	$*.h
	$(CC) $(CFLAGS) -c $*.cpp

CC = g++
CFLAGS = -I/usr/local/include/opencv2 -g
LDFLAGS = 	-L/usr/local/lib -lopencv_core \
			-lopencv_highgui -lopencv_imgproc \
			-lglut -lGLU -lGL -lGLEW -g

all: eggBox
	
eggBox:  eggBox.o shader.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm *.o *.c~*.*~ sinteticTexture simpleTexture 
