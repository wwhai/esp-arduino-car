// Copyright (C) 2024 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef CTRL_H
#define CTRL_H

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "crc16.h"

// 定义电机控制引脚
const int motorA1 = 2;
const int motorA2 = 3;
const int motorB1 = 4;
const int motorB2 = 5;
const int motorC1 = 6;
const int motorC2 = 7;
const int motorD1 = 8;
const int motorD2 = 9;

// 定义 nRF24L01 的 SPI 接口引脚
const int CE_PIN = 7;            // 片选使能引脚
const int CSN_PIN = 8;           // 片选引脚
const byte address[6] = "00001"; // 无线模块的通信地址

class Ctrl
{
public:
    Ctrl();
    void setup();
    void loop();

private:
    RF24 radio;
    void motorAForward();
    void motorAReverse();
    void motorAStop();
    void motorBForward();
    void motorBReverse();
    void motorBStop();
    void motorCForward();
    void motorCReverse();
    void motorCStop();
    void motorDForward();
    void motorDReverse();
    void motorDStop();
    void forward();
    void reverse();
    void turnLeft();
    void turnRight();
    void stop();
    uint16_t calculateCRC16(uint8_t *data, uint8_t len);
    void parsePacket();
};

#endif