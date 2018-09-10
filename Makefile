ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y



all: 

	$(CC) $(CFLAGS) -I${ROOTSYS}/include -I./CORE -c *.cc 
	$(CC) *.o $(ROOTLIBS) CORE/CMS3_CORE.so -o babymaker


clean:
	rm -rf *.o
	rm -rf babymaker
