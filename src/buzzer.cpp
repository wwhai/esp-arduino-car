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

#include "buzzer.h"

// 构造函数
Buzzer::Buzzer(uint8_t pin, uint16_t frequency, uint16_t duration)
    : buzzerPin(pin), beepFrequency(frequency), beepDuration(duration) {}

// 初始化蜂鸣器
void Buzzer::setup()
{
    ledcAttachPin(buzzerPin, 0);    // 将 GPIO 引脚绑定到 PWM 通道 0
    ledcSetup(0, beepFrequency, 8); // 配置 PWM 通道 0，设置频率和分辨率
}

// 播放滴声音
void Buzzer::beep()
{
    // 启动 PWM 输出
    ledcWriteTone(0, beepFrequency);
    delay(beepDuration);
    // 停止 PWM 输出
    ledcWriteTone(0, 0);
}
