#include <Arduino.h>

#include "5641AS_driver.h"
#include "config.h"

D_5641AS::D_5641AS() {}

void D_5641AS::begin() {
  // Hot wire?
  pinMode(SEG_D_4, OUTPUT);
  pinMode(SEG_D_3, OUTPUT);
  pinMode(SEG_D_2, OUTPUT);
  pinMode(SEG_D_1, OUTPUT);

  pinMode(SEG_ENA, OUTPUT);
  pinMode(SEG_SIG, OUTPUT);

  pinMode(SEG_L_S0, OUTPUT);
  pinMode(SEG_L_S1, OUTPUT);
  pinMode(SEG_L_S2, OUTPUT);
  pinMode(SEG_L_S3, OUTPUT);
}