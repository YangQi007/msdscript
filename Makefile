INCS = expr.h catch.h parse.h cmdline.h val.h

OBJS = main.o expr.o parse.o cmdline.o val.o

msdscript: $(OBJS)
	c++ --std=c++14 -O2 -o msdscript $(OBJS)

main.o : main.cpp $(INCS)
	c++ --std=c++14 -O2 -c main.cpp

expr.o : expr.cpp $(INCS)
	c++ --std=c++14 -O2 -c expr.cpp

parse.o : parse.cpp $(INCS)
	c++ --std=c++14 -O2 -c parse.cpp

cmdline.o : cmdline.cpp $(INCS)
	c++ --std=c++14 -O2 -c cmdline.cpp

val.o : val.cpp $(INCS)
	c++ --std=c++14 -O2 -c val.cpp

clean :
	rm -f $(OBJS)

.PHONY: test
test: msdscript
		./msdscript --test