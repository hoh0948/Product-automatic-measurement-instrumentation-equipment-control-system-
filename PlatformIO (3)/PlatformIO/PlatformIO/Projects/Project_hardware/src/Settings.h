#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// ------------------- 핀 번호 설정 -------------------
const int SERVO_PINS[6] = {2, 3, 4, 5, 6, 7};
const int COVER_PINS[2] = {8, 22};

#define IR_SENSOR_PIN 42
#define ENA_PIN 10
#define IN1_PIN 11
#define IN2_PIN 12

// ------------------- 시간 및 속도 설정 -------------------
const int SPEED_DELAY = 20;
const int ROTATION_TIME_180 = 800;
const int ROTATION_TIME_90 = 950;
const int ROTATION_TIME_LEFT_90 = 1100;
const int I_ROTATION_TIME_90 = 240;
const int I_ROTATION_TIME_LEFT_90 = 240;
const int CONTINUOUS_SERVO_INDEX = 4; // 360도 모터 인덱스

// ------------------- 로봇팔 상태 각도 데이터 -------------------
// (배열 내용은 보내주신 코드 그대로입니다)
const int STATE_ANGLES[38][6] = {
  {90, 90, 170, 60, 90, 25}, //0 initial_state1
  {90, 90, 145, 60, 90, 50}, //1 move_left
  {90, 35, 120, 60, 90, 50}, //2 move_down
  {90, 30, 120, 90, 90, 50}, //3 move_down
  {90, 28, 120, 90, 90, 50}, //4 move_down
  {90, 28, 120, 90, 90, 25}, //5 pick_down
  {90, 90, 145, 60, 90, 25}, //6 move_right
  {90, 160, 10, 80, 90, 25},  //7
  {90, 90, 90, 90, 90, 25},  //8
  {90, 90, 165, 60, 90, 25}, //9
  {90, 90, 150, 60, 90, 25}, //10
  {90, 90, 135, 60, 90, 25}, //11
  {90, 10, 110, 90, 90, 25}, //12 down
  {90, 10, 110, 90, 90, 50}, //13 down
  {90, 90, 165, 60, 90, 50}, //14 up
  {90, 90, 155, 60, 90, 50}, //15 up
  {90, 90, 145, 60, 90, 50}, //16 up
  {90, 90, 130, 60, 90, 25}, //17
  {90, 50, 120, 90, 90, 25}, //18 late_up
  {90, 90, 165, 60, 90, 25}, //19 late_up
  {90, 120, 175, 60, 90, 25}, //20 late_up
  {90, 30, 110, 85, 90, 50}, //21 down
  {90, 28, 120, 90, 90, 50}, //22 pick_up
  {90, 50, 120, 90, 90, 50}, //23 twice_up
  {90, 90, 165, 60, 90, 50}, //24 twice_up
  {90, 90, 155, 60, 90, 50}, //25 twice_up
  {90, 35, 120, 60, 90, 25}, //26 twice_down
  {90, 30, 120, 90, 90, 25}, //27 twice_down
  {90, 28, 120, 90, 90, 25}, //28 twice_down
  {90, 90, 170, 60, 90, 25}, //29 initial_state2
  {90, 90, 170, 60, 90, 25}, //30 initial_state3
  {90, 90, 155, 60, 90, 50}, //31 normal_judgement
  {90, 90, 175, 60, 90, 25}, //32 normal_judgement
  {90, 90, 155, 60, 90, 50}, //33 defective_judgment
  {90, 35, 120, 90, 90, 50}, //34 defective_judgment
  {90, 30, 120, 90, 90, 50}, //35 defective_judgment
  {90, 30, 120, 90, 90, 50}, //36 defective_judgment
  {90, 25, 120, 90, 90, 25}, //37 defective_judgment
};

#endif