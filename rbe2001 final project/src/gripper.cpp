#include<gripper.h>
Servo32U4 servo;
gripper:: gripper(int minEncoder, int maxEncoder){
    start = minEncoder;
    end = maxEncoder;
}

int gripper:: countToMs(int count){
    return count * servoMsPerCount;
}
void gripper:: init(){
    servo.setMinMaxMicroseconds(servoMin,servoMax);
    close();
}

void gripper:: release(){
    servo.writeMicroseconds(countToMs(start));
    int currentCount = analogRead(servoEncoder);
    while(abs(start - currentCount) > servoErrorTolerance){
        servo.writeMicroseconds(countToMs(start));
        Serial.println("running: ");
        currentCount = analogRead(servoEncoder);
    }
    Serial.println(analogRead(servoEncoder));
    Serial.println("released");
}

void gripper:: close(){
    bool gripping = true;
    int stallCount = 0;
    while(gripping){
        servo.writeMicroseconds(countToMs(gripperEndEncoderCount));
        delay(50);
        bool incomplete = abs(gripperEndEncoderCount - analogRead(servoEncoder)) > gripperLockZone;
        Serial.println(analogRead(servoEncoder));
        for(stallCount = 0; stallCount <  stall + 1 && incomplete; stallCount++){
            stallCount++;
            Serial.println("retry");
            servo.writeMicroseconds(countToMs(gripperEndEncoderCount));
            delay(50);
            Serial.println(analogRead(servoEncoder));
            incomplete = abs(gripperEndEncoderCount - analogRead(servoEncoder)) > gripperLockZone;
        }
        if(stallCount > stall){
            release();
            delay(1000);
        }else{
            gripping = false;
        }
        Serial.print("stall count: ");
        Serial.println(stallCount);
        Serial.print("incomplete: ");
        Serial.println(incomplete);
    }
    Serial.println("Done");
}

void gripper:: hold(){
    if(gripperEndEncoderCount - analogRead(servoEncoder) > servoErrorTolerance) servo.writeMicroseconds(countToMs(gripperEndEncoderCount));
}

void gripper:: closeTo(float percent){
    int travelLength = gripperEndEncoderCount - gripperStartEncoderCount;
    int writeValue = countToMs(start + (float)travelLength * percent);
    servo.writeMicroseconds(writeValue);
}