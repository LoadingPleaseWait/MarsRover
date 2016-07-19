#pragma config(Motor,  motor1,          leftMotor,          tmotorVexIQ, openLoop, encoder)
#pragma config(Motor,  motor6,          rightMotor,         tmotorVexIQ, openLoop, reversed, encoder)
#pragma config(Sensor, port12,          limitSwitchSensor,  sensorVexIQ_Touch)

/// task that gets called right when the program is run
task main(){
	// drive until robot runs into an object
	while(!getBumperValue(limitSwitchSensor)){
		setMotorSpeed(leftMotor, 50);
		setMotorSpeed(rightMotor, 50);
	}
}
