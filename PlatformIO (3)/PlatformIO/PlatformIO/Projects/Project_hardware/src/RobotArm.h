#ifndef ROBOTARM_H
#define ROBOTARM_H

#include <Arduino.h>
#include <Servo.h>
#include "Settings.h"
#include "Conveyor.h" // 비상정지 시 컨베이어 제어를 위해 필요

class RobotArm {
private:
Servo servos[6];
Servo servocover[2];
int currentAngles[6];
Conveyor* conveyor; // 비상정지 시 멈추기 위해 참조

// 내부에서만 쓰는 안전 함수
void checkSerialForEStop();
void safeDelay(unsigned long ms);

public:
bool isEmergencyStopped;
bool isSystemRunning; // ★ 추가: 시스템이 정식으로 가동 중인지 확인

RobotArm(Conveyor* conv); // 생성자
void init(); // 초기화 및 attach

void resetEmergencyStop();
// 기본 이동 함수
void moveToAngles(int targetAngles[6]);
void moveToState(int stateIndex);

// 덮개 제어
void up_open();
void down_open();

// 0번 모터 제어 (360도)
void move_left();
void move_right();

// 복합 동작 함수들 (기존 코드 그대로)
void action();
void ready();
void action_Normal();
void action_defective();

// 세부 동작들
void initial_state1();
void initial_state2();
void initial_state3();
void down();
void up();
void late_up();
void twice_up();
void twice_down();
void late_left();
void pick_down();
void pick_up();
void move_down();
void normal_judgement();
void defective_judgment();

// 비상 정지
void stopAllHardware();
};


#endif