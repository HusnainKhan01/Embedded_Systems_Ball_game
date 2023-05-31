#include "app.h"
#include "main.h"
#include "timer.h"
#include "mpu6050.h"
#include "Dem128064B.h"

extern UART_HandleTypeDef huart2;

/**
 * Line Starting positions
 */
int lineCol = 127;
int lineColSlowMove = 127;

/**
 * Player score,
 * score is counted every single time line crosses
 * from the left side
 */
int score = 0;

/**
 * initialization of the MPU Module, check if initialization is successful,
 * check if it is working,
 * setup the LCD, when everything is fine write Welcome message on the screen
 * Register the functions those are required to be 
 * called periodically in timer_register
 */
void app_init(void) {
	// Init MPU and error handling
	if (mpu6050_init() != 1) {
		HAL_UART_Transmit(&huart2, "Init Fails ...", 14, HAL_MAX_DELAY);
	}
	if (isWorking() != 1) {
		HAL_UART_Transmit(&huart2, "NOT WORKING ...", 15, HAL_MAX_DELAY);
	}

	// setup Lcd
	setUp();

	// write welcome message when game is started...
	writeWelcomeToArray();
	refresh();
	HAL_Delay(WELCOME_MESSAGE_DELAY);
	init_DisplayArray();
	HAL_Delay(100);
	refresh();

	// registring the functions to be called periodically
	timer_register(getMpuData, SENSOR_REFRESH_RATE);
	timer_register(ballMovementWithSpeed, BALL_MOVEMENT_RATE);
	timer_register(refresh, REFRESH_RATE);
	timer_register(moveLine, MOVE_LINE_UPPER_RATE);
	timer_register(slowMoveLine, MOVE_LINE_LOWER_RATE);
}

/**
 * game is ended when gameEnd flag is set to 1, 
 * when game is ended game over message is written
 * and calculated score, ball is set to starting position,
 */
void app_loop(void) {
	if (gameEnd == 1) {
		// clear the screen 
		init_DisplayArray();

		// refresh the screen, set the content of the 2d array to the LCD
		refresh();

		// set the ball position to the starting position
		prevPos.prev_Col = 50;
		prevPos.prev_Row = 50;

		// set the line to the default value 
		lineCol = START_LINE_COL_VALUE;
		lineColSlowMove = START_LINE_COL_VALUE;

		// set game over message
		writeGameOver();
		writeScore(score);
		score = 0;
		HAL_Delay(GAME_OVER_DELAY);
		init_DisplayArray();
		refresh();

		// Reset game End flag, so game can be played again
		gameEnd = 0;
	}
}

/**
 * Used to write the Score on the display array
 * Write score if score is less then 100
 */
void writeScore(int score) {
	if (score < MAX_SCORE) {
		int unit = score % 10;
		score = score / 10;
		int ten = (score) % 10;
		writeNumber(unit, UNIT_COL_SHIFT_VAL);
		writeNumber(ten, TEN_COL_SHIFT_VAL);
	}
}

/**
 * Used to get the data from the MPU6050 Sensor, also do error handling
 * print the result for debugging, also check if there some kind of reading error
 * if readData()  returns 0 then it is reading Error which is to be transmitted via UART
 * if not then the result is printed for debugging 
 */
int getMpuData() {
	if (readData() != 1) {
		HAL_UART_Transmit(&huart2, "Reading Error ...", 17, HAL_MAX_DELAY);
	} else {
		printResult();
	}
}

/**
 * refresh screen,
 * set the content on the Lcd 
 */
void refresh() {
	refreshScreen();
}

/**
 * For collision detection
 * 0 for lower line, else do collision detection for upper line 
 * when collision is detected gameEnd flag is also set 
 */
void collision(int forLower) {
	int getBallStatus = 0;
	if (forLower == 0) {
		getBallStatus = ballIsOnLowerLine(lineColSlowMove);
	} else {
		getBallStatus = ballIsOnLine(lineCol);
	}

	if (getBallStatus == 0) {
		HAL_UART_Transmit(&huart2, "BAL IS ON LINE ..", 17, HAL_MAX_DELAY);
		gameEnd = 1;
	}
}

/**
 * Ball movement in all direction
 * calculating the new position of the ball and setting it
 * 630 is total rows multiple of 10, 1270 total column multiple of 10
 * used to compare with multiple of 10 because the position of the ball
 * is stored as multiple of 10
 */
void ballMovementWithSpeed() {
	int newRow = prevPos.prev_Row + allAngles.thetaX / 90;
	int newCol = prevPos.prev_Col + allAngles.thetaY / 90;

	if (newRow < 630 && newCol < 1270 && newRow >= 0 && newCol >= 0) {
		if (gameEnd == 0) {
			set_Ball_To_Position(prevPos.prev_Row / 10, prevPos.prev_Col / 10, 0);
			set_Ball_To_Position(newRow / 10, newCol / 10, 1);
			prevPos.prev_Row = newRow;
			prevPos.prev_Col = newCol;

			collision(0);
			collision(1);
		}
	}
}

/**
 * The upper line which moves from the right side of the screen 
 * towards the left side, 
 * this line moves faster then the lower line, 
 * speed of the line is not adjusted here, 
 * speed of the line is adjusted when this function is registered in the timeRegister
 */
void moveLine() {
	if (gameEnd == 0) {
		draw_Vert_Line(START_OF_UPPER_LINE, lineCol, LENGTH_UPPER_LINE, 1);
		draw_Vert_Line(START_OF_UPPER_LINE, lineCol + 1, LENGTH_UPPER_LINE, 0);
		if (lineCol < 0) {
			lineCol = START_LINE_COL_VALUE;
			score++;
		} else {
			lineCol--;
		}
		collision(1);
	}
}

/**
 * The lower line, moves from the right side of the screen
 * towards the left side and will appear again from the 
 * right side when it reaches to left side of the screen
 * The speed of the line is not adjusted here 
 */
void slowMoveLine() {
	if (gameEnd == 0) {
		draw_Vert_Line(START_OF_LOWER_LINE, lineColSlowMove,
				LENGTH_OF_LOWER_LINE, 1);
		draw_Vert_Line(START_OF_LOWER_LINE, lineColSlowMove + 1,
				LENGTH_OF_LOWER_LINE, 0);
		if (lineColSlowMove < 0) {
			lineColSlowMove = START_LINE_COL_VALUE;
			score++;
		} else {
			lineColSlowMove--;
		}
		collision(0);
	}
}


