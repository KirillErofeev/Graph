//
// Created by love on 14.05.16.
//
#include <cstddef>
#include <ctime>
static unsigned long x=std::time(0)%1000000000, y=362436069, z=521288629;

size_t fastRand(const unsigned long mask=0xFFFFFFFFFFFFFFFFul){

    //period 2^96-1
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
        return z&mask;
}