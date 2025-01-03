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
Buzzer::Buzzer(uint8_t pin, uint16_t duration)
    : buzzerPin(pin), beepDuration(duration) {}

// 初始化蜂鸣器
void Buzzer::setup()
{
    pinMode(buzzerPin, OUTPUT);
}

// 播放滴声音
void Buzzer::beep()
{
    digitalWrite(buzzerPin, LOW);
    tone(buzzerPin, 440, 100);
    noTone(buzzerPin);
    digitalWrite(buzzerPin, HIGH);
}
