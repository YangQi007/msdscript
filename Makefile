INCS = cmdline.h

OBJS = main.o cmdline.o

msdscript: $(OBJS)
	c++ --std=c++14 -O2 -o msdscript $(OBJS)

main.o = main.cpp $(INCS)
	c++ --std=c++14 -O2 -c main.cpp

cmdline.o = cmdline.cpp $(INCS)
	c++ --std=c++14 -O2 -c cmdline.cpp
