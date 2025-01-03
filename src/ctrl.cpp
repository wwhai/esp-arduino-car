#include "ctrl.h"
#include "buzzer.h"
Buzzer buzzer(27, 100);

CarController::CarController()
{
    buzzer.setup();
}
void init_gpio()
{
    pinMode(A1_PIN, OUTPUT);
    pinMode(A2_PIN, OUTPUT);
    pinMode(B1_PIN, OUTPUT);
    pinMode(B2_PIN, OUTPUT);
    pinMode(C1_PIN, OUTPUT);
    pinMode(C2_PIN, OUTPUT);
    pinMode(D1_PIN, OUTPUT);
    pinMode(D2_PIN, OUTPUT);
}
void CarController::setup()
{
    init_gpio();
    // 开机自检
    if (!selfTest())
    {
        Serial.println("Self-test failed! Please check hardware connections.");
        while (true)
            ; // 停止程序运行
    }

    Serial.println("Self-test passed!");
}
// 自检功能
bool CarController::selfTest()
{
    bool allPassed = true;

    Serial.println("Starting self-test...");
    allPassed &= testMotor("Motor A",
                           &CarController::motorAForward, &CarController::motorAReverse);
    allPassed &= testMotor("Motor B",
                           &CarController::motorBForward, &CarController::motorBReverse);
    allPassed &= testMotor("Motor C",
                           &CarController::motorCForward, &CarController::motorCReverse);
    allPassed &= testMotor("Motor D",
                           &CarController::motorDForward, &CarController::motorDReverse);

    return allPassed;
}

// 测试单个电机
bool CarController::testMotor(const String &motorName,
                              void (CarController::*forward)(), void (CarController::*reverse)())
{
    Serial.print("Testing ");
    Serial.print(motorName);
    Serial.println("...");
    // 正转测试
    (this->*forward)();
    buzzer.beep();
    delay(500);
    (this->*reverse)();
    buzzer.beep();
    delay(500);
    (this->*forward)();
    buzzer.beep();
    delay(500);
    stop();
    for (size_t i = 0; i < 5; i++)
    {
        buzzer.beep();
        delay(50);
    }
    Serial.println(motorName + " passed!");
    return true;
}

void CarController::loop()
{
    readSerialData();
    parsePacket();
}

// 电机A控制
void CarController::motorAForward()
{
    digitalWrite(A2_PIN, HIGH);
    digitalWrite(A1_PIN, LOW); // 01
}

void CarController::motorAReverse()
{
    digitalWrite(A2_PIN, LOW);
    digitalWrite(A1_PIN, HIGH); // 01
}

void CarController::motorAStop()
{
    digitalWrite(A1_PIN, LOW);
    digitalWrite(A2_PIN, LOW);
}

// 电机B控制
void CarController::motorBForward()
{
    digitalWrite(B1_PIN, LOW);
    digitalWrite(B2_PIN, HIGH);
}

void CarController::motorBReverse()
{
    digitalWrite(B1_PIN, HIGH);
    digitalWrite(B2_PIN, LOW);
}

void CarController::motorBStop()
{
    digitalWrite(B1_PIN, LOW);
    digitalWrite(B2_PIN, LOW);
}

// 电机C控制
void CarController::motorCForward()
{
    digitalWrite(C1_PIN, HIGH);
    digitalWrite(C2_PIN, LOW);
}

void CarController::motorCReverse()
{
    digitalWrite(C1_PIN, LOW);
    digitalWrite(C2_PIN, HIGH);
}

void CarController::motorCStop()
{
    digitalWrite(C1_PIN, LOW);
    digitalWrite(C2_PIN, LOW);
}

// 电机D控制
void CarController::motorDForward()
{
    digitalWrite(D1_PIN, HIGH);
    digitalWrite(D2_PIN, LOW);
}

void CarController::motorDReverse()
{
    digitalWrite(D1_PIN, LOW);
    digitalWrite(D2_PIN, HIGH);
}

void CarController::motorDStop()
{
    digitalWrite(D1_PIN, LOW);
    digitalWrite(D2_PIN, LOW);
}

// 整车控制
void CarController::forward()
{
    motorCForward();
    motorDForward();
    motorAForward();
    motorBForward();
}

void CarController::reverse()
{
    motorAReverse();
    motorBReverse();
    motorCReverse();
    motorDReverse();
}

void CarController::turnLeft()
{
    motorBStop();
    motorDStop();
    motorCForward();
    motorAForward();
}

void CarController::turnRight()
{
    motorBForward();
    motorDForward();
    motorAStop();
    motorCStop();
}

void CarController::stop()
{
    motorAStop();
    motorBStop();
    motorCStop();
    motorDStop();
}
// 定义全局缓冲区
uint8_t globalBuffer[64];

// Modbus CRC16 计算函数
uint16_t CarController::calculateCRC16(uint8_t *data, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    return crc;
}

void CarController::readSerialData()
{
    // 检查是否有可用的串口数据
    while (Serial.available())
    {
        uint8_t incomingByte = Serial.read();

        // 写入 globalBuffer，防止缓冲区溢出
        if (bufferIndex < sizeof(globalBuffer))
        {
            globalBuffer[bufferIndex++] = incomingByte;

            // 检测是否接收到完整数据包（结束标志）
            if (bufferIndex >= 4 &&
                globalBuffer[bufferIndex - 2] == 0xEE &&
                globalBuffer[bufferIndex - 1] == 0xFF)
            {
                // 调用解析函数
                parsePacket();
                // 重置缓冲区索引
                bufferIndex = 0;
            }
        }
        else
        {
            // 缓冲区溢出处理，重置索引
            Serial.println("Buffer overflow!");
            bufferIndex = 0;
        }
    }
}
// 解析数据包函数

void CarController::parsePacket()
{
    // 检查缓冲区是否至少包含一个完整的数据包
    if (bufferIndex < 8)
    { // 起始标志(2) + 数据长度(1) + 命令(1) + CRC(2) + 结束标志(2)
        return;
    }

    // 检查起始标志
    if (globalBuffer[0] != 0xAA || globalBuffer[1] != 0xBB)
    {
        Serial.println("Invalid start flag!");
        bufferIndex = 0; // 重置缓冲区
        return;
    }

    // 检查数据长度是否有效（固定为 1 字节命令）
    uint8_t dataLength = globalBuffer[2];
    if (dataLength != 3)
    {
        Serial.println("Invalid data length!");
        bufferIndex = 0; // 重置缓冲区
        return;
    }

    // 检查是否包含完整数据包
    uint8_t expectedPacketSize = 2 + 1 + 1 + 2 + 2; // 起始标志 + 数据长度 + 命令 + CRC + 结束标志
    if (bufferIndex < expectedPacketSize)
    {
        return; // 等待更多数据
    }

    // 检查结束标志
    if (globalBuffer[6] != 0xEE || globalBuffer[7] != 0xFF)
    {
        Serial.println("Invalid end flag!");
        bufferIndex = 0; // 重置缓冲区
        return;
    }
    // 提取命令码
    uint8_t commandCode = globalBuffer[3];
    Serial.print("Received command: ");
    Serial.println(commandCode, HEX);
    // 解析命令
    switch (commandCode)
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
    case 0x06:
        init_gpio();
        break;
    default:
        Serial.println("Unknown command!");
        break;
    }

    // 重置缓冲区
    bufferIndex = 0;
}
