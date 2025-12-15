#include "RobotArm.h"

// 생성자
RobotArm::RobotArm(Conveyor* conv) {
    conveyor = conv;
    isEmergencyStopped = false;
    isSystemRunning = false; // (참고) 헤더 파일에 선언되지 않은 변수라면 주석 처리 필요
}

// 초기화
void RobotArm::init() {
    for (int i = 0; i < 6; i++) {
        servos[i].attach(SERVO_PINS[i]);
    }
    for (int i = 0; i < 2; i++) {
        servocover[i].attach(COVER_PINS[i]);
    }

    // 초기 상태로 이동
    for (int i = 0; i < 6; i++) {
        int homeAngle = STATE_ANGLES[0][i];
        currentAngles[i] = homeAngle;
        // 0번, 4번 모터 주의 (360도)
        if (i == 0 || i == CONTINUOUS_SERVO_INDEX) {
            servos[i].write(90); // 정지
        } else {
            servos[i].write(homeAngle);
        }
    }
    delay(100);
}

// ---------------- 안전 관련 함수 ----------------
void RobotArm::checkSerialForEStop() {
    if (isEmergencyStopped) return;
    // loop()에서 시리얼을 읽어 처리하므로 여기서는 플래그만 확인하거나
    // 필요 시 추가 구현. 현재 구조상 safeDelay에서 처리함.
}

void RobotArm::safeDelay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        if (Serial.available() > 0) {
            String s = Serial.peek() == 'E' ? Serial.readStringUntil('\n') : "";
            if (s.indexOf("E_STOP") != -1) {
                stopAllHardware();
                break;
            }
        }
        if (isEmergencyStopped) break;
    }
}

void RobotArm::stopAllHardware() {
    if (isEmergencyStopped) return;
    isEmergencyStopped = true;
    isSystemRunning = false; // 헤더에 선언되어 있지 않다면 주석 처리
    Serial.println("[!!] 비상 정지 실행!");
    conveyor->stop(); // 컨베이어 정지
    for (int i = 0; i < 6; i++) {
        servos[i].detach();
    }
}

// [추가] 비상 정지 상태를 해제하는 함수
void RobotArm::resetEmergencyStop() {
    isEmergencyStopped = false;
    isSystemRunning = true; // 시스템 가동 상태 복구
    
    Serial.println(">>> [알림] 비상 정지 해제. 하드웨어 재설정 중...");
    
    // 중요: 끊어졌던 서보 모터들을 다시 핀에 연결하고 초기 위치로 이동
    init(); 
    
    // (선택 사항) init() 안에 딜레이가 있으므로 완료 후 메시지 출력
    Serial.println(">>> [알림] 재가동 준비 완료.");
}

// ---------------- 이동 관련 함수 ----------------

void RobotArm::moveToAngles(int targetAngles[6]) {
    bool allServosAtTarget = false;
    while (!allServosAtTarget) {
        checkSerialForEStop();
        if (isEmergencyStopped) break;

        allServosAtTarget = true;
        for (int i = 0; i < 6; i++) {
            if (i == 0 || i == CONTINUOUS_SERVO_INDEX) continue; // 360도 제외

            if (currentAngles[i] < targetAngles[i]) {
                currentAngles[i]++;
                allServosAtTarget = false;
            } else if (currentAngles[i] > targetAngles[i]) {
                currentAngles[i]--;
                allServosAtTarget = false;
            }
            servos[i].write(currentAngles[i]);
        }
        safeDelay(SPEED_DELAY);
    }
}

void RobotArm::moveToState(int stateIndex) {
    int targetAngles[6];
    for(int i=0; i<6; i++) targetAngles[i] = STATE_ANGLES[stateIndex][i];
    
    // moveToAngles 로직과 동일하지만 내부 구현 중복 방지 위해 복사 사용
    bool allServosAtTarget = false;
    while (!allServosAtTarget) {
        checkSerialForEStop();
        if (isEmergencyStopped) break;

        allServosAtTarget = true;
        for (int i = 0; i < 6; i++) {
            if (i == 0 || i == CONTINUOUS_SERVO_INDEX) continue;
            
            if (currentAngles[i] < targetAngles[i]) {
                currentAngles[i]++;
                allServosAtTarget = false;
            } else if (currentAngles[i] > targetAngles[i]) {
                currentAngles[i]--;
                allServosAtTarget = false;
            }
            servos[i].write(currentAngles[i]);
        }
        safeDelay(SPEED_DELAY);
    }
}

// ---------------- 동작 함수들 (기존 로직) ----------------

