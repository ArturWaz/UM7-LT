//
// Created by geox on 13.08.15.
//


#include "um7lt.h"




union UIF { uint32_t in; float out; };
union UII { uint16_t in; int16_t out; };
UIF toFloat[5];
UII toInt[5];

using namespace um7lt;



uint16_t um7lt::calculateChecksum(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet) noexcept {
    uint16_t checksum = 0;
    for (uint8_t const *ptr = packet.data(); ptr != packet.data()+(*(packet.data()+4)&0x3C); ++ptr) checksum += *ptr;
    return checksum;
}

bool um7lt::validateChecksum(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet) noexcept {
    return um7lt::calculateChecksum(packet) == (((*(packet.end()-1))<<8) | (*packet.end()));
}

bool um7lt::validatePacket(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet) noexcept {
    if (*packet.data() != 's' || *(packet.data()+1) != 'n' || *(packet.data()+2) != 'p') return false;
    return um7lt::validateChecksum(packet);
}



bool um7lt::getHealth(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const& packet, uint32_t &health) noexcept {
    if (getDataType(packet) != DataType::HEALTH) return false;
    health = uint32_t(((*(packet.data()+5))<<24) | ((*(packet.data()+6))<<16) | ((*(packet.data()+7))<<8) | (*packet.data()+8));
    return true;
}



#define UM7LT_RAW_DATA_CONVERTER_GENERATOR() \
    x = ((*(packet.data()+5))<<8) | ((*(packet.data()+6))<<0); \
    y = ((*(packet.data()+7))<<8) | ((*(packet.data()+8))<<0); \
    z = ((*(packet.data()+9))<<8) | ((*(packet.data()+10))<<0); \
    toFloat[0].in = ((*(packet.data()+11))<<24) | ((*(packet.data()+12))<<16) | ((*(packet.data()+13))<<8) | ((*(packet.data()+14))<<0); \
    return toFloat[0].out;


