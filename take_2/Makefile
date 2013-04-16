CC = g++
OBJ = SurveillanceSystem.o BgdCapturerSingle.o  BgdCapturerAverage.o FrameProcessor.o MotionProbYDiff.o MotionLocatorGrid.o
CFLAGS = -I/opt/local/include/ -Wall -c
LFLAGS = -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

all: $(OBJ)
	$(CC) $(LFLAGS) -o main $(OBJ)

BgdCapturerAverage.o: BgdCapturerAverage.cpp BgdCapturerAverage.h video_frame.h FrameProcessor.h FrameProcessor.cpp
	$(CC) $(CFLAGS) -o $@ $<

BgdCapturerSingle.o: BgdCapturerSingle.cpp BgdCapturerSingle.h video_frame.h FrameProcessor.h FrameProcessor.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) main 

FrameProcessor.o: FrameProcessor.cpp FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

MotionLocatorGrid.o: MotionLocatorGrid.cpp MotionLocatorGrid.h MotionProbYDiff.cpp MotionProbYDiff.h MotionProb.h FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

MotionProbYDiff.o: MotionProbYDiff.cpp MotionProbYDiff.h MotionProb.h FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

SurveillanceSystem.o: SurveillanceSystem.cpp video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