void RobotArm::move_left() {
    servos[0].write(111);
    safeDelay(ROTATION_TIME_LEFT_90);
    servos[0].write(90);
    currentAngles[0] = 90;
}

void RobotArm::move_right() {
    servos[0].write(69);
    safeDelay(ROTATION_TIME_90);
    servos[0].write(90);
    currentAngles[0] = 90;
}

void RobotArm::up_open() {
    servocover[0].write(90);
    delay(50);
    servocover[1].write(180);
    delay(50);
}

void RobotArm::down_open() {
    servocover[0].write(0);
    delay(50);
    servocover[1].write(80);
    delay(50);
    delay(5000);
    up_open();
}

// --- 세부 동작들 (단순 호출로 정리) ---
void RobotArm::initial_state1() { moveToState(0); safeDelay(500); }
void RobotArm::initial_state2() { moveToState(31); safeDelay(500); }
void RobotArm::initial_state3() { moveToState(32); safeDelay(500); }

void RobotArm::down() {
    moveToState(12); safeDelay(500);
    moveToState(13); safeDelay(500);
}
void RobotArm::up() {
    moveToState(21); safeDelay(50);
    moveToState(14); safeDelay(50);
    moveToState(15); safeDelay(50);
    moveToState(16); safeDelay(50);
}
void RobotArm::late_up() {
    moveToState(18); safeDelay(50);
    moveToState(19); safeDelay(50);
    moveToState(20); safeDelay(7000);
}
void RobotArm::twice_up() {
    moveToState(23); safeDelay(50);
    moveToState(24); safeDelay(50);
    moveToState(25); safeDelay(50);
}
void RobotArm::twice_down() {
    moveToState(26); safeDelay(50);
    moveToState(27); safeDelay(50);
    moveToState(28); safeDelay(50);
}
void RobotArm::late_left() {
    moveToState(23); safeDelay(50);
    moveToState(24); safeDelay(50);
}
void RobotArm::pick_down() { moveToState(5); safeDelay(500); }
void RobotArm::pick_up() { moveToState(22); safeDelay(500); }
void RobotArm::move_down() {
    moveToState(2); safeDelay(50);
    moveToState(3); safeDelay(50);
    moveToState(4); safeDelay(50);
}
void RobotArm::normal_judgement() {
    moveToState(31); safeDelay(50);
    moveToState(32); safeDelay(50);
}
void RobotArm::defective_judgment() {
    moveToState(33); safeDelay(50);
    moveToState(34); safeDelay(50);
    moveToState(35); safeDelay(50);
    moveToState(36); safeDelay(50);
    moveToState(37); safeDelay(50);
}

void RobotArm::ready() {
    moveToState(0); safeDelay(50);
    moveToState(9); safeDelay(50);
    moveToState(10); safeDelay(50);
    moveToState(11); safeDelay(50);
    moveToState(17); safeDelay(50);
    move_left();
}

void RobotArm::action() {
    initial_state1(); if(isEmergencyStopped) return;
    down(); if(isEmergencyStopped) return;
    up(); if(isEmergencyStopped) return;
    move_left(); if(isEmergencyStopped) return;
    move_down(); if(isEmergencyStopped) return;
    pick_down(); if(isEmergencyStopped) return;
    late_up(); if(isEmergencyStopped) return;
}

void RobotArm::action_Normal() {
    Serial.println("Action: 정상(Normal) 분류 동작 시작");

    // 요청하신 순서: twice_down -> pick_up -> twice_up -> move_left -> normal_judgement -> move_right -> move_right
    
    twice_down();       if(isEmergencyStopped) return;
    pick_up();          if(isEmergencyStopped) return;
    twice_up();         if(isEmergencyStopped) return;
    move_left();        if(isEmergencyStopped) return; // 0번 모터 회전
    normal_judgement(); if(isEmergencyStopped) return;

    initial_state1();   if(isEmergencyStopped) return;
    
    // move_right 2회 반복
    move_right();       if(isEmergencyStopped) return;
    move_right();       if(isEmergencyStopped) return;
    
    Serial.println("Action: 정상 분류 완료");
}

void RobotArm::action_defective() {
    Serial.println("Action: 불량(Defective) 분류 동작 시작");

    twice_down();       if(isEmergencyStopped) return;
    pick_up();          if(isEmergencyStopped) return;
    twice_up();         if(isEmergencyStopped) return;
    move_left();        if(isEmergencyStopped) return; // 0번 모터 회전
    defective_judgment();if(isEmergencyStopped) return;

    initial_state1();   if(isEmergencyStopped) return;

    move_right();       if(isEmergencyStopped) return;
    move_right();       if(isEmergencyStopped) return;

    Serial.println("Action: 불량 분류 완료");
}