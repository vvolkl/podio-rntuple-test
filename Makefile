all:
	g++ test.cpp `root-config --cflags --libs` -lROOTNTuple -ledm4hep -ledm4hepDict
	./a.out
