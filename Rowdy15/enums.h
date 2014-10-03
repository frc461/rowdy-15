#ifndef ENUMS_H
#define ENUMS_H

enum RobotDriveTrainPorts {
	dt_pwm_rear_right = 1,
	dt_pwm_rear_left = 2,
	dt_pwm_front_right = 3,
	dt_pwm_front_left = 4
};

enum RobotWingPorts {
	w_pwm_roller_left = 5,
	w_pwm_roller_right = 7,
	w_pwm_wing_left = 9,
	w_pwm_wing_right = 10
};

enum RobotPneumaticsPorts {
	p_sol_shooter_a = 7,
	p_sol_shooter_b = 8,
	p_rel_compressor = 8
};

enum RobotJoystickPorts {
	j_1 = 1,
	j_2 = 2,
	j_3 = 3,
	j_4 = 4,
	j_left = j_1,
	j_right = j_2
};

enum Attack3Buttons {
	atk3_btn_1 = 0x1,
	atk3_btn_2 = 0x2,
	atk3_btn_3 = 0x3,
	atk3_btn_4 = 0x4,
	atk3_btn_5 = 0x5,
	atk3_btn_6 = 0x6,
	atk3_btn_7 = 0x7,
	atk3_btn_8 = 0x8,
	atk3_btn_9 = 0x9,
	atk3_btn_10 = 0xa,
	atk3_btn_11 = 0xb,
	atk3_btn_12 = 0xc,
	atk3_btn_trigger = atk3_btn_1,
	atk3_btn_thumb_back = atk3_btn_2,
	atk3_btn_thumb_front = atk3_btn_3,
	atk3_btn_thumb_left = atk3_btn_4,
	atk3_btn_thumb_right = atk3_btn_5
};

enum ea_pretty_name {
		both_wings_down = 0x0,
		both_wings_up = 0x1,
		left_wing_up = 0x2,
		left_wing_down = 0x3,
		fire_button = 0x4,
		lower_power_button = 0x5,
		right_wing_up = 0x6,
		right_wing_down = 0x7,
		left_roller_in = 0x8,
		left_roller_out = 0x9,
		both_roller_out = 0xa,
		both_roller_in = 0xb,
		right_roller_in = 0xc,
		right_roller_out = 0xd
};

#endif
