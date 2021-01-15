#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Motor BL (BLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor BR (BRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor lRoller(lRollerPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor rRoller(rRollerPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor bouncer(bouncerPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	Motor ejector(ejectorPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor shooter(shooterPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
	Motor froller(frollerPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
	// ADIEncoder encoderL (encdL_port,encdL_port+1,false);
	// ADIEncoder encoderR (encdR_port,encdR_port+1,false);
	ADIAnalogIn color(colorPort);
	ADIDigitalIn limit(limitPort);
	Controller master(E_CONTROLLER_MASTER);
	BL.tare_position();
	BR.tare_position();
	master.clear();
	// encoderL.reset();
	// encoderR.reset();
		/** declaration and initialization of asynchronous Tasks */
	// Task baseOdometryTask(baseOdometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
	// Task baseControlTask(baseControl, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
	// Task baseMotorControlTask(baseMotorControl, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
	Task objectOdometryTask(objectOdometry, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
	// Task visionBaseControlTask(visionBaseControl, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
	// Task controllerPrinterTask(controllerPrinter, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	ADIAnalogIn color(colorPort);
	delay(200);
	// while(true){
	// 	printf("auton: color: %d\n", color.get_value());
	// 	delay(10);
	// }
	int autonNum = 2;
	switch(autonNum){
		case 1: blueLeft(); break;
		case 2: blueRight(); break;
		case 3: redLeft(); break;
		case 4: redRight(); break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	int BRAKE_POW = 0;
	int OPCONTROL_RAMPING_POW = 8; // 2 per 5ms -> 40 per 100ms
	int targLeft = 0, targRight = 0, left = 0, right = 0;
	int count = 0;
	Motor BL (BLPort);
	Motor BR (BRPort);
	Motor lRoller(lRollerPort);
	Motor rRoller(rRollerPort);
	Motor bouncer(bouncerPort);
	Motor ejector(ejectorPort);
	Motor shooter(shooterPort);
	Motor froller(frollerPort);
	ADIAnalogIn color(colorPort);
	Controller master(E_CONTROLLER_MASTER);
	Vision visFlag(visFlagport);
	Vision visBall(visBallport);
	// ADIEncoder encoderL (encdL_port,encdL_port+1);
	// ADIEncoder encoderR (encdR_port,encdR_port+1);
	bool tankDrive = true;
	bool xMode = false;
	bool xModeisTeamRed = true;
	bool xModeisAlliance;
	int wThreshold = 250;

	while (true) {
		if(master.get_digital_new_press(DIGITAL_Y)) tankDrive = !tankDrive;
		if(tankDrive){
      targLeft = master.get_analog(ANALOG_LEFT_Y) + BRAKE_POW;
      targRight = master.get_analog(ANALOG_RIGHT_Y) + BRAKE_POW;
    } else{
      int y = master.get_analog(ANALOG_LEFT_Y);
      int x = master.get_analog(ANALOG_RIGHT_X);
      targLeft = y+x+BRAKE_POW;
      targRight = y-x+BRAKE_POW;
    }
		left += abscap(targLeft - left, OPCONTROL_RAMPING_POW);
		right += abscap(targRight - right, OPCONTROL_RAMPING_POW);
		BL.move(left);
		BR.move(right);
		if(master.get_digital_new_press(DIGITAL_X)){
			xMode = !xMode;
		}
		if(master.get_digital_new_press(DIGITAL_B)){
			xModeisTeamRed = !xModeisTeamRed;
		}
		if(count % 20 == 0){
			master.print(1, 0, "%s XMODE: %s", xModeisTeamRed ? "RED " : "BLUE", xMode ? "ON " : "OFF");
		}


		if(redBall.curr.w >= wThreshold || blueBall.curr.w >= wThreshold){
			// valid change
			if(xModeisTeamRed){
				xModeisAlliance = (redBall.curr.w > blueBall.curr.w);
			}else{
				xModeisAlliance = (blueBall.curr.w > redBall.curr.w);
			}
		}
			if(master.get_digital(DIGITAL_R1)){
				if(master.get_digital(DIGITAL_L1)){
					// intake with shooting
					if(xMode){
						if(xModeisAlliance){
							allMove(127, 127, 127, 127, 127);
						}else{
							allMove(127,127,-127,0,0);
						}
					}else allMove(127, 127, 127, 127, 127);
				}else{
					if(xMode){
						if(xModeisAlliance){
							allMove(127, 127, 127, 0, 0);
						}else{
							allMove(127,127,-127,0,0);
						}
					}else{
						allMove(127, 127, 127, 0, 0);
					}
					// intake without shooting
				}
			}else if(master.get_digital(DIGITAL_L1)){
				// shooting without intaking
				allMove(0, 127, 127, 127, 127);
			}else if(master.get_digital(DIGITAL_L2)){
				if(master.get_digital(DIGITAL_R2)){
					// back eject from intake
					allMove(127,127,-127,0,0);
				}else{
					// back eject from storage
					allMove(0, 127, -127, -127, 127);
				}
			}else if(master.get_digital(DIGITAL_R2)){
				// front eject
				allMove(-127,-127,-127,-127, -127);
			}else{
				// stop bro
				allMove(0,0,0,0,0);
			}
		}
		pros::delay(5);
}
