//
// Created by geox on 13.08.15.
//

#ifndef UM7_LT_ASYNCHRONOUSPACKETREADER_H
#define UM7_LT_ASYNCHRONOUSPACKETREADER_H


#include "um7lt.h"



class packetSplitter_um7lt {
public:

    packetSplitter_um7lt() noexcept;

    uint32_t splitPackets(uint8_t const *inputBuffer, uint32_t inputLength, std::array<uint8_t,UM7LT_BUFFER_LENGTH> *outputPackets,uint32_t maxNumberOfPackets) noexcept;
    void resetSpliting() noexcept; // could be useful in timeouts

private:

    uint8_t packet_[254] = {};
    uint8_t *const packetBegin_;
    uint8_t *const packetEnd_;

    uint8_t *ptrPacketCur_;
    uint8_t *ptrPacketBegin_;
    uint8_t *ptrPacketEnd_;
};


#endif //UM7_LT_ASYNCHRONOUSPACKETREADER_H
