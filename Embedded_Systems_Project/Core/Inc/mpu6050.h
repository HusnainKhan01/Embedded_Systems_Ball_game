/**
 * MPU6050 module is used for getting tilt angles from the sensor.
 * The data which is read from the
 * sensor is in the form of acceleration in three axis x, y and z
 * this acceleration is then converted to
 * tilt angle and is stored in a structure of angles having two angles thetaX and thetaY.
 *
 * Created on: Nov 26, 2021
 * Author: Husnain
 *
 */

/**
 * Who Am I register
 */
#define REG_WHO_AM	(0x75)

/**
 * PWR MAGT 1 Register
 */
#define PWR_MAGT_1_REG	0x6B

/**
 * Acceleration config Register
 */
#define ACCEL_CONFIG_REG 0x1C

/**
 * 7 bits address value in datasheet
 * shifted to the left
 */
#define MPU_ADDRESS (0xD0)

/**
 * Address to read accelerometer data
 */
#define ACCEL_XOUT 0x3B
#define ACCEL_YOUT 0x3D
#define ACCEL_ZOUT 0x3F

/**
 * Angles stored
 * angles are stored as multiple of 10
 * for example 300 for 30 degree
 */
struct angles {
	float thetaX;
	float thetaY;
} allAngles;

/**
 * Used to initialize MPU6050 to read accelerometer,
 * return 1 if it is successful else 0
 */
extern int mpu6050_init(void);

/**
 * Used to check if controller is working
 * return 1 if is working
 */
extern int isWorking(void);

/**
 * Used to read Data
 */
extern int readData(void);

/**
 * Used to print the Results
 */
extern int printResult(void);

