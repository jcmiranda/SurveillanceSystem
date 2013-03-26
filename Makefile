CC = g++
OBJ = BackgroundCapturer.o BackgroundSingleCapturer.o SurveillanceSystem.o
CFLAGS = -I/opt/local/include/ -Wall -c
LFLAGS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

all:
	$(CC) $(CFLAGS) BackgroundSingleCapturer.cpp -o BackgroundSingleCapturer.o
	$(CC) $(CFLAGS) SurveillanceSystem.cpp -o SurveillanceSystem.o
	$(CC) $(LFLAGS) -o All SurveillanceSystem.o BackgroundSingleCapturer.o

clean:
	rm -rf $(OBJ) All 
