CC=g++
CFLAGS= -w -O3 -funroll-loops -fno-rtti -fomit-frame-pointer -ffast-math -fno-stack-protector -ffunction-sections
LFLAGS= -lX11
SRC=src
TEST=tests
INC=include

all:
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeException.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeHardware.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeMath.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/Geode3DMath.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeGraphicalContext.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeFrameBuffer.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeWindow.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodePoint.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodeVertex.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodePrimitive2D.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/GeodePrimitive3D.cpp
	$(CC) -c $(CFLAGS) -I$(INC) $(SRC)/Geode3DObject.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -I$(INC) $(TEST)/main.cpp *.o -o Geode3D
	
clean:
	rm *.o Geode3D
