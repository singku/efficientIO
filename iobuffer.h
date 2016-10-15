#ifndef __IOBUFFER_H__
#define __IOBUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <utility>

using namespace std;
#define IOBufferSize    (10*1024*1024)

class IOBuffer {
public:
    FILE *fp;
    char *buf;
    size_t offset;  //valid data pos
    size_t bufLen;  //total length of buf. fixed.
    size_t readPos; //when using as read buffer, record readPos

    IOBuffer(const IOBuffer &iob) {
        this->fp = 0;
        offset = 0;
        buf = (char*)malloc(IOBufferSize);
        bufLen = IOBufferSize;
        readPos = 0;
    }
    IOBuffer(FILE *fp) {
        this->fp = fp;
        offset = 0;
        buf = (char*)malloc(IOBufferSize);
        bufLen = IOBufferSize;
        readPos = 0;
    }
    ~IOBuffer() {
        free(buf);
        buf = 0;
    }
    void rewind() {
        offset = 0;
        readPos = 0;
    }
    void setfp(FILE *fp) {
        this->fp = fp;
    }
    bool isfeof() {
        return feof(this->fp);
    }
    void write(const void *src, size_t srcLen, bool rightaway = false);
    void writeRemain();
    bool read(); //read from file
    inline size_t dataLen() {
        return offset - readPos;
    }
    char* fetch(size_t len);
    uint8_t fetch8() {
        char *v = fetch(1);
        if (!v) return 0;
        return *((uint8_t*)v);
    }
    uint16_t fetch16() {
        char *v = fetch(2);
        if (!v) return 0;
        return *((uint16_t*)v);
    }
    uint32_t fetch32() {
        char *v = fetch(4);
        if (!v) return 0;
        return *((uint32_t*)v);
    }
    uint64_t fetch64() {
        char *v = fetch(8);
        if (!v) return 0;
        return *((uint64_t*)v);
    }

};



#endif
