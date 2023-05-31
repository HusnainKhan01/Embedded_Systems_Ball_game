#include "mpu6050.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

/**
 * Accelerometer data for x, y and z axis
 */
uint8_t AcceleroX[2];
uint8_t AcceleroY[2];
uint8_t AcceleroZ[2];

/**
 * accelerometer data for the hterm,
 */
char buffX[16];
char buffY[16];
char buffZ[16];

/**
 * Copy angle theta X and Y
 * used to print on Hterm for debuggine purpose
 */
char buffTiltX[16];
char buffTiltY[16];

/**
 * Initialization of Mpu sensor is done, Error handling, Accelerometer
 * configuration, return 0 if there is an error else return 1
 */
int mpu6050_init() {
	HAL_StatusTypeDef handleReturn;
	// PWR_MGMT_1
	handleReturn = HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, PWR_MAGT_1_REG, 1,
			0x00, 1,
			HAL_MAX_DELAY);
	if (handleReturn != HAL_OK) {
		return 0;
	}

	// ACCEL_CONFIG
	handleReturn = HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, ACCEL_CONFIG_REG, 1,
			0x00, 1,
			HAL_MAX_DELAY);
	if (handleReturn != HAL_OK) {
		return 0;
	}
	return 1;
}

/**
 * Check if working or not
 * return 1 for working else 0
 * for true it reads who am i register
 * who am i register from the mpu sensor is read and the
 * expected value is compared if expected value is same
 * then 1 returned(sensor is working) else 0
 */
int isWorking() {
	uint8_t whoReg;
	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, REG_WHO_AM, 1, &whoReg, 1,
			HAL_MAX_DELAY);
	if (whoReg == 0x68) {
		return 1;
	}
	return 0;
}

/**
 * read data
 * return 0 if fails,
 * error handling if return is not HAL_OK
 * value is to be checked before it goes to asin because asin function should not
 * take value more the 1 or less then -1 otherwise nun is returned by this function
 * Data is read from the sensor using I2C after data is read from the sensor
 * it is stored in array ACCEL_XOUT for x-axis ACCEL_YOUT for y axis and
 * ACCEL_ZOUT for z-axis, then all the data is for every single axis is combined to
 * 16 bit variables totalX, totalY and totalZ are the variables used for that
 *
 */
int readData() {
	HAL_StatusTypeDef handleReturn;

	handleReturn = HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, ACCEL_XOUT, 1,
			AcceleroX, 2,
			HAL_MAX_DELAY);
	if (handleReturn != HAL_OK) {
		return 0;
	}

	handleReturn = HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, ACCEL_YOUT, 1,
			AcceleroY, 2,
			HAL_MAX_DELAY);
	if (handleReturn != HAL_OK) {
		return 0;
	}

	handleReturn = HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, ACCEL_ZOUT, 1,
			AcceleroZ, 2,
			HAL_MAX_DELAY);
	if (handleReturn != HAL_OK) {
		return 0;
	}

	// copy the accelerometer data to 16bit variable
	int16_t totalX = ((uint16_t) AcceleroX[0] << 8) | (uint16_t) AcceleroX[1];
	int16_t totalY = ((uint16_t) AcceleroY[0] << 8) | (uint16_t) AcceleroY[1];
	int16_t totalZ = ((uint16_t) AcceleroZ[0] << 8) | (uint16_t) AcceleroZ[1];

	//totalX = (float) totalX/16384.0;
	// to float type 
	float totalX2 = totalX / 16.384;
	float totalY2 = totalY / 16.384;
	float totalZ2 = totalZ / 16.384;

	// for printing the data copy it to two decimal place in char type array
	sprintf(buffX, "%.2f", totalX2);
	sprintf(buffY, "%.2f", totalY2);
	sprintf(buffZ, "%.2f", totalZ2);

	if (totalX2 > 1000) {
		totalX2 = 1000;
	} else if (totalX2 < -1000) {
		totalX2 = -1000;
	}

	if (totalY2 > 1000) {
		totalY2 = 1000;
	} else if (totalY2 < -1000) {
		totalY2 = -1000;
	}

	if (totalZ2 > 1000) {
		totalZ2 = 1000;
	} else if (totalZ2 < -1000) {
		totalZ2 = -1000;
	}
	
	// 180 / pi
	// calculate angles thetaX and thetaY, converting into degree from radians
	allAngles.thetaX = asin(totalX2 / 1000) / 2 / 3.141592654 * 360;
	allAngles.thetaX = allAngles.thetaX * 10;
	allAngles.thetaY = asin(totalY2 / 1000) / 2 / 3.141592654 * 360;
	allAngles.thetaY = allAngles.thetaY * 10;

	// copy Theta
	sprintf(buffTiltX, "%.2f", allAngles.thetaX);
	sprintf(buffTiltY, "%.2f", allAngles.thetaY);

	return 1;
}

/**
 * Print mpu results on the hterm
 */
int printResult() {
	HAL_UART_Transmit(&huart2, "AX:  ", 5, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffX, strlen(buffX), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, "   ", 3, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart2, "AY:  ", 5, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffY, strlen(buffY), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, "   ", 3, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart2, "AZ:  ", 5, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffZ, strlen(buffZ), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, "   ;", 3, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart2, "  TX : ", 7, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffTiltX, strlen(buffTiltX), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, "  TY : ", 7, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffTiltY, strlen(buffTiltY), HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart2, "\n", 1, HAL_MAX_DELAY);

	return 1;
}

