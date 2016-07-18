#pragma config(Sensor, port4,           gyro,          sensorVexIQ_Gyro)
#pragma config(Sensor, port9,           colorSensor,   sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port12,          limitSwitchSensor,  sensorVexIQ_Touch)
#pragma config(Motor,  motor1,          leftMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor,  motor6,          rightMotor,    tmotorVexIQ, openLoop, reversed, encoder)
#pragma config(Motor, motor10,          armMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11,          clawMotor,     tmotorVexIQ, openLoop, encoder)

const long LINE_THRESHOLD = 180;
const int DRIVE_SPEED = 50;
const int ROTATION_SPEED = 50;
const int ARM_SPEED = 50;
const int CLAW_SPEED = 50;

bool limitSwitchPressed();

/// drive the robot forward using motor encoders
void driveWithEncoders(int degrees){
	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	setMotorTarget(leftMotor, degrees, DRIVE_SPEED);
	setMotorTarget(rightMotor, degrees, DRIVE_SPEED);
	waitUntilMotorStop(leftMotor);
}

/// rotate with counterclockwise as positive
void rotate(int degrees){
	resetGyro(gyro);
	if(degrees > 0){
		// counterclockwise turn
		while(getGyroHeading(gyro) < degrees){
			setMotorSpeed(leftMotor, -ROTATION_SPEED);
			setMotorSpeed(rightMotor, ROTATION_SPEED);
		}
	} else {
		while(getGyroHeading(gyro) > degrees){
			setMotorSpeed(leftMotor, ROTATION_SPEED);
			setMotorSpeed(rightMotor, -ROTATION_SPEED);
		}
	}
}

/** pick up and put down objects
 * @param pickingUp true if object is getting picked up false if being put down
 */
void useClaw(bool pickingUp){
	while(!limitSwitchPressed()){
		setMotorSpeed(armMotor, -ARM_SPEED);
	}
	setMotorSpeed(armMotor, 0);
	setMotorSpeed(clawMotor, (pickingUp ? 1 * CLAW_SPEED : -1) * CLAW_SPEED);
	sleep(500);
	setMotorSpeed(clawMotor, 0);
	if (pickingUp) {
		resetMotorEncoder(armMotor);
		setMotorTarget(armMotor, 700, ARM_SPEED);
		waitUntilMotorStop(armMotor);
	}
}

/// returns whether or not robot is pressed up against something
bool limitSwitchPressed(){
	return getBumperValue(limitSwitchSensor);
}

/// called when program starts
task main()
{
	setMotorSpeed(armMotor, 0);// don't let arm move around

	// drive to object
	driveWithEncoders(500);
	rotate(-90);

	// keep going until we hit the object
	while(!limitSwitchPressed()) {
		setMotorSpeed(leftMotor, DRIVE_SPEED);
		setMotorSpeed(rightMotor, DRIVE_SPEED);
	}

	// back up
	driveWithEncoders(-200);

	// pick up object
	useClaw(true);

	// rotate and back up until it finds the line
	rotate(-45);

	while(getColorGrayscale(colorSensor) > LINE_THRESHOLD){
		setMotorSpeed(leftMotor, -DRIVE_SPEED);
		setMotorSpeed(rightMotor, -DRIVE_SPEED);
	}

	while(!limitSwitchPressed()) {
		// follow line
		displayString(1, "%d", getColorGrayscale(colorSensor));

		if(getColorGrayscale(colorSensor) > LINE_THRESHOLD) {
			setMotorSpeed(leftMotor, 20);
			setMotorSpeed(rightMotor, 0);
		}else {
			setMotorSpeed(leftMotor, 0);
			setMotorSpeed(rightMotor, 20);
		}
	}

	// back up and put down object
	driveWithEncoders(-200);
	useClaw(false);
}
