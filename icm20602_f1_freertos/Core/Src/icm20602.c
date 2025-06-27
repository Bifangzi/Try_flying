#include "icm20602.h"

#include "i2c.h"
#include "usart.h"

void ICM20602_Init(void) {
    // Initialize the ICM20602 sensor
    uint8_t send_data[1] = {0x80}; // Reset command
    uint8_t receive_data[1] = {0};

    // Reset the device
    HAL_I2C_Mem_Write(&hi2c1, ICM20602_ADDRESS, ICM20602_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, send_data, 1, HAL_MAX_DELAY);
    HAL_Delay(100); // Wait for reset to complete

    HAL_I2C_Mem_Read(&hi2c1, ICM20602_ADDRESS, ICM20602_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, receive_data, 1, HAL_MAX_DELAY);
    if(receive_data[0] != 0x41) {
        // Handle error: device did not reset correctly
        printf("ICM20602 reset failed,receive_data:%d\r\n",receive_data[0]);
        return;
    }
    /*
    // Set clock source to internal oscillator
    send_data = 0x01; // Internal oscillator
    HAL_I2C_Mem_Write(&hi2c1, ICM20602_ADDRESS, ICM20602_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);

    // Configure accelerometer and gyroscope settings
    send_data = 0x00; // Full scale range ±2g for accelerometer and ±250dps for gyroscope
    HAL_I2C_Mem_Write(&hi2c1, ICM20602_ADDRESS, ICM20602_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
    HAL_I2C_Mem_Write(&hi2c1, ICM20602_ADDRESS, ICM20602_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);

    // Set sample rate divider
    send_data = 0x07; // Sample rate of 100Hz
    HAL_I2C_Mem_Write(&hi2c1, ICM20602_ADDRESS, ICM20602_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
    */
}