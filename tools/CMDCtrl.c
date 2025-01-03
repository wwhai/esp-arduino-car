// Copyright (C) 2025 wwhai
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

#include <windows.h>
#include <stdio.h>

int main()
{
    // 打开串口
    HANDLE hSerial = CreateFile(
        "COM9", // 串口名称，例如"COM3"
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Error opening serial port.\n");
        return 1;
    }

    // 配置串口参数
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error getting serial port state.\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200; // 波特率
    dcbSerialParams.ByteSize = 8;          // 数据位
    dcbSerialParams.StopBits = ONESTOPBIT; // 停止位
    dcbSerialParams.Parity = NOPARITY;     // 无校验位

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error setting serial port state.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 设置超时
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Error setting timeouts.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 发送数据
    const char *text = "Hello, serial port!";
    DWORD bytes_written;
    if (!WriteFile(hSerial, text, strlen(text), &bytes_written, NULL))
    {
        printf("Error writing to serial port.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 接收数据
    char buffer[1024];
    DWORD bytes_read;
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytes_read, NULL))
    {
        printf("Error reading from serial port.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 打印接收到的数据
    printf("Received %d bytes: %s\n", bytes_read, buffer);

    // 关闭串口
    CloseHandle(hSerial);

    return 0;
}
