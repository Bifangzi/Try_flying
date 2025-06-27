#include "mpu6050.h"

#include "i2c.h"
#include "usart.h"
#include "math.h"

static double gyro_x_offset = 0.0, gyro_y_offset = 0.0, gyro_z_offset = 0.0;


void MPU6050_Init(void)
{
    uint8_t send_data;
    uint8_t receive_data;

    // Check if the device is connected
    HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDRESS,MPU6050_WHO_AM_I,I2C_MEMADD_SIZE_8BIT,&receive_data,1,HAL_MAX_DELAY);
    if(receive_data != 0x68) // 0x68 is the default WHO_AM_I value for MPU6050
    {
        // Handle error: device not found
        printf("MPU6050 not found, receive_data: %x\r\n", receive_data);
        return;
    }
    else
    {
        printf("MPU6050 found, receive_data: %x\r\n", receive_data);
        send_data = 0x80; // Reset all settings to default
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
        HAL_Delay(1000); // Wait for reset to complete
        
        // Configure the MPU6050
        send_data = 0x07; // Set clock source to PLL with X axis gyroscope reference
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
        HAL_Delay(10); // Wait for the clock source to stabilize
        send_data = 0x00; // Disable sleep mode
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &receive_data, 1, HAL_MAX_DELAY);
        if (receive_data & 0x40)
        {
            // Handle error: device did not wake up from sleep mode
            printf("MPU6050 wake up failed, receive_data: %x\r\n", receive_data);
            return;
        }
        else
        {
            printf("MPU6050 wake up successed\r\n");
            
            // Set accelerometer configuration
            send_data = 0x00; // Full scale range ±2g
            HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
            
            // Set gyroscope configuration
            send_data = 0x18; // Full scale range ±2000 degrees/s
            HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
            
            // Set sample rate divider
            send_data = 0x00; // Sample rate = 1000Hz / (1 + SMPLRT_DIV) = 1000Hz / 1 = 1000Hz
            HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
            
            // Disable interrupts
            send_data = 0x00; // Disable data ready interrupt
            HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_INT_ENABLE, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);

            send_data = 0x1A; // Set DLPF to 20Hz
            HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS, MPU6050_CONFIG, I2C_MEMADD_SIZE_8BIT, &send_data, 1, HAL_MAX_DELAY);
            
            printf("MPU6050 initialized successfully\r\n");
        }

    }
    
}



void MPU6050_Read_Accel(double *accel_x, double *accel_y, double *accel_z)
{
    uint8_t receive_data[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, receive_data, 6, HAL_MAX_DELAY);
    
    int16_t accel_xraw = (int16_t)((receive_data[0] << 8) | receive_data[1]);
    int16_t accel_yraw = (int16_t)((receive_data[2] << 8) | receive_data[3]);
    int16_t accel_zraw = (int16_t)((receive_data[4] << 8) | receive_data[5]);

    // Convert raw data to g's
    *accel_x = (double)accel_xraw / 16383.75; // ±2g sensitivity
    *accel_y = (double)accel_yraw / 16383.75; // ±2g sensitivity
    *accel_z = (double)accel_zraw / 16383.75; // ±2g sensitivity

}

void MPU6050_Read_Gyro(double *gyro_x, double *gyro_y, double *gyro_z)
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);
    
    int16_t gyro_xraw = (int16_t)((data[0] << 8) | data[1]);
    int16_t gyro_yraw = (int16_t)((data[2] << 8) | data[3]);
    int16_t gyro_zraw = (int16_t)((data[4] << 8) | data[5]);
    // Convert raw data to degrees/s
    *gyro_x = (double)gyro_xraw / 16.4; // ±2000 degrees/s sensitivity
    *gyro_y = (double)gyro_yraw / 16.4; // ±2000 degrees/s sensitivity
    *gyro_z = (double)gyro_zraw / 16.4; // ±2000 degrees/s sensitivity

    *gyro_x -= gyro_x_offset;
    *gyro_y -= gyro_y_offset;
    *gyro_z -= gyro_z_offset;
}


void MPU6050_Gyro_Calibrate()
{
    double gyro_x_sum = 0.0, gyro_y_sum = 0.0, gyro_z_sum = 0.0;
    int samples = 1000; // Number of samples for calibration
    printf("The MPU6050 will be calibrated in 3 seconds, please keep it still for a while...\r\n");
    osDelay(3000); // Wait for 3 seconds before starting calibration
    printf("Starting gyro calibration...\r\n");

    for (int i = 0; i < samples; i++)
    {
        double gyro_x, gyro_y, gyro_z;
        MPU6050_Read_Gyro(&gyro_x, &gyro_y, &gyro_z);
        gyro_x_sum += gyro_x;
        gyro_y_sum += gyro_y;
        gyro_z_sum += gyro_z;
        HAL_Delay(10); // Delay between samples
    }

    gyro_x_offset = gyro_x_sum / samples;
    gyro_y_offset = gyro_y_sum / samples;
    gyro_z_offset = gyro_z_sum / samples;

    printf("Gyro calibration completed.\r\n");
    // printf("Gyro offsets: X=%.2f, Y=%.2f, Z=%.2f\r\n", *gyro_x_offset, *gyro_y_offset, *gyro_z_offset);
}




void MPU6050_Get_Euler_By_Accel(double *roll, double *pitch, double *yaw)
{
    double accel_x, accel_y, accel_z;

    MPU6050_Read_Accel(&accel_x, &accel_y, &accel_z);
    // Calculate roll and pitch from accelerometer data
    *roll = atan2(accel_y, sqrt(accel_x * accel_x + accel_z * accel_z)) * 180.0 / 3.1415927; // Roll in degrees
    *pitch = atan2(-accel_x, sqrt(accel_y * accel_y + accel_z * accel_z)) * 180.0 / 3.1415927; // Pitch in degrees
    *yaw = 0.0; // Yaw is not calculated from accelerometer data
    
}


void MPU6050_Get_Euler_By_Gyro(double *roll, double *pitch, double *yaw)
{
    double gyro_x, gyro_y, gyro_z;

    MPU6050_Read_Gyro(&gyro_x, &gyro_y, &gyro_z);

    // Calculate roll, pitch, and yaw from gyroscope data
    *roll = *roll + gyro_x * 0.01; // Assuming 100Hz sampling rate, adjust as needed
    *pitch = *pitch + gyro_y * 0.01; // Assuming 100Hz sampling rate, adjust as needed
    *yaw = *yaw + gyro_z * 0.01; // Assuming 100Hz sampling rate, adjust as needed
}


void MPU6050_Get_Euler_By_Accel_Gyro(double *roll, double *pitch, double *yaw)
{
    double roll_accel, pitch_accel, yaw_accel;
    double roll_gyro, pitch_gyro, yaw_gyro;

    MPU6050_Get_Euler_By_Accel(&roll_accel, &pitch_accel, &yaw_accel);
    MPU6050_Get_Euler_By_Gyro(&roll_gyro, &pitch_gyro, &yaw_gyro);

    // Calculate roll and pitch from accelerometer data
    *roll = 0.95238 * roll_gyro + (1-0.95238) * roll_accel; // Complementary filter
    *pitch = 0.95238 * pitch_gyro + (1-0.95238) * pitch_accel; // Complementary filter
    *yaw = yaw_gyro; // Yaw is only calculated from gyroscope data
}


void MPU6050_Read_Temperature(double *temperature)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS, MPU6050_TEMP_OUT_H, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    
    int16_t temp_raw = (data[0] << 8) | data[1];
    *temperature = (double)temp_raw / 340.0 + 36.53; // Convert to Celsius
}