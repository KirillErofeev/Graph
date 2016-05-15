#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <ostream>

template<class T>
void printArray(std::ostream &os, T* out, size_t size){
    for(int i = 0; i<size; ++i)
        os << *(out + i) << " ";
    os<<std::endl;
}

template<typename T>
std::pair<size_t, T> getMax(T* in, size_t size){
    T max = *in;
    size_t index;
    for (size_t i = 1; i < size; ++i) {
        if(in[i] > max) {
            max = in[i];
            index = i;
        }
    }
    return {index, max};
}
#endif