CC = g++
OBJ = SurveillanceSystem.o BgdCapturerSingle.o  BgdCapturerAverage.o FrameProcessor.o IPCamProcessor.o MotionProbYDiff.o MotionLocBlobThresh.o
CFLAGS = -I/opt/local/include/ -Wall -c -O0 -ggdb
LFLAGS = -L/opt/local/lib -lcvblob -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lpthread -lcurlpp -lstdc++ -lcurl `pkg-config opencv --libs`

all: $(OBJ)
	$(CC) -o main $(OBJ) $(LFLAGS)

BgdCapturerAverage.o: BgdCapturerAverage.cpp BgdCapturerAverage.h video_frame.h FrameProcessor.h FrameProcessor.cpp
	$(CC) $(CFLAGS) -o $@ $<

BgdCapturerSingle.o: BgdCapturerSingle.cpp BgdCapturerSingle.h video_frame.h FrameProcessor.h FrameProcessor.cpp
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -rf $(OBJ) main 

FrameProcessor.o: FrameProcessor.cpp FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

IPCamProcessor.o: IPCamProcessor.cpp IPCamProcessor.h
	$(CC) $(CFLAGS) -o $@ $<

MotionLocBlobThresh.o: MotionLocBlobThresh.cpp MotionLocBlobThresh.h MotionProbYDiff.cpp MotionProbYDiff.h MotionProb.h FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

MotionProbYDiff.o: MotionProbYDiff.cpp MotionProbYDiff.h MotionProb.h FrameProcessor.h video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

SurveillanceSystem.o: SurveillanceSystem.cpp video_frame.h
	$(CC) $(CFLAGS) -o $@ $<

