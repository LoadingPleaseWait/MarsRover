#pragma config(Motor, motor10,          armMotor,     tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11,          clawMotor,    tmotorVexIQ, openLoop, encoder)

const int CLAW_SPEED = 50;
const int ARM_SPEED = 50;

/// begin program
task main(){
	// close claw
	resetMotorEncoder(clawMotor);
	setServoTarget(clawMotor, 200);
	sleep(1000);
	// bring arm up
	resetMotorEncoder(armMotor);
	setMotorTarget(armMotor, 700, ARM_SPEED);
	waitUntilMotorStop(armMotor);
}
