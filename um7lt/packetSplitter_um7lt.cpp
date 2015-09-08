//
// Created by geox on 13.08.15.
//


#include "packetSplitter_um7lt.h"
#include <stdexcept>
#include <cstring>
#include <iostream>




packetSplitter_um7lt::packetSplitter_um7lt() noexcept :
        packetBegin_(&(packet_[5])),
        packetEnd_(&(packet_[254])),
        ptrPacketCur_(packetBegin_),
        ptrPacketBegin_(nullptr),
        ptrPacketEnd_(nullptr) {}


uint32_t packetSplitter_um7lt::splitPackets(uint8_t const *inputBuffer, uint32_t inputLength,
                                            std::array<uint8_t, UM7LT_BUFFER_LENGTH> *outputPackets,
                                            uint32_t maxNumberOfPackets) noexcept {

    if (maxNumberOfPackets < 1) return 0;
    uint32_t numberOfPackets = 0;

    for (uint8_t const *ptrBuff = &(inputBuffer[0]); ptrBuff != &(inputBuffer[inputLength]); ++ptrBuff) {
        *ptrPacketCur_ = *ptrBuff;

        if (ptrPacketEnd_ == nullptr) {
            uint8_t *ptrTmp = ptrPacketCur_;
            if (*(--ptrTmp) == 'p') {
                if (*(--ptrTmp) == 'n') {
                    if (*(--ptrTmp) == 's') {
                        if (((*ptrPacketCur_)&0xC0) == 0xC0) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 7 + ((*ptrPacketCur_)&0x3C);
                        }
                        else if (((*ptrPacketCur_)&0xC0) == 0x80) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 11;
                        }
                        else if (((*ptrPacketCur_)&0xC0) == 0x00) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 7;
                        }
                    }
                }
            }
        }

        if (ptrPacketEnd_ == ptrPacketCur_) {
            uint16_t checksum = 0;
            for (uint8_t *ptr = ptrPacketBegin_; ptr != ptrPacketEnd_-1; ++ptr)
                checksum += *ptr;
            if (checksum == (((*ptrPacketEnd_-1)<<8) | (*ptrPacketEnd_))) {
                std::memcpy(outputPackets[numberOfPackets].data(),ptrPacketBegin_,sizeof(uint8_t)*(ptrPacketEnd_-ptrPacketBegin_+1));
                ++numberOfPackets;
                if (maxNumberOfPackets <= numberOfPackets) return numberOfPackets;
            }
            else std::cerr << "\tWARNING: [ packetSplitter_um7lt::splitPackets(...) ]: Different checksums.\n";
            ptrPacketEnd_ = nullptr;
            ptrPacketCur_ = packetBegin_;
        }

        ++ptrPacketCur_;
        if (ptrPacketCur_ == packetEnd_) {
            ptrPacketCur_ = packetBegin_;
            std::cerr << "\tWARNING: [ packetSplitter_um7lt::splitPackets(...) ]: Many outputPackets lost.\n";
        }
    }

    return numberOfPackets;
}




/*
uint32_t packetSplitter_um7lt::splitPackets(uint8_t const *inputBuffer, uint32_t inputLength, uint8_t **outputPackets, uint32_t maxNumberOfPackets, uint32_t maxNumberOfBytes) {
    if (maxNumberOfPackets < 1 || maxNumberOfBytes < 8) return 0;
    uint32_t numberOfPackets = 0;

    for (uint8_t const *ptrBuff = &(inputBuffer[0]); ptrBuff != &(inputBuffer[inputLength]); ++ptrBuff) {
        *ptrPacketCur_ = *ptrBuff;

        if (ptrPacketEnd_ == nullptr) {
            uint8_t *ptrTmp = ptrPacketCur_;
            if (*(--ptrTmp) == 'p') {
                if (*(--ptrTmp) == 'n') {
                    if (*(--ptrTmp) == 's') {
                        if (maxNumberOfBytes < (*ptrPacketCur_) + 7u)
                            throw std::range_error(" [ packetSplitter_um7lt::splitPackets(...) ]: Maximum number of bytes is too small.\n");
                        if (((*ptrPacketCur_)&0xC0) == 0xC0) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 7 + ((*ptrPacketCur_)&0x3C);
                        }
                        else if (((*ptrPacketCur_)&0xC0) == 0x80) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 11;
                        }
                        else if (((*ptrPacketCur_)&0xC0) == 0x00) {
                            ptrPacketBegin_ = ptrTmp;
                            ptrPacketEnd_ = ptrPacketBegin_ + 7;
                        }
                    }
                }
            }
        }

        if (ptrPacketEnd_ == ptrPacketCur_) {
            uint16_t checksum = 0;
            for (uint8_t *ptr = ptrPacketBegin_; ptr != ptrPacketEnd_-1; ++ptr)
                checksum += *ptr;
            if (checksum == (((*ptrPacketEnd_-1)<<8) | (*ptrPacketEnd_))) {
                std::memcpy(outputPackets[numberOfPackets],ptrPacketBegin_,sizeof(uint8_t)*(ptrPacketEnd_-ptrPacketBegin_+1));
                ++numberOfPackets;
                if (maxNumberOfPackets <= numberOfPackets) return numberOfPackets;
            }
            else std::cerr << "\tWARNING: [ packetSplitter_um7lt::splitPackets(...) ]: Different checksums.\n";
            ptrPacketEnd_ = nullptr;
            ptrPacketCur_ = packetBegin_;
        }

        ++ptrPacketCur_;
        if (ptrPacketCur_ == packetEnd_) {
            ptrPacketCur_ = packetBegin_;
            std::cerr << "\tWARNING: [ packetSplitter_um7lt::splitPackets(...) ]: Many packets lost.\n";
        }
    }

    return numberOfPackets;
}
*/



void packetSplitter_um7lt::resetSpliting() noexcept {
    ptrPacketEnd_ = nullptr;
    ptrPacketCur_ = packetBegin_;
}

