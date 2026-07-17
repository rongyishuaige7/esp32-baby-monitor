# 本地 HTTP 协议与语义边界

## 启用条件

只有在本机创建 `src/wifi_credentials.h`、填入可用 2.4 GHz Wi-Fi 凭据且 ESP32 成功连接后，HTTP 服务才会启动。固件不会提供默认 SSID、密码或开放热点。

HTTP 服务是无 TLS、无认证、无访问控制、无速率限制、无审计、无设备身份校验的教学接口。它只能在隔离可信局域网使用，不能暴露公网、端口转发或用于远程看护。

## `GET /api/status`

返回当前程序执行路径中可得的数值和演示级字段。例如：

```json
{
  "scope": "demonstration_only",
  "distanceClassification": "ambiguous",
  "topDistanceCm": 42.1,
  "sideDistanceCm": 29.4,
  "temperatureC": 23.0,
  "humidityPercent": 62.0,
  "environmentReadingUsable": true,
  "soundAmplitude": 128,
  "signalLevel": "attention",
  "buzzerMuted": false,
  "distancePatternUsable": true
}
```

示例中的数值是格式示例，不是当前设备、儿童、环境或传感器的真实数据。字段说明：

| 字段 | 含义 | 不代表 |
| :-- | :-- | :-- |
| `distanceClassification` | 两个距离读数经过固定条件产生的中性标签 | 人体、婴儿、姿态、睡姿、趴睡或安全判断 |
| `topDistanceCm` / `sideDistanceCm` | 程序再次读取超声波模块时得到的数值 | 经标定距离、可靠存在检测或连续实时数据 |
| `temperatureC` / `humidityPercent` | 最近一次环境检查缓存的 DHT 读数 | 准确环境监测、舒适性/安全/医疗建议 |
| `soundAmplitude` | 此次 ADC 峰峰值采样 | 哭声、音频内容、情绪、风险或通知触发 |
| `signalLevel` | 本地固定阈值/组合代码的提示等级 | 正常、危险、报警、安全、健康、已通知或已处理 |
| `buzzerMuted` | 程序中的蜂鸣器静音标志 | 告警已确认、有人已响应或风险已解除 |

## `POST /api/mute`

切换本机蜂鸣器静音标志并返回：

```json
{"scope":"demonstration_only","buzzerMuted":true}
```

HTTP 200 只表示这个 HTTP 处理函数完成。它不确认蜂鸣器实际电平/响声、远端收到结果、有人看护、通知已送达、风险消失或任何安全处置完成。