float um7lt::getGyroRaw(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept {
    if (getDataType(packet) != DataType::GYRO_RAW) return -1.f;
    UM7LT_RAW_DATA_CONVERTER_GENERATOR()
}

float um7lt::getAccelRaw(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept {
    if (getDataType(packet) != DataType::ACCEL_RAW) return -1.f;
    UM7LT_RAW_DATA_CONVERTER_GENERATOR()
}

float um7lt::getMagRaw(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept {
    if (getDataType(packet) != DataType::MAG_RAW) return -1.f;
    UM7LT_RAW_DATA_CONVERTER_GENERATOR()
}




float um7lt::getTemperature(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, float &temperature) noexcept {
    if (getDataType(packet) != DataType::TEMPERATURE) return -1.f;
    toFloat[0].in = ((*(packet.data()+5))<<24) | ((*(packet.data()+6))<<16) | ((*(packet.data()+7))<<8) | (*(packet.data()+8));
    toFloat[1].in = ((*(packet.data()+9))<<24) | ((*(packet.data()+10))<<16) | ((*(packet.data()+11))<<8) | (*(packet.data()+12));
    temperature = toFloat[0].out;
    return toFloat[1].out;
}




#define UM7LT_PROC_DATA_CONVERTER_GENERATOR() \
    toFloat[0].in = ((*(packet.data()+5))<<24) | ((*(packet.data()+6))<<16) | ((*(packet.data()+7))<<8) | (*(packet.data()+8)); \
    toFloat[1].in = ((*(packet.data()+9))<<24) | ((*(packet.data()+10))<<16) | ((*(packet.data()+11))<<8) | (*(packet.data()+12)); \
    toFloat[2].in = ((*(packet.data()+13))<<24) | ((*(packet.data()+14))<<16) | ((*(packet.data()+15))<<8) | (*(packet.data()+16)); \
    toFloat[3].in = ((*(packet.data()+17))<<24) | ((*(packet.data()+18))<<16) | ((*(packet.data()+19))<<8) | (*(packet.data()+20)); \
    x = toFloat[0].out; \
    y = toFloat[1].out; \
    z = toFloat[2].out; \
    return toFloat[3].out;


float um7lt::getGyroProc(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept {
    if (getDataType(packet) != DataType::GYRO_PROC) return -1.f;
    UM7LT_PROC_DATA_CONVERTER_GENERATOR()
}

float ::um7lt::getAccelProc(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept {
    if (getDataType(packet) != DataType::ACCEL_PROC) return -1.f;
    UM7LT_PROC_DATA_CONVERTER_GENERATOR()
}

float um7lt::getMagProc(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept {
    if (getDataType(packet) != DataType::MAG_PROC) return -1.f;
    UM7LT_PROC_DATA_CONVERTER_GENERATOR()
}



float um7lt::getQuat(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, int16_t &a, int16_t &b, int16_t &c, int16_t &d) noexcept {
    if (getDataType(packet) != DataType::QUAT) return -1.f;
    toInt[0].in = ((*(packet.data()+5))<<8) | (*(packet.data()+6));
    toInt[1].in = ((*(packet.data()+7))<<8) | (*(packet.data()+8));
    toInt[2].in = ((*(packet.data()+9))<<8) | (*(packet.data()+10));
    toInt[3].in = ((*(packet.data()+11))<<8) | (*(packet.data()+12));
    toFloat[0].in = ((*(packet.data()+13))<<24) | ((*(packet.data()+14))<<16) | ((*(packet.data()+15))<<8) | (*(packet.data()+16));
    a = toInt[0].out;
    b = toInt[1].out;
    c = toInt[2].out;
    d = toInt[3].out;
    return toFloat[0].out;
}

float um7lt::getEuler(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, int16_t &roll, int16_t &pitch, int16_t &yaw) noexcept {
    if (getDataType(packet) != DataType::EULER) return -1.f;
    toInt[0].in = ((*(packet.data()+5))<<8) | (*(packet.data()+6));
    toInt[1].in = ((*(packet.data()+7))<<8) | (*(packet.data()+8));
    toInt[2].in = ((*(packet.data()+9))<<8) | (*(packet.data()+10));
    toFloat[0].in = ((*(packet.data()+21))<<24) | ((*(packet.data()+22))<<16) | ((*(packet.data()+23))<<8) | (*(packet.data()+24));
    roll = toInt[0].out;
    pitch = toInt[1].out;
    yaw = toInt[2].out;
    return toFloat[0].out;
}

float um7lt::getEulerDot(std::array<uint8_t, UM7LT_BUFFER_LENGTH> const &packet, int16_t &rollRate, int16_t &pitchRate, int16_t &yawRate) noexcept {
    if (getDataType(packet) != DataType::EULER) return -1.f;
    toInt[0].in = ((*(packet.data()+13))<<8) | (*(packet.data()+14));
    toInt[1].in = ((*(packet.data()+15))<<8) | (*(packet.data()+16));
    toInt[2].in = ((*(packet.data()+17))<<8) | (*(packet.data()+18));
    toFloat[0].in = ((*(packet.data()+21))<<24) | ((*(packet.data()+22))<<16) | ((*(packet.data()+23))<<8) | (*(packet.data()+24));
    rollRate = toInt[0].out;
    pitchRate = toInt[1].out;
    yawRate = toInt[2].out;
    return toFloat[0].out;
}

void um7lt::setZeroGyros(std::array<uint8_t, UM7LT_BUFFER_LENGTH> &packet) noexcept {
    packet[0] = 's';
    packet[1] = 'n';
    packet[2] = 'p';
    packet[3] = 0x00;
    packet[4] = 0xAD; // zero gyro command
    packet[5] = ('s'+'n'+'p'+0x00+0xAD)>>8; // checksum?
    packet[6] = uint8_t('s'+'n'+'p'+0x00+0xAD);
}

