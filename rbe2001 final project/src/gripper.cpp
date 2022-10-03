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
}

void gripper:: release(){
    servo.writeMicroseconds(countToMs(start));
}

void gripper:: close(){
    Serial.println("start");
    int currentEncoderRead = analogRead(ServoEncoder);
    int previousEencoderRead = currentEncoderRead;
    int writeCount = start;
    int step = 0;
    while(currentEncoderRead < end){
        Serial.print("closing: ");
        Serial.print(writeCount);
        Serial.print(" ");
        writeCount += servoMsPerCount;
        servo.writeMicroseconds(writeCount);
        Serial.println(writeCount);
        Serial.print("previous read: ");
        Serial.println(previousEencoderRead);
        currentEncoderRead = analogRead(ServoEncoder);
        Serial.print("current read: ");
        Serial.println(currentEncoderRead);
        previousEencoderRead = currentEncoderRead;
        delay(10);
    }
}