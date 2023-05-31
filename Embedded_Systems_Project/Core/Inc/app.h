/*
 *
 * App module has two functions. One of the functions (app_init) is used for the initialization of all
 * the modules used in this project like MPU6050 sensor, Dem128064B LCD Module at first then the
 * registration certain functions take place inside using Timer module. Only those functions are
 * registered inside the timer module those are called after a certain period. A welcome message is
 * also a part of initialization which is shown to the user before game is started.
 * app.h
 *
 *  Created on: Nov 9, 2021
 *      Author: Husnain Khan
 */

#ifndef INC_APP_H_
#define INC_APP_H_

/**
 * Sensor refresh rate
 */
#define SENSOR_REFRESH_RATE	50

/**
 * Ball movement rate
 */
#define BALL_MOVEMENT_RATE 20

/**
 * Screen refresh
 */
#define REFRESH_RATE 50

/**
 * Upper line movement rate
 */
#define MOVE_LINE_UPPER_RATE 30

/**
 * Lower line movement rate
 */
#define MOVE_LINE_LOWER_RATE 40

/**
 * Delay for the welcome message
 */
#define WELCOME_MESSAGE_DELAY 2000

/**
 * Delay for the game over
 */
#define GAME_OVER_DELAY 2000

/**
 * Starting row of upper line
 */
#define START_OF_UPPER_LINE 0

/**
 * Length of upper line
 */
#define LENGTH_UPPER_LINE 30

/**
 * Starting of lower line
 */
#define START_OF_LOWER_LINE 30

/**
 * Length of lower line
 */
#define LENGTH_OF_LOWER_LINE 33

/**
 * starting column of moving lines
 */
#define START_LINE_COL_VALUE 127

/**
 * Column shift value for unit
 */
#define UNIT_COL_SHIFT_VAL 60

/**
 * Column shift value for Ten
 */
#define TEN_COL_SHIFT_VAL 30

/**
 * MAX points user can score in the game,
 * if points are greater then max score then points
 * will not be shown on the display
 */
#define MAX_SCORE 100

/**
 * Used to keep track if the game is Ended or not
 * 0 if game is running, 1 if game is ended
 */
extern volatile int gameEnd = 0;

/**
 * initilization
 */
extern void app_init(void);

/**
 * loop function, called inside the main loop,
 * game End is to be checked here if the game
 * is ended then a message is to be shown to the user
 * for a period of time
 */
extern void app_loop(void);

/**
 * used to get the mpu data,
 * the new data is calculated and is to be set
 * in the tilt angles inside the mpu module
 */
extern int getMpuData(void);

/**
 * refresh the screen,
 * the pixels stored locally in a 2d array
 * are to be reflected to the screen
 */
extern void refresh(void);

/**
 * Used to move the line,
 * for the upper line to be moved on the screen
 */
extern void moveLine(void);

/**
 * Used to move the line slowly,
 * lower line which moves slower then the upper line
 */
extern void slowMoveLine(void);

/**
 * for the ball movement,
 * calculating the new position of the ball
 * from the tilt angle
 */
extern void ballMovementWithSpeed(void);

#endif /* INC_APP_H_ */
