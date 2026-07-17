# 第三方组件与再分发说明

本仓只分发 Rongyi 整理后的项目源码与文档。框架和依赖由 PlatformIO 在构建时从上游获取；使用、再分发或发布二进制前，请以其当前许可证、版本和分发条款为准。

| 组件 | 用途 | 来源 / 许可入口 |
| :-- | :-- | :-- |
| Espressif32 Platform | ESP32 PlatformIO 构建平台 | https://github.com/platformio/platform-espressif32 · Apache-2.0 |
| Arduino-ESP32 | ESP32 Arduino 框架、Wi-Fi 与 WebServer | https://github.com/espressif/arduino-esp32 · LGPL-2.1-or-later |
| ArduinoJson 6.21.6 | HTTP JSON 序列化 | https://github.com/bblanchon/ArduinoJson · MIT |
| DHT sensor library 1.4.7 | DHT11 读取 | https://github.com/adafruit/DHT-sensor-library · MIT |
| Adafruit Unified Sensor 1.1.15 | DHT 依赖接口 | https://github.com/adafruit/Adafruit_Sensor · MIT |

仓库不分发 Android App 源码、固件二进制、实物照片、视频、音频、儿童影像、EDA、PCB、Gerber、制造文件或任何网络凭据。原工程的 `lib/HCSR04` 与 `lib/SoundDetector` 源码作为 Rongyi 的项目源码随本仓 MIT 发布；DHT 访问已切换至上述上游 Adafruit 库，不再随仓分发原始本地 DHT11 包装层。
