#ifndef __PORT_BH1750_H
#define __PORT_BH1750_H

#define BHAddWrite     0x46      //�ӻ���ַ+���д����λ
#define BHAddRead      0x47      //�ӻ���ַ+��������λ
#define BHPowDown      0x00      //�ر�ģ��
#define BHPowOn        0x01      //��ģ��ȴ�����ָ��
#define BHReset        0x07      //�������ݼĴ���ֵ��PowerOnģʽ����Ч
#define BHModeH1       0x10      //�߷ֱ��� ��λ1lx ����ʱ��120ms
#define BHModeH2       0x11      //�߷ֱ���ģʽ2 ��λ0.5lx ����ʱ��120ms
#define BHModeL        0x13      //�ͷֱ��� ��λ4lx ����ʱ��16ms
#define BHSigModeH     0x20      //һ�θ߷ֱ��� ���� ������ģ��ת�� PowerDownģʽ
#define BHSigModeH2    0x21      //ͬ������
#define BHSigModeL     0x23      // ������


#ifndef LINUX
#include "main.h"
//IO��������
 
#define SDA_IN() 		IIC_SDA_input() //{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)8<<28;}
#define SDA_OUT() 	IIC_SDA_output()//{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)3<<28;}

#define IIC_SCL_SET  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)  //PDout(6) //SCL
#define IIC_SCL_RESET 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)  //PDout(6) //SCL

#define IIC_SDA_SET  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET) //PDout(6) //SCL
#define IIC_SDA_RESET 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)  //PDout(6) //SCL

#define READ_SDA  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) //PDin(7)  //����SDA 

#define ADDR 0x23//0100011
#define uchar unsigned char 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	
void Single_Write_BH1750(uchar REG_Address);

#endif

void PORT_bh_Init(void);
void PORT_bh_data_send(unsigned char command);
unsigned short int PORT_bh_data_read(void);

#endif
