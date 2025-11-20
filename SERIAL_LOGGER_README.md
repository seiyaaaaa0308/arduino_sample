# Arduino Serial Logger

## 概要
Arduinoのシリアル出力を日時付きでログファイルに保存するPythonスクリプトです。

## 必要なライブラリ
```bash
pip install pyserial
```

## 使用方法

### 基本的な使い方（自動ポート検出）
```bash
python serial_logger.py
```

### ポート指定
```bash
# Windows
python serial_logger.py COM3

# Linux/Mac
python serial_logger.py /dev/ttyUSB0
```

## 機能
- Arduinoポートの自動検出
- 日時付きログ出力
- ログファイルへの自動保存（`logs/`ディレクトリ）
- コンソールへのリアルタイム表示

## ログファイル形式
```
Arduino Serial Log - Started at 2025-01-20 15:30:45
============================================================
[2025-01-20 15:30:47.123] =================================
[2025-01-20 15:30:47.124] Collision Warning System v2.0
[2025-01-20 15:30:47.125] Initializing components...
[2025-01-20 15:30:47.126] =================================
[2025-01-20 15:30:47.234] [DEBUG] DistanceSensor initialized: TRIG=8, ECHO=9
...
```

## 終了方法
`Ctrl+C` でログ記録を停止します。

## ログファイルの保存先
`logs/arduino_log_YYYYMMDD_HHMMSS.txt`

例: `logs/arduino_log_20250120_153045.txt`

## トラブルシューティング

### ポートが見つからない
- Arduinoが正しく接続されているか確認
- USBドライバがインストールされているか確認
- 手動でポートを指定して実行

### 文字化けする
- ボーレートが正しいか確認（デフォルト: 9600）
- スクリプト内の`baudrate`パラメータを変更

### パーミッションエラー（Linux/Mac）
```bash
sudo usermod -a -G dialout $USER
# 再ログイン後に有効
```

## カスタマイズ

### ボーレート変更
```python
logger = ArduinoLogger(port=None, baudrate=115200, log_dir="logs")
```

### ログディレクトリ変更
```python
logger = ArduinoLogger(port=None, baudrate=9600, log_dir="my_logs")
```
