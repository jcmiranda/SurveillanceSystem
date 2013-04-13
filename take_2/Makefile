CC = g++
OBJ = SurveillanceSystem.o 
CFLAGS = -I/opt/local/include/ -Wall -c
LFLAGS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

all: $(OBJ)
	$(CC) $(LFLAGS) -o main $(OBJ)

SurveillanceSystem.o: SurveillanceSystem.cpp video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) main 
