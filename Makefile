ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y



all: 

	$(CC) $(CFLAGS) -I${ROOTSYS}/include -c  *.cc $(ROOTLIBS)
	$(CC) *.o CORE/*.o -o babymaker


clean:
	rm -rf *.o
	rm -rf babymaker
