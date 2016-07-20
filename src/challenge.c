#pragma config(Sensor, port4,           gyro,          sensorVexIQ_Gyro)
#pragma config(Sensor, port9,           colorSensor,   sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port12,          limitSwitchSensor,  sensorVexIQ_Touch)
#pragma config(Motor,  motor1,          leftMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor,  motor6,          rightMotor,    tmotorVexIQ, openLoop, reversed, encoder)
#pragma config(Motor, motor10,          armMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11,          clawMotor,     tmotorVexIQ, openLoop, encoder)

const long LINE_THRESHOLD = 180;
const int DRIVE_SPEED = 50;
const int ARM_SPEED = 50;
const int CLAW_SPEED = 50;

/// calculate error for a proportional controller
float proportionalOutput(float kP, float target, float input){
    return kP * (target - input);
}

/// drive the robot forward using motor encoders
void driveWithEncoders(int degrees){
    resetMotorEncoder(leftMotor);
    resetMotorEncoder(rightMotor);
    setMotorTarget(leftMotor, degrees, DRIVE_SPEED);
    setMotorTarget(rightMotor, degrees, DRIVE_SPEED);
    waitUntilMotorStop(leftMotor);
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
    sleep(200);
}

/// drive a certain distance in inches
void driveInches(float inches, int driveSpeed) {
    resetMotorEncoder(leftMotor);
    resetMotorEncoder(rightMotor);
    setMotorTarget(leftMotor, inches/7.8, driveSpeed);
    setMotorTarget(rightMotor, inches/7.8, driveSpeed);
    waitUntilMotorStop(leftMotor);
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
    sleep(200);
}

/// drive straight using the gyro
void gyroDriveInches(float inches, int driveSpeed) {
    inches = inches-2;
    float kP = 2.5;
    bool onTarget = false;
    resetMotorEncoder(leftMotor);
    resetMotorEncoder(rightMotor);
    resetGyro(gyro);

    while(!onTarget) {

        if(getMotorEncoder(leftMotor) < inches/7.8) {
            setMotorSpeed(leftMotor, driveSpeed + proportionalOutput(kP, 0, getGyroDegreesFloat(gyro)));
        }else {
            onTarget = true;
        }

        if(getMotorEncoder(rightMotor) < inches/7.8) {
            setMotorSpeed(rightMotor, driveSpeed - proportionalOutput(kP, 0, getGyroDegreesFloat(gyro)));
        }
    }
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
    sleep(200);
}

/// rotate with counterclockwise (left) as positive
void rotate(int degrees, bool reset){
    if(reset)
        resetGyro(gyro);

    float kP = 1;

    if(degrees > 0){
        // counterclockwise turn
        while(getGyroHeading(gyro) < degrees){
            setMotorSpeed(leftMotor, -proportionalOutput(kP, degrees, getGyroDegreesFloat(gyro)));
            setMotorSpeed(rightMotor, proportionalOutput(kP, degrees, getGyroDegreesFloat(gyro)));
        }
    } else {
        while(getGyroHeading(gyro) > degrees){
            setMotorSpeed(leftMotor, -proportionalOutput(kP, degrees, getGyroDegreesFloat(gyro)));
            setMotorSpeed(rightMotor, proportionalOutput(kP, degrees, getGyroDegreesFloat(gyro)));
        }
    }

    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
    sleep(200);
}

/// returns whether or not robot is pressed up against something
bool limitSwitchPressed(){
    return getBumperValue(limitSwitchSensor);
}

/// move arm back to bottom
void homeArm() {
    while(!limitSwitchPressed()){
        setMotorSpeed(armMotor, -ARM_SPEED);
    }
    setMotorSpeed(armMotor, 0);
    resetMotorEncoder(armMotor);
}

/// close or open claw
void clawClose(bool close) {
    setMotorSpeed(clawMotor, (close ? 1 * CLAW_SPEED : -1) * CLAW_SPEED);
    sleep(200);
    setMotorSpeed(clawMotor, 0);
}

/// move arm to certain degrees
void setArmDegrees(float degrees) {
    resetMotorEncoder(armMotor);
    setMotorTarget(armMotor, degrees/360, ARM_SPEED);
    waitUntilMotorStop(armMotor);
}

/// called when program starts
task main()
{
    setMotorEncoderUnits(encoderRotations);

    setMotorSpeed(armMotor, 0);// don't let arm move around

    // drive to object
    driveInches(13.0, 50);
    rotate(-90, true);
    driveInches(12.0, 50);
    rotate(90, true);
    driveInches(11.0, 50);
    rotate(90, true);
    driveInches(12.0, 50);
    rotate(-90, true);
    driveInches(12.0, 50);
    rotate(-90, true);
    driveInches(24.0, 50);
    rotate(-90, true);
    driveInches(24.0, 50);
    rotate(90, true);
    driveInches(12.0, 50);
    rotate(90, true);

    // keep going until we hit the object
    while(!limitSwitchPressed()) {
        setMotorSpeed(leftMotor, DRIVE_SPEED);
        setMotorSpeed(rightMotor, DRIVE_SPEED);
    }

    // back up
    driveInches(-4.0, 50);

    // pick up object
    clawClose(false);
    homeArm();
    driveInches(1, 50);
    clawClose(true);
    driveInches(-4, 50);
    setArmDegrees(400);

    // get to line
    while(getColorGrayscale(colorSensor) > LINE_THRESHOLD){
        setMotorSpeed(leftMotor, DRIVE_SPEED);
    }

    while(!limitSwitchPressed()) {
        // follow line
        displayString(1, "%d", getColorGrayscale(colorSensor));

        if(getColorGrayscale(colorSensor) > LINE_THRESHOLD) {
            setMotorSpeed(leftMotor, 50);
            setMotorSpeed(rightMotor, 0);
        }else {
            setMotorSpeed(leftMotor, 0);
            setMotorSpeed(rightMotor, 50);
        }
    }

    // back up and put down object
    driveInches(-3.5, 50);
    homeArm();
    clawClose(false);
    setArmDegrees(400);
}
