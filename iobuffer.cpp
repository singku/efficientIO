#include "iobuffer.h"
#include <string.h>

char* IOBuffer::fetch(size_t len) {
    if (len > bufLen) return 0; //ask for too much

    if (len > dataLen()) {
        memmove((void*)buf, (void*)(buf + readPos), dataLen());
        offset = dataLen();
        readPos = 0;
        read();
    }
    if (len > dataLen()) return 0; //still ask too much
    readPos += len;
    return (buf + readPos - len);
}

bool IOBuffer::read() {
    if (dataLen() == bufLen) return false;

    if (offset == readPos) rewind();

    if (feof(fp)) return false;

    size_t readCnt = fread(buf + offset, 1, bufLen - offset, fp);
    if (!readCnt) return false;
    offset += readCnt;
    return true;
}

void IOBuffer::writeRemain() {
    if (offset == 0) return;

    fwrite(buf, offset, 1, fp);
    offset = 0;
}

void IOBuffer::write(const void *src, size_t srcLen, bool rightaway) {

    size_t cpyLen = min(srcLen, bufLen - offset);
    memcpy(buf + offset, src, cpyLen);
    offset += cpyLen;
    if (offset == bufLen) {
        fwrite(buf, bufLen, 1, fp);
        offset = 0;;
    }

    if (srcLen > cpyLen) {
        memcpy(buf + offset, (const char*)src + cpyLen, srcLen - cpyLen);
        offset += srcLen - cpyLen;
    }

    if (rightaway) {
        fwrite(buf, offset, 1, fp);
        offset = 0;
    }
}
