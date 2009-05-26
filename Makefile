CC = g++
SRCDIR = src/
INCDIR = include/
CFLAGS = -O3 -funroll-loops -fno-rtti -fomit-frame-pointer -ffast-math -fno-stack-protector -ffunction-sections -fexceptions -fno-operator-names -I$(INCDIR)
LFLAGS = -lpthread -lX11


all: 
	$(CC) -c $(CFLAGS) $(SRCDIR)G3DObject.cpp 
	$(CC) -c $(CFLAGS) $(SRCDIR)GFrameBuffer.cpp 
	$(CC) -c $(CFLAGS) $(SRCDIR)GWindow.cpp 
	$(CC) $(CFLAGS) $(SRCDIR)main.cpp $(LFLAGS) -o Geode *.o

clean:
	rm -f *.o Geode

