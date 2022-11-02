/**
 * @file EziServoPlusE.h
 * @author Indruino (indruino.com)
 * @brief
 * @version 0.1
 * @date 2022-04-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef EziServoPlusE_H_
#define EziServoPlusE_H_

#include "Arduino.h"
#include "CommStatusEziMotion.h"
#include "Motion_frame_define.h"
#include <string.h>

#define Max_buffer_size 256
#define Ethernet_5500
#define Debug_Serial Serial
#define DEBUG

#ifdef Ethernet_5100
#include <SPI.h> // needed for Arduino versions later than 0018
#include <Ethernet.h>
#endif

#ifdef Ethernet_5500
#include <SPI.h> // needed for Arduino versions later than 0018
#include <Ethernet2.h>
#endif

class EziServoPlusE
{
private:
    char header[1] = {(char)0xAA};
    uint8_t reserved = 0;
    uint8_t length = 3;
    IPAddress _ip1;
    IPAddress _ip2;
    unsigned int localPort = 2001;
    uint8_t syncNo = 0;

    bool encode(char *des, const char *source, const uint8_t length);                                                    // encode_buffer
    bool decode(char *des, const char *source, const uint8_t length);                                                    // decode_buffer
    bool processData(IPAddress ip, char *des, const char *source, const uint8_t len_request, const uint8_t len_respond); // send data and after that it wait the data respond

public:
    EziServoPlusE();
    ~EziServoPlusE();

    bool initServo(IPAddress ip1, IPAddress ip2);
    char ServoEnable(IPAddress ip, bool state, uint8_t *Com_status);
    char ServoAlarmReset(IPAddress ip, uint8_t *Com_status);
    char MoveStop(IPAddress ip, uint8_t *Com_status);
    char EmergencyStop(IPAddress ip, uint8_t *Com_status);
    char MoveOriginSingleAxis(IPAddress ip, uint8_t *Com_status);
    char MoveSingleAxisAbsPos(IPAddress ip, int32_t posVal, uint32_t speed_pps, uint8_t *Com_status);
    char MoveSingleAxisIncPos(IPAddress ip, int32_t posVal, uint32_t speed_pps, uint8_t *Com_status);
    char MoveVelocity(IPAddress ip, uint32_t speed_pps, bool jog_dir, uint8_t *Com_status);
    char GetCommandPos(IPAddress ip, int32_t *cmdPosValueRt, uint8_t *Com_status);
    char SetCommandPos(IPAddress ip, int32_t cmdPosValueRt, uint8_t *Com_status);
    char GetActualPos(IPAddress ip, int32_t *cmdPosValueRt, uint8_t *Com_status);
    char SetActualPos(IPAddress ip, int32_t cmdPosValueRt, uint8_t *Com_status);
    char ClearPosition(IPAddress ip);
    char SetParameter(IPAddress ip, uint8_t para_num, int32_t data_para, uint8_t *Com_status);
    char GetParameter(IPAddress ip, uint8_t para_num, int32_t *data_para, uint8_t *Com_status);

    char VelocityOverride(IPAddress ip, uint32_t new_speed_cmd, uint8_t *Com_status);
};

#endif