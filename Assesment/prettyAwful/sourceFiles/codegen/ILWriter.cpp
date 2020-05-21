//
//  ILWriter.cpp
//  orbitasm
//
//  Created by Cesar Parent on 08/02/2017.
//  Copyright © 2017 cesarparent. All rights reserved.
//
//no changes have been made to these files as they contain orbatism (a concept I am unfamiliar with)

#include "ILWriter.hpp"



#define IEEE754_BITS 64
#define IEEE754_EXPBITS 11
#define IEEE754_SIGNIFICANDBITS (IEEE754_BITS - IEEE754_EXPBITS - 1)

// Barely adapted form `Beej's guide to network programming`
// http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#serialization
static uint64_t pack754(long double f)
{
    long double fnorm;
    int32_t shift;
    int64_t sign, exp, significand;
    
    if (f == 0.0) return 0; // get this special case out of the way
    
    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }
    
    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;
    
    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<IEEE754_SIGNIFICANDBITS) + 0.5f);
    
    // get the biased exponent
    exp = shift + ((1<<(IEEE754_EXPBITS-1)) - 1); // shift + bias
    
    // return the final answer
    return (sign<<(IEEE754_BITS-1)) | (exp<<(IEEE754_BITS-IEEE754_EXPBITS-1)) | significand;
}

void ILWriter::write(ILWriter::OMFTag data) {
    out_.put(uint8_t(data));
}

void ILWriter::write(uint8_t data) {
    out_.put(data);
}

void ILWriter::write(uint16_t data) {
    out_.put((data >> 8) & 0x00ff);
    out_.put(data & 0x0ff);
}

void ILWriter::write(uint32_t data) {
    out_.put((data >> 24) & 0xff);
    out_.put((data >> 16) & 0xff);
    out_.put((data >> 8) & 0xff);
    out_.put(data & 0x0ff);
}

void ILWriter::write(uint64_t data) {
    write(uint32_t((data >> 32) & 0x00000000ffffffff));
    write(uint32_t(data & 0x00000000ffffffff));
}


void ILWriter::write(double data) {
    write(OMFTag::NUM);
    write(pack754(data));
}

void ILWriter::write(const std::string& data) {
    auto size = data.length();
    if(size > UINT16_MAX) size = UINT16_MAX;
    
    write(OMFTag::STRING);
    write(uint16_t(size));
    out_.write(data.data(), size);
}

ILWriter::ILWriter(std::ostream& out) : out_(out) {
    static const char signature[] = "OMFF";
    out.write(signature, 4);
    out.put(0x00);
    out.put(0x01);
}
