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
// 定义电机驱动引脚
#define A1_PIN 5
#define A2_PIN 18
#define B1_PIN 19
#define B2_PIN 21
#define C1_PIN 22
#define C2_PIN 23
#define D1_PIN 25
#define D2_PIN 26

// 定义协议标志
#define START_FLAG_1 0xAA
#define START_FLAG_2 0xBB
#define END_FLAG_1 0xEE
#define END_FLAG_2 0xFF

class CarController
{
public:
    CarController();
    void setup();
    void loop();
    bool selfTest(); // 新增自检功能

private:
    uint8_t globalBuffer[64] = {0};
    uint8_t bufferIndex = 0;
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
    void readSerialData();
    uint16_t calculateCRC16(uint8_t *data, uint8_t len);
    void parsePacket();

    bool testMotor(const String &motorName,
                   void (CarController::*forward)(), void (CarController::*reverse)());
};

#endif