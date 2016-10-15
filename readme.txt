This project mainly includes two parts: the indexer and the merger

Indexer: process separate data and index files and generate intermediate tmp files
        for every 100 data/index files (basically decided by the No. of tarball packages)
        we will generate three files: tmp_x_url tmp_x_lexicon tmp_x_ivIndex
        x equals to (no of files / 100). 
        This is done in memory and output to disk. The gz reader and words parser are
        adjusted for efficiency purpoes based on the source from the course project website.

        To get the gz data files, you should run setenv.sh to setup environment.
        Corresponding script is explaned here.
        ```
        #extract gz raw file from tarball packages
        find ./NZ/ -name "*.tar" -exec tar -xf {} \;
        #refine data
        mv ./data/4c/*/*/*/vol_*/* ./data/
        #remove garbege dirs
        rm -rf ./data/4c
        #create tmpdata dir
        mkdir -p tmpdata
        #compile and link to get indexer and merger
        make
        ```
notice that the last step of setenv.sh is to run make to get binary
the Makefile is described here
the only thing need to notice is adding -DDEBUG to CFLAGS
when you want to check the intermediate output you can add this Macro flag
the Macro in the code is DEBUG_LOG(x) and x is cout << msg << endl;
if -DDBUG is specified then DEBUG_LOG(x) is defined as nothing.
check the common.h header file

##################Makefile#####################
SRC_FILES=parser.cpp readgz.cpp indexer.cpp writer.cpp
MERGER_FILES=merger.cpp
OBJ_FILES=$(SRC_FILES:%.cpp=%.o)
MERGER_OBJ=merger.o
CFLAGS=-I./ -W -fPIC -ggdb -std=c++11 
#-DDEBUG
LFLAGS=-lz
TARGET=indexer
MERGER=merger

$(TARGET): $(OBJ_FILES) $(MERGER_OBJ)
	g++ -o $(TARGET) $(LFLAGS) $(OBJ_FILES)
	g++ -o $(MERGER) $(MERGER_OBJ)
	-rm $(OBJ_FILES) $(MERGER_OBJ)

$(OBJ_FILES) : %.o : %.cpp
	g++ -o $@ -c $< $(CFLAGS)

$(MERGER_OBJ) : %.o : %.cpp
	g++ -o $@ -c $< $(CFLAGS)

clean:
	-rm $(TARGET) $(MERGER)
###################################################

Merger:
    Merger is an simple multi-way priority queue merge sort robot
    you can specify how many files are there to be merged and how
    many ways you want to merge simultaneously. but those numbers
    should be reasonable. you can't merge 2 files in one way.

Here is the content of run.sh
# there will be 4180 data files in data dir 
# for every 100 files we write a tmp disk file
time ./indexer ./data 4180 100
# there will be 42 tmp files generated. with 42way merging
time ./merger 42 42

Basically it takes 10 minutes to preprocess all the files
and takes about 20s to merge with 42 ways.
Here is the testing result.

real    9m13.031s
user    7m12.000s
sys 0m20.912s
totalWords: 1232293

real    0m19.741s
user    0m1.068s
sys     0m0.784s

Note that we only treate alpha and digit as valid words based
ont the parser.cpp

After processing, we get pretty much about 2635893 urls,
1232293 words.
the urltable has 157MB, lexicon table has 24MB and the ivIndexList has 178MB
