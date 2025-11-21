#!/usr/bin/env python3
"""
Arduino シリアルモニターログ記録スクリプト
シリアルポートからのデータを日時付きでファイルに保存
"""

import serial
import serial.tools.list_ports
import datetime
import os
import sys
import time

class ArduinoLogger:
    def __init__(self, port=None, baudrate=9600, log_dir="logs"):
        """
        Arduino ログ記録クラス
        
        Args:
            port: シリアルポート名（Noneの場合は自動検出）
            baudrate: ボーレート（デフォルト: 9600）
            log_dir: ログ保存ディレクトリ
        """
        self.baudrate = baudrate
        self.log_dir = log_dir
        self.serial_conn = None
        
        # ログディレクトリ作成
        if not os.path.exists(log_dir):
            os.makedirs(log_dir)
            print(f"[INFO] ログディレクトリを作成しました: {log_dir}")
        
        # ポート検出または指定
        if port is None:
            self.port = self.detect_arduino_port()
        else:
            self.port = port
        
        if self.port is None:
            raise Exception("Arduinoが接続されていません")
        
        print(f"[INFO] 使用ポート: {self.port}")
        
    def detect_arduino_port(self):
        """Arduino接続ポートを自動検出"""
        ports = serial.tools.list_ports.comports()
        for port in ports:
            # Arduino Uno/Mega などを検出
            if 'Arduino' in port.description or 'CH340' in port.description or 'USB' in port.description:
                print(f"[INFO] Arduinoを検出: {port.device} - {port.description}")
                return port.device
        return None
    
    def connect(self):
        """シリアルポートに接続"""
        try:
            self.serial_conn = serial.Serial(self.port, self.baudrate, timeout=1)
            time.sleep(2)  # Arduino リセット待機
            print(f"[INFO] {self.port} に接続しました（{self.baudrate} bps）")
            return True
        except serial.SerialException as e:
            print(f"[ERROR] シリアルポート接続エラー: {e}")
            return False
    
    def disconnect(self):
        """シリアルポート切断"""
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.close()
            print("[INFO] シリアルポートを切断しました")
    
    def get_log_filename(self):
        """ログファイル名生成（日時ベース）"""
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"arduino_log_{timestamp}.txt"
        return os.path.join(self.log_dir, filename)
    
    def start_logging(self):
        """ログ記録開始"""
        if not self.connect():
            return
        
        log_file = self.get_log_filename()
        print(f"[INFO] ログファイル: {log_file}")
        print("[INFO] ログ記録を開始します（Ctrl+C で終了）")
        print("=" * 60)
        
        try:
            with open(log_file, 'w', encoding='utf-8') as f:
                # ヘッダー書き込み
                header = f"Arduino Serial Log - Started at {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
                header += "=" * 60 + "\n"
                f.write(header)
                f.flush()
                
                while True:
                    if self.serial_conn.in_waiting > 0:
                        try:
                            line = self.serial_conn.readline().decode('utf-8', errors='ignore').strip()
                            if line:
                                # 日時情報付きでログ出力
                                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                                log_entry = f"[{timestamp}] {line}\n"
                                
                                # ファイルに書き込み
                                f.write(log_entry)
                                f.flush()
                                
                                # コンソールにも出力
                                print(log_entry.strip())
                        except UnicodeDecodeError:
                            pass
                    
                    time.sleep(0.01)  # CPU負荷軽減
                    
        except KeyboardInterrupt:
            print("\n[INFO] ログ記録を停止しました")
        except Exception as e:
            print(f"[ERROR] エラーが発生しました: {e}")
        finally:
            self.disconnect()
            print(f"[INFO] ログを保存しました: {log_file}")

def main():
    """メイン関数"""
    print("=" * 60)
    print("Arduino Serial Logger")
    print("=" * 60)
    
    # コマンドライン引数からポート指定可能
    port = None
    if len(sys.argv) > 1:
        port = sys.argv[1]
    
    try:
        logger = ArduinoLogger(port=port, baudrate=9600, log_dir="logs")
        logger.start_logging()
    except Exception as e:
        print(f"[ERROR] {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
