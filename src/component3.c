#pragma config(Motor,  motor1,          leftMotor,          tmotorVexIQ, openLoop, encoder)
#pragma config(Motor,  motor6,          rightMotor,         tmotorVexIQ, openLoop, reversed, encoder)
#pragma config(Sensor, port4,           gyro,               sensorVexIQ_Gyro)

const int MOTOR_SPEED = 50;

/// called when program begins
task main(){
	// Keep in mind that VEX IQ gyros are counterclockwise positive!
	resetGyro(gyro);
	while(getGyroHeading(gyro) < 90){
		// turn counterclockwise 90 degrees
		setMotorSpeed(rightMotor, MOTOR_SPEED);
	}
	setMotorSpeed(rightMotor, 0);

	// make a turn 45 degrees to the right
	resetGyro(gyro);
	while(getGyroHeading(gyro) > -45){
		setMotorSpeed(leftMotor, MOTOR_SPEED);
	}
	setMotorSpeed(leftMotor, 0);
}
