#include "Settings.h"
#include "Conveyor.h"
#include "Sensor.h"
#include "RobotArm.h"

// 객체 생성
Conveyor conveyor;
Sensor sensor;
RobotArm arm(&conveyor);

// 현재 로봇팔이 작업 중인지 확인하는 변수
bool isProcessing = false; 

// [수정] 동작 완료 후 센서가 떨어질 때까지 대기하는 함수 (안전장치 추가됨)
void finishCycleAndCheckSensor() {
    // 1. 이미 비상 정지 상태라면 즉시 리턴 (무한 대기 방지)
    if (arm.isEmergencyStopped) return;

    // 기계적 진동 안정화
    delay(100);

    // 2. 물체가 치워질 때까지 대기
    while (sensor.isDetected()) {
        // 대기 중이라도 비상 정지 걸리면 즉시 탈출
        if (arm.isEmergencyStopped) break; 
        delay(10); 
    }

    // 비상 정지가 아닐 때만 처리 완료 플래그 해제
    if (!arm.isEmergencyStopped) {
        isProcessing = false; 
    }
}

void setup() {
    Serial.begin(9600);
    
    conveyor.init();
    sensor.init();
    conveyor.run();
    
    Serial.println("초기화: 로봇팔을 Home 위치로 이동합니다.");
    arm.init();
    
    // 시스템 강제 가동
    arm.isSystemRunning = true; 
    
    Serial.println("Arduino: 준비 완료. 물체 감지 대기 중...");
}

void loop() {
    // 1. PC로부터 판정 데이터 수신
    if (Serial.available() > 0) {
        String line = Serial.readStringUntil('\n');
        line.trim();
        
        if (line == "E_STOP") {
            arm.stopAllHardware();
            // 여기서 return을 하지 않고 아래 로직들이 실행되지 않도록 흐름 제어
        }
        else if (line == "RESET") {
            Serial.println("1");
            arm.resetEmergencyStop(); // 하드웨어 및 플래그 초기화
            Serial.println("2");
            conveyor.run();
            Serial.println("3");
            isProcessing = false; 
            Serial.println("명령: 리셋 완료. 대기 중...");
        }
        // 비상 정지 상태가 아닐 때만 동작 명령 수행
        else if (isProcessing == true && !arm.isEmergencyStopped) { 
            if (line == "NORMAL") {
                Serial.println("판정: 정상(Normal)");
                arm.action_Normal(); 
                
                // [핵심] 비상 정지가 아닐 때만 센서 대기 실행
                if (!arm.isEmergencyStopped) {
                    finishCycleAndCheckSensor(); 
                }
            }
            else if (line == "DEFECT") {
                Serial.println("판정: 불량(Defective)");
                arm.action_defective(); 
                
                // [핵심] 비상 정지가 아닐 때만 센서 대기 실행
                if (!arm.isEmergencyStopped) {
                    finishCycleAndCheckSensor();
                }
            }
            else if (line == "UP") {
                Serial.println("명령: 덮개 열기");
                arm.up_open(); 
            }
            else if (line == "DOWN") {
                Serial.println("명령: 덮개 닫기");
                arm.down_open(); 
            }
        }
    }

    // 비상 정지 상태라면 loop의 나머지 부분(센서 감지 등)은 실행하지 않음
    if (arm.isEmergencyStopped) return;

    // 2. 센서 감지
    if (arm.isSystemRunning && !isProcessing) { 
        if (sensor.isDetected()) {
            // [노이즈 방지]
            delay(100); 

            if (sensor.isDetected()) {
                // 감지 직후 비상 정지인지 한 번 더 체크
                if (arm.isEmergencyStopped) return;

                Serial.println("!! 물체 감지 !! -> action() 시작");
                
                isProcessing = true; // 중복 감지 차단
                arm.action();        // 물체 집어 올리기 (내부에서 E_STOP 체크)
                
                // action() 중 비상정지가 발생했다면 isProcessing을 풀지 않고 유지
                if (!arm.isEmergencyStopped) {
                    Serial.println("상태: PC 판정 대기 중...");
                }
            }
        }
    }
} // <--- [오류 해결] 이 괄호가 없어서 에러가 났었습니다.