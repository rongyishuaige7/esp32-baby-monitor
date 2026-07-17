# 源码来源与公开范围

## 权威来源裁决

公开候选从以下桌面原工程隔离复制：

```text
/home/rongyi/桌面/baby_monitor
```

原工程存在 Git 历史，但发布准备时工作区不干净且没有远程仓库。其历史基线为：

```text
branch: master
HEAD: 4d108b2167c81b242632230526ac38b52364bcb5
subject: Fix critical issues in WiFiServer
```

发布前工作区相对历史 HEAD 的差异包括：`platformio.ini`、`src/` 多个模块、`lib/DHT11/`、`lib/SoundDetector/`，以及未跟踪的 `.gitignore`、`README.md`；`include/README` 与 `lib/README` 在工作区被删除。当前有效工程文件不含 Android App 源码、照片、视频、EDA、PCB、Gerber 或制造文件。

经裁决，**原工程的当前工作区版本**是本次公开候选的功能来源；历史 HEAD 只用于记录来源差异。原工程和历史 Git 不被本仓清理、提交、推送或反向修改。所有净化、文档、CI 和 Git 提交只发生在：

```text
/home/rongyi/桌面/esp32-baby-monitor
```

## 公开整理

候选保留有效固件功能的基础上，进行了以下必要公开整理：

- 固定 PlatformIO 平台与直接依赖版本，移除未使用的注释环境；
- 新增不含默认凭据的 `wifi_credentials.example.h`，真实 `wifi_credentials.h` 被忽略；无凭据时不连接 Wi-Fi、不启动 HTTP；
- 将 HTTP 路径统一为 `/api/status` 与 `/api/mute`，响应只陈述演示级读数、分类和静音状态；
- 移除 Android App 是当前可复现范围的表述：原 README 提过 App，但当前来源没有 App 源码；
- 将“正常/危险/趴睡/哭声报警/安全”等强结论改为中性的距离组合、固定阈值和本地提示语义；
- 无有效距离或环境读数时显式使用 `unknown`，不再将失效读数默认为“正常”；
- 删除未使用的本地 `lib/DHT11` 包装层，使用 PlatformIO 获取的 Adafruit DHT 库；
- 添加 MIT、第三方声明、硬件边界、BOM、接线边界图、来源/状态/协议/验证文档、密钥与生成物扫描、源码契约和固定 Action 工作流。

这些修改不反写原工程，也不声称公开候选与原工作区逐字节一致。其目的仅是让公开版本的凭据、网络、硬件和婴儿相关风险边界可审计。

## 不公开内容

本仓不包含：

- Wi-Fi SSID、密码、私网 IP、设备日志或真实网络配置；
- `.pio/`、固件二进制、ELF、MAP、IDE 状态或系统缓存；
- Android App 源码（当前未找到，故不作为公开可复现能力）；
- 实物照片、视频、音频、儿童影像、运行截图或当前真机日志；
- EDA、原理图、PCB、Gerber、制造文件、外壳或装配资料；
- 任何可推断儿童、家庭、地址、身份或健康信息的数据。

缺少这些内容不阻断源码开源；但在补齐并绑定精确 Git commit 前，不能把它们写成已公开、已验证或可复现。
