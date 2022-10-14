#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

long oldValue = 0;
long newValue;
long count = 0;
unsigned time = 0;


BlueMotor::BlueMotor(){
    error = 0;
    pError = 0;
    reset();
}
void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    pinMode(startSensorPin,INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isrA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB),isrB,CHANGE);
    reset();
}

long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}

void BlueMotor::motorTest(){
    int testNegative = false;
    int testCount = 0;
    delay(5000);
    if(testCount < 2){
        if(testNegative){
        Serial.println("negative");
        }else{
        Serial.println("positive");
        }
        for(int i = 0; i < 5; i++){
        reset();
        delay(500);
        Serial.print("test ");
        Serial.print(i + 1);
        Serial.println(" start");
        bool encoderMove = getPosition();
        int effort = 1; 
        while(effort < 401 && !encoderMove){
            if(testNegative){
            setEffort(-effort);
            }else{
            setEffort(effort);
            };
            delay(100);
            effort++;
            encoderMove = getPosition();
        }
        delay(100);
        motorBreak();
        Serial.println("moving effort: ");
        Serial.println(effort);
        Serial.println();
        delay(500);
        }
        testNegative = !testNegative;
        testCount ++;
    }else{
        Serial.println("test finished");
        while(true);
    }
}

void BlueMotor::motorPlot(){
    noInterrupts();
    int currentPosition = 0;
    int previousPosition = currentPosition;
    int RPM = 0;
    int actualEffort = 0;
    int time = 0;
    int startTime = millis();
    interrupts();
    for(int effort = 0; effort < motorEffort + 1; effort++){
        currentPosition = getPosition();
        actualEffort = setEffortWithoutDB(effort);
        RPM = (currentPosition - previousPosition) * 600 / CPR;
        noInterrupts();
        time = millis() - startTime;
        interrupts();
        previousPosition = currentPosition;
    }
}

void BlueMotor::isrA()
{
    if(digitalRead(ENCA) == digitalRead(ENCB)){
        count--;
    }else{
        count++;
    }
}

void BlueMotor::isrB()
{
    if(digitalRead(ENCA) == digitalRead(ENCB)){
        count++;
    }else{
        count--;
    }
}

void BlueMotor::setEffort(int effort)
{
    //Serial.println(effort);
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

int BlueMotor::setEffortWithoutDB(int effort){
    
    if(effort > 0){
        effort += minMotorEffort;
    }else if(effort < 0){
        effort -= minMotorEffort;
    }else if(abs(effort) < minMotorEffort){
        effort = 0;
    }
    setEffort(effort);
}

void BlueMotor::motorBreak()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
}

void BlueMotor::stayAT(long target)
{}

void BlueMotor::moveTo(int target)   
{ 
    bool keepMove = true;
    while(keepMove){
        keepMove = move(target);
    }
}

bool BlueMotor::move(int target){
    bool keepMove = abs(target - count) > tolerance;
    if(keepMove){
        float count = getPosition();
        Serial.print("count: ");
        Serial.println(count);
        float fix = getFix(count,target,Kp,Kd);
        setEffortWithoutDB(motorEffort * fix);
        Serial.print("output: ");
        Serial.println(motorEffort * fix);
        Serial.println();
    }else{
        motorBreak();
    }
    return keepMove;
}

float BlueMotor::getFix(float count,float target,float Kp,float Kd)
{
    error = target - count;
    Serial.print("error: ");
    Serial.println(error);
    float Pout = error / (float)CPR * Kp;
    Serial.print("Pout: ");
    Serial.print(error / CPR);
    Serial.print(" ");
    Serial.println(Pout);
    Serial.println("pError: ");
    Serial.println(pError);
    float Dout = (error - pError) / (float)CPR * Kd;
    Serial.print("Dout");
    Serial.println(error - pError);
    Serial.println(Dout);
    float fixValue = Pout + Dout;
    Serial.print("fix value: ");
    Serial.println(fixValue);
    pError = error;
    //delay(100);
    return fixValue;
}

void BlueMotor:: toStartPosition(int effort){
    int positiveCount = 0;
    while(positiveCount < 10){
        Serial.print("zeroing: ");
        setEffortWithoutDB(-effort);
        Serial.print(digitalRead(sensorPin));
        Serial.print(" ");
        if(digitalRead(sensorPin)){
            positiveCount ++;
        }else{
            positiveCount = 0;
        }
        Serial.println(positiveCount);
        delay(10);
    }
    motorBreak();
    reset();
}

void BlueMotor:: toStartPosition(){
    setEffort(minMotorEffort);
    Serial.println("function called");
    delay(100);
    int previousCount = 0;
    int currentCount = getPosition();
    Serial.print("previous: ");
    Serial.println(previousCount);
    Serial.print("now: ");
    Serial.println(currentCount);
    Serial.println("start");
    do{
        previousCount = currentCount;
        delay(100);
        currentCount = getPosition();
    }while(currentCount != previousCount);
    motorBreak();
    reset();
    moveTo(-liftMax + liftStart);
    reset();
    motorBreak();
    Serial.println("done");
}