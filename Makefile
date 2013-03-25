CC = g++
OBJ = BackgroundCapturer.o
CFLAGS = -I/opt/local/include/ -Wall -c
LFLAGS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc

all: 
	$(CC) $(CFLAGS) BackgroundCapturer.cpp -o BackgroundCapturer.o
	$(CC) $(LFLAGS) -o All BackgroundCapturer.o 

clean:
	rm -rf $(OBJ) All 
