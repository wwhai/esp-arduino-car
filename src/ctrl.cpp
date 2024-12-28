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
#include "ctrl.h"

// 构造函数，初始化 RF24 无线模块
Ctrl::Ctrl() : radio(CE_PIN, CSN_PIN) {}

// 初始化设置
void Ctrl::setup()
{
    // 设置电机控制引脚为输出模式
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);
    pinMode(motorC1, OUTPUT);
    pinMode(motorC2, OUTPUT);
    pinMode(motorD1, OUTPUT);
    pinMode(motorD2, OUTPUT);

    // 初始化 nRF24L01 无线模块
    radio.begin();
    radio.openReadingPipe(0, address); // 打开接收管道 0，使用指定地址
    radio.setPALevel(RF24_PA_MIN);     // 设置功率级别为最小
    radio.startListening();            // 进入监听模式
}

// 控制电机 A 正转
void Ctrl::motorAForward()
{
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
}

// 控制电机 A 反转
void Ctrl::motorAReverse()
{
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
}

// 控制电机 A 停止
void Ctrl::motorAStop()
{
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
}

// 控制电机 B 正转
void Ctrl::motorBForward()
{
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
}

// 控制电机 B 反转
void Ctrl::motorBReverse()
{
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
}

// 控制电机 B 停止
void Ctrl::motorBStop()
{
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
}

// 控制电机 C 正转
void Ctrl::motorCForward()
{
    digitalWrite(motorC1, HIGH);
    digitalWrite(motorC2, LOW);
}

// 控制电机 C 反转
void Ctrl::motorCReverse()
{
    digitalWrite(motorC1, LOW);
    digitalWrite(motorC2, HIGH);
}

// 控制电机 C 停止
void Ctrl::motorCStop()
{
    digitalWrite(motorC1, LOW);
    digitalWrite(motorC2, LOW);
}

// 控制电机 D 正转
void Ctrl::motorDForward()
{
    digitalWrite(motorD1, HIGH);
    digitalWrite(motorD2, LOW);
}

// 控制电机 D 反转
void Ctrl::motorDReverse()
{
    digitalWrite(motorD1, LOW);
    digitalWrite(motorD2, HIGH);
}

// 控制电机 D 停止
void Ctrl::motorDStop()
{
    digitalWrite(motorD1, LOW);
    digitalWrite(motorD2, LOW);
}

// 小车前进
void Ctrl::forward()
{
    motorAForward();
    motorBForward();
    motorCForward();
    motorDForward();
}

// 小车后退
void Ctrl::reverse()
{
    motorAReverse();
    motorBReverse();
    motorCReverse();
    motorDReverse();
}

// 小车左转
void Ctrl::turnLeft()
{
    motorAReverse();
    motorBReverse();
    motorCForward();
    motorDForward();
}

// 小车右转
void Ctrl::turnRight()
{
    motorAForward();
    motorBForward();
    motorCReverse();
    motorDReverse();
}

// 小车停止
void Ctrl::stop()
{
    motorAStop();
    motorBStop();
    motorCStop();
    motorDStop();
}

// 计算 CRC16 校验和
uint16_t Ctrl::calculateCRC16(uint8_t *data, uint8_t len)
{
    return calculateCRC16(data, len);
}

// 解析接收到的数据包
void Ctrl::parsePacket()
{
    if (radio.available())
    {
        uint8_t packet[32];
        radio.read(packet, sizeof(packet));
        // 检查包头
        if (packet[0] == 0xAA && packet[1] == 0xBB)
        {
            uint8_t length = packet[2];
            uint8_t cmd = packet[3];
            uint16_t receivedCRC = (packet[length + 4] << 8) | packet[length + 5];
            uint8_t endMarker1 = packet[length + 6];
            uint8_t endMarker2 = packet[length + 7];
            // 检查结束标志
            if (endMarker1 == 0xEE && endMarker2 == 0xFF)
            {
                // 计算校验和
                uint16_t calculatedCRC = calculateCRC16(packet, length + 4);
                if (receivedCRC == calculatedCRC)
                {
                    // 检查命令码的合法性
                    if (cmd >= 0x01 && cmd <= 0x05)
                    {
                        switch (cmd)
                        {
                        case 0x01:
                            forward();
                            break;
                        case 0x02:
                            reverse();
                            break;
                        case 0x03:
                            turnLeft();
                            break;
                        case 0x04:
                            turnRight();
                            break;
                        case 0x05:
                            stop();
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
}

// 主循环中调用的函数，用于处理无线数据
void Ctrl::loop()
{
    parsePacket();
}