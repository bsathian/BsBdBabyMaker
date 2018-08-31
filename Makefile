ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y



all: 

	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o babymaker *.cc $(ROOTLIBS)


clean:
	rm -rf *.o
	rm -rf babymaker
