#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "stdint.h"

#define MPU6050_ADDRESS 0xD0      // I2C address of the MPU6050 (shifted for HAL I2C)

#define MPU6050_SELF_TEST_X 0x0D    //自检寄存器X
#define MPU6050_SELF_TEST_Y 0x0E    //自检寄存器Y
#define MPU6050_SELF_TEST_Z 0x0F    //自检寄存器Z
#define MPU6050_SELF_TEST_A 0x10    //自检寄存器A
#define MPU6050_SMPLRT_DIV 0x19     //采样率分频寄存器
#define MPU6050_CONFIG 0x1A         //配置寄存器
#define MPU6050_GYRO_CONFIG 0x1B    //陀螺仪配置寄存器
#define MPU6050_ACCEL_CONFIG 0x1C   //加速度传感器配置寄存器
#define MPU6050_MOT_THR 0x1F        //运动检测阈值寄存器
#define MPU6050_FIFO_EN 0x23        // FIFO使能寄存器
#define MPU6050_I2C_MST_CTRL 0x24
#define MPU6050_I2C_SLV0_ADDR 0x25
#define MPU6050_I2C_SLV0_REG 0x26
#define MPU6050_I2C_SLV0_CTRL 0x27
#define MPU6050_I2C_SLV1_ADDR 0x28
#define MPU6050_I2C_SLV1_REG 0x29
#define MPU6050_I2C_SLV1_CTRL 0x2A
#define MPU6050_I2C_SLV2_ADDR 0x2B
#define MPU6050_I2C_SLV2_REG 0x2C
#define MPU6050_I2C_SLV2_CTRL 0x2D
#define MPU6050_I2C_SLV3_ADDR 0x2E
#define MPU6050_I2C_SLV3_REG 0x2F
#define MPU6050_I2C_SLV3_CTRL 0x30
#define MPU6050_I2C_SLV4_ADDR 0x31
#define MPU6050_I2C_SLV4_REG 0x32
#define MPU6050_I2C_SLV4_DO 0x33
#define MPU6050_I2C_SLV4_CTRL 0x34
#define MPU6050_I2C_SLV4_DI 0x35
#define MPU6050_I2C_MST_STATUS 0x36
#define MPU6050_INT_PIN_CFG 0x37
#define MPU6050_INT_ENABLE 0x38     //中断使能寄存器
#define MPU6050_INT_STATUS 0x3A
#define MPU6050_ACCEL_XOUT_H 0x3B   //加速度传感器X轴输出高字节
#define MPU6050_ACCEL_XOUT_L 0x3C   //加速度传感器X轴输出低字节
#define MPU6050_ACCEL_YOUT_H 0x3D   //加速度传感器Y轴输出高字节
#define MPU6050_ACCEL_YOUT_L 0x3E   //加速度传感器Y轴输出低字节
#define MPU6050_ACCEL_ZOUT_H 0x3F   //加速度传感器Z轴输出高字节

#define MPU6050_TEMP_OUT_H 0x41     //温度传感器输出高字节
#define MPU6050_TEMP_OUT_L 0x42     //温度传感器输出低字节
#define MPU6050_GYRO_XOUT_H 0x43    //陀螺仪X轴输出高字节
#define MPU6050_GYRO_XOUT_L 0x44    //陀螺仪X轴输出低字节
#define MPU6050_GYRO_YOUT_H 0x45    //陀螺仪Y轴输出高字节
#define MPU6050_GYRO_YOUT_L 0x46    //陀螺仪Y轴输出低字节
#define MPU6050_GYRO_ZOUT_H 0x47    //陀螺仪Z轴输出高字节        
#define MPU6050_GYRO_ZOUT_L 0x48    //陀螺仪Z轴输出低字节
#define MPU6050_USER_CTRL 0x6A
#define MPU6050_PWR_MGMT_1 0x6B     //电源管理寄存器1
#define MPU6050_PWR_MGMT_2 0x6C     //电源管理寄存器2
#define MPU6050_FIFO_COUNTH 0x72
#define MPU6050_FIFO_COUNTL 0x73
#define MPU6050_FIFO_R_W 0x74
#define MPU6050_WHO_AM_I 0x75       //设备ID寄存器
#define MPU6050_XA_OFFSET_H 0x77
#define MPU6050_XA_OFFSET_L 0x78
#define MPU6050_YA_OFFSET_H 0x7A
#define MPU6050_YA_OFFSET_L 0x7B
#define MPU6050_ZA_OFFSET_H 0x7D
#define MPU6050_ZA_OFFSET_L 0x7E

void MPU6050_Init(void);
void MPU6050_Read_Accel(double *accel_x, double *accel_y, double *accel_z);
void MPU6050_Read_Gyro(double *gyro_x, double *gyro_y, double *gyro_z);
void MPU6050_Get_Euler_By_Gyro(double *roll, double *pitch, double *yaw);
void MPU6050_Get_Euler_By_Accel(double *roll, double *pitch, double *yaw);
void MPU6050_Get_Euler_By_Accel_Gyro(double *roll, double *pitch, double *yaw);
void MPU6050_Gyro_Calibrate();


void MPU6050_Read_Temperature(double *temperature);


#endif /* __MPU6050_H__ */