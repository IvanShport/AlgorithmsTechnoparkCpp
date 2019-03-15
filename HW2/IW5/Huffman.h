//
// Created by ivanshport on 30.11.18.
//

#ifndef IW5_HUFFMAN_H
#define IW5_HUFFMAN_H

typedef unsigned char byte;

struct IInputStream {
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};


void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

#endif //IW5_HUFFMAN_H
