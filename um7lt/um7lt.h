//
// Created by geox on 13.08.15.
//

#ifndef __BASE_UM7LT_H
#define __BASE_UM7LT_H


#include <stdint.h>
#include <array>


#define UM7LT_BUFFER_LENGTH 80 // cant be smaller than 74


namespace um7lt {

    enum class DataType : uint8_t {
        HEALTH = 0x55,

        GYRO_RAW = 0X56,
        ACCEL_RAW = 0x59,
        MAG_RAW = 0x5C,

        TEMPERATURE = 0x5F,

        GYRO_PROC = 0x61,
        ACCEL_PROC = 0x65,
        MAG_PROC = 0x69,

        QUAT = 0x6D,
        EULER = 0x70
    };


    inline DataType getDataType(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet) noexcept {
        return DataType(*(packet.data()+4));
    }



    bool getHealth(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, uint32_t &health) noexcept;

    // below functions return time in [s]
    float getGyroRaw(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept;
    float getAccelRaw(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept;
    float getMagRaw(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, uint16_t &x, uint16_t &y, uint16_t &z) noexcept;

    float getTemperature(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, float &temperature) noexcept;

    float getGyroProc(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept;
    float getAccelProc(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept;
    float getMagProc(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, float &x, float &y, float &z) noexcept;

    float getQuat(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, int16_t &a, int16_t &b, int16_t &c, int16_t &d) noexcept;
    inline double quatToSI(int16_t value) noexcept { return double(value)/29789.09091; }
    float getEuler(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, int16_t &roll, int16_t &pitch, int16_t &yaw) noexcept;
    inline double eulerToSI(int16_t value) noexcept { return double(value)/91.02222; }
    float getEulerDot(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet, int16_t &rollRate, int16_t &pitchRate, int16_t &yawRate) noexcept;
    inline double eulerDotToSI(int16_t value) noexcept { return double(value)/16.; }


    void setZeroGyros(std::array<uint8_t,UM7LT_BUFFER_LENGTH> &packet) noexcept;



    uint16_t calculateChecksum(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet) noexcept;

    bool validateChecksum(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet) noexcept;

    bool validatePacket(std::array<uint8_t,UM7LT_BUFFER_LENGTH> const &packet) noexcept;

}


#endif //DYNAMIXEL__BASE_UM7LT_H
