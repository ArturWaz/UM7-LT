
#include "um7lt/um7lt.h"
#include "um7lt/um7lt_packetSplitter.h"


#include <cstdint>
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <array>
#include <vector>


using namespace std;

void show(std::uint8_t l) { cout << hex << "0x" << int(l) << " " << dec << endl; }


template <class T>
void printLine(T const *b, std::uint32_t length) {
    cout << "Line:   [ ";
    for (uint32_t i = 0; i < length; ++i) {
        cout << b[i];
    }
    cout << " ].\n";
}


void printLine(std::uint8_t const *b, std::uint32_t length) {
    cout << "Line:   [ " << hex;
    for (uint32_t i = 0; i < length; ++i) {
        cout << "0x" << int(b[i]) << " ";
    }
    cout << dec << "].\n";
}


void printPacket(std::uint8_t const *b) {
    cout << "Line:   [ " << hex;
    for (uint32_t i = 0; i < b[3]+uint32_t(4); ++i) {
        cout << "0x" << int(b[i]) << " ";
    }
    cout << dec << "].\n";
}



template<class T, size_t N>
size_t size(T (&)[N]) { return N; }




int main() {

    array<uint8_t,20> lol;
    lol[5] = 34;

    vector<int> lol1;


    uint8_t a[7];
    std::cout << "Length of array = " << (sizeof(a)/sizeof(*a)) << std::endl;

    //std::cout << "sdfsdF :  " << um7lt::calculateChecksum(a) << std::endl;


    int *p = new int[7];
    std::cout << "Length of array = " << (sizeof(p)/sizeof(*p)) << std::endl;


    int *c;
    std::cout << "Length of array = " << (sizeof(c)/sizeof(*c)) << std::endl;

    return 0;
}