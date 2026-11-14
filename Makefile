# モジュール名 (生成されるファイルは badapple.mpy になる)
MOD = badapple

# コンパイルするCファイル
SRC = badapple.c

# Pico 2 / Pico 2 W (RP2350) の指定アーキテクチャ
ARCH = armv7emsp

# MicroPythonリポジトリへのパス (適宜変更してください)
MPY_DIR = ../micropython

# dynruntime.mkをインクルード
include $(MPY_DIR)/py/dynruntime.mk