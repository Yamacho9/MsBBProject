#include "app.h"

#include "Tracer.h"

using namespace ev3api;

Tracer tracer;

void tracer_cyc(intptr_t exinf) {
    act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {
  if (ev3_button_is_pressed(LEFT_BUTTON)) {
    wup_tsk(MAIN_TASK);  // 左ボタン押下でメインを起こす
  } else {
    tracer.run();  // ;走行
  }
  ext_tsk();
}

void main_task(intptr_t unused) {
  tracer.init();
  ev3_sta_cyc(TRACER_CYC);
  slp_tsk();  // 起きたら、走行をやめる
  ev3_stp_cyc(TRACER_CYC);
  tracer.terminate();
  ext_tsk();
}
