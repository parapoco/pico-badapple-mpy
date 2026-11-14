#include "py/dynruntime.h"

// 展開関数: pythonから in_buf と out_buf の2つを受け取る
STATIC mp_obj_t decode_badapple(mp_obj_t in_obj, mp_obj_t out_obj) {
    mp_buffer_info_t in_buf;
    mp_buffer_info_t out_buf;

    // バッファのポインタとサイズを取得
    mp_get_buffer_raise(in_obj, &in_buf, MP_BUFFER_READ);
    mp_get_buffer_raise(out_obj, &out_buf, MP_BUFFER_WRITE);

    // バッファサイズのチェック (320x240の1bit=9600 bytes, 16bit=153600 bytes)
    if (in_buf.len < 9600 || out_buf.len < 153600) {
        mp_raise_ValueError(MP_ROM_QSTR(MP_QSTR_buffer_too_small));
    }

    const uint8_t *in_data = (const uint8_t *)in_buf.buf;
    uint16_t *out_data = (uint16_t *)out_buf.buf;

    // RGB565の白と黒（白黒なのでバイトスワップは不要）
    uint16_t color_white = 0xFFFF;
    uint16_t color_black = 0x0000;

    int out_idx = 0;
    // 9,600バイトを順番に読み、1バイトあたり8ピクセル（16バイト分）を展開
    for (size_t i = 0; i < 9600; i++) {
        uint8_t b = in_data[i];
        
        // ※MSBファースト(左端ピクセルが最上位ビット)の場合。
        // エンコード方法が逆なら (b & 0x01) から判定するように書き換えてください。
        out_data[out_idx++] = (b & 0x80) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x40) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x20) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x10) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x08) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x04) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x02) ? color_white : color_black;
        out_data[out_idx++] = (b & 0x01) ? color_white : color_black;
    }

    return mp_const_none;
}

// MicroPythonから呼び出せるように関数を登録
STATIC MP_DEFINE_CONST_FUN_OBJ_2(decode_badapple_obj, decode_badapple);

// モジュールの初期化関数 (import時に呼ばれる)
mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    MP_DYNRUNTIME_INIT_ENTRY

    // "decode" という名前で関数を登録
    mp_store_global(MP_QSTR_decode, MP_OBJ_FROM_PTR(&decode_badapple_obj));

    MP_DYNRUNTIME_INIT_EXIT
}