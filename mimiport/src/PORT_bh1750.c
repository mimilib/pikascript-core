#include "PORT_bh1750.h"
#ifndef LINUX
#include "main.h"
#include "tim.h"
#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef LINUX
static void IIC_SDA_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void IIC_SDA_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

typedef unsigned char BYTE;
//BYTE    BUF[8];                         //�������ݻ�����
//iic�ӿڳ�ʼ��

void Single_Write_BH1750(uchar REG_Address)
{
	IIC_Start();				//��ʼ�ź�
	IIC_Send_Byte(BHAddWrite);	//�����豸��ַ+д�ź�
	IIC_Send_Byte(REG_Address); //�ڲ��Ĵ�����ַ��
								//  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
	IIC_Stop();					//����ֹͣ�ź�
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT(); //sda�����
	IIC_SDA_SET;
	IIC_SCL_SET;
	delay_us(4);
	IIC_SDA_RESET; //START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL_RESET; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT(); //sda�����
	IIC_SCL_RESET;
	IIC_SDA_RESET; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL_SET;
	IIC_SDA_SET; //����I2C���߽����ź�
	delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	SDA_IN(); //SDA����Ϊ����
	IIC_SDA_SET;
	delay_us(1);
	IIC_SCL_SET;
	delay_us(1);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_RESET; //ʱ�����0
	return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_RESET;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_RESET;
}
//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_SET;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_RESET;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(unsigned char txd)
{
	unsigned char t;
	SDA_OUT();
	IIC_SCL_RESET; //����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		//IIC_SDA=(txd&0x80)>>7;
		if ((txd & 0x80) >> 7)
			IIC_SDA_SET;
		else
			IIC_SDA_RESET;
		txd <<= 1;
		delay_us(2); //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_SET;
		delay_us(2);
		IIC_SCL_RESET;
		delay_us(2);
	}
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_RESET;
		delay_us(2);
		IIC_SCL_SET;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck(); //����nACK
	else
		IIC_Ack(); //����ACK
	return receive;
}

/*************************************************************************************/
#endif

void PORT_bh_data_send(unsigned char command)
{
#ifndef LINUX
	do
	{
		IIC_Start();			   //iic��ʼ�ź�
		IIC_Send_Byte(BHAddWrite); //����������ַ
	} while (IIC_Wait_Ack());	   //�ȴ��ӻ�Ӧ��
	IIC_Send_Byte(command);		   //����ָ��
	IIC_Wait_Ack();				   //�ȴ��ӻ�Ӧ��
	IIC_Stop();					   //iicֹͣ�ź�
#endif
#ifdef LINUX
	printf("send to bh1750 %d\r\n", command);
#endif
}

unsigned short int PORT_bh_data_read(void)
{
#ifndef LINUX
	unsigned short int buf;
	IIC_Start();					  //iic��ʼ�ź�
	IIC_Send_Byte(BHAddRead);		  //����������ַ+����־λ
	IIC_Wait_Ack();					  //�ȴ��ӻ�Ӧ��
	buf = IIC_Read_Byte(1);			  //��ȡ����
	buf = buf << 8;					  //��ȡ������߰�λ����
	buf += 0x00ff & IIC_Read_Byte(0); //��ȡ������ڰ�λ����
	IIC_Stop();						  //����ֹͣ�ź�
	return buf;
#endif
#ifdef LINUX
	unsigned short int buf = 55;
	return buf;
#endif
}

void PORT_bh_Init(void)
{
#ifndef LINUX
	Single_Write_BH1750(0x01);
#endif
#ifdef LINUX
	printf("init bh1750_PORT\r\n");
#endif
}
