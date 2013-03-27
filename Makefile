CC = g++
OBJ = BackgroundSingleCapturer.o SurveillanceSystem.o BackgroundCapturer.o
CFLAGS = -I/opt/local/include/ -Wall -c
LFLAGS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

all: $(OBJ)
	$(CC) $(LFLAGS) -o main $(OBJ)

BackgroundCapturer.o: BackgroundCapturer.cpp
	$(CC) $(CFLAGS) -o $@ $<

BackgroundSingleCapturer.o: BackgroundSingleCapturer.cpp BackgroundSingleCapturer.h BackgroundCapturer.h BackgroundCapturer.cpp
	$(CC) $(CFLAGS) -o $@ $<

SurveillanceSystem.o: SurveillanceSystem.cpp 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) main 
