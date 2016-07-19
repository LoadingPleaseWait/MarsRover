#pragma config(Motor, motor10,          armMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11,          clawMotor,    tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port12,          limitSwitchSensor,  sensorVexIQ_Touch)

const int ARM_SPEED = 50;

/// bring arm down to limit switch
void bringArmDown(){
	while(!getBumperValue(limitSwitchSensor)){
		setMotorSpeed(armMotor, -ARM_SPEED);
	}
	setMotorSpeed(armMotor, 0);
}

/// begin program
task main(){
	// pick up and object and then put it back down
	bringArmDown();
	// close claw
	resetMotorEncoder(clawMotor);
	setServoTarget(clawMotor, 200);
	sleep(1000);
	// bring arm up
	resetMotorEncoder(armMotor);
	setMotorTarget(armMotor, 700, ARM_SPEED);
	waitUntilMotorStop(armMotor);
	bringArmDown();
}
