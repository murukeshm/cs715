
CXX = g++-4.7.2
CC = /opt/cs715/gcc-4.7.2/gcc

GCCPLUGINS_DIR:=$(shell $(CXX) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include -I$(CC)
LFCPA=/home/bro3886/devel/cs715/project/lipta-gcc-4.7-V1.1-multiple/lib/lfcpa.so

FLAGS= -fPIC -O2
CFLAGS = -fdump-ipa-all -fdump-ipa-pta -fipa-pta -O1 
# -fdump-tree-all
all: test

plugin.so: plugin.cpp
	$(CXX) $(INCLUDE) $(FLAGS) -fplugin=lfcpa -shared $^ -o $@

.PHONY: test 4.test 6.test
test:  test4.c
# Specifiy plugin for each file if output for each file is needed.
#	$(CXX) -fplugin=./plugin.so -fplugin=$(LFCPA) -flto -flto-partition=none $(INCLUDE) -c test6.c -o test $(CFLAGS)
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c test4.c -o test $(CFLAGS)
#	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none $(INCLUDE) -c test4.c -o test $(CFLAGS)
#	$(CXX) -fplugin=./plugin.so -flto -c test1.c $(CFLAGS)
# 	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -c test2.c  $(CFLAGS)
#	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -o test test1.o  test2.o $(CFLAGS)
# Partition = none explicitly loads all bodies, so they appear part of the same
# translation unit. Partition = 1to1 explicitly retains the original distinction,
# and the default (partition = balanced), does its own thing. Look for ltrans files
# if partition != none.
# 	$(CXX) -fplugin=./plugin.so -flto -flto-partition=1to1 -o test test1.o test2.o $(CFLAGS)
# 	$(CXX) -fplugin=./plugin.so -flto -o test test1.o test2.o $(CFLAGS)

test4: test4.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test6: test6.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test7: test7.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test8: test8.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test9: test9.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test10: test10.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

test11: test11.c
	$(CXX) -fplugin=lfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o lfcpa/$@ $(CFLAGS)
	$(CXX) -fplugin=mlfcpa -flto -flto-partition=none $(INCLUDE) -c $^ -o mlfcpa/$@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f plugin.so test *.*.* *~ *.o a.out lfcpa/* mlfcpa/*
