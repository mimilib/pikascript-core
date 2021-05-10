#include "VM_lcd.h"
#include "VM_delay.h"

int IO_operation_virtual(void)
{
	return 0;
}

/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(unsigned short int xsta, unsigned short int ysta, unsigned short int xend, unsigned short int yend, unsigned short int color)
{
	unsigned short int i, j;
	LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //������ʾ��Χ
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(unsigned short int x, unsigned short int y, unsigned short int color)
{
	LCD_Address_Set(x, y, x, y); //���ù��λ��
	LCD_WR_DATA(color);
}

/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	unsigned short int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //������������
	delta_y = y2 - y1;
	uRow = x1; //�����������
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //���õ�������
	else if (delta_x == 0)
		incx = 0; //��ֱ��
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //ˮƽ��
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //ѡȡ��������������
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color); //����
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}

/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
void Draw_Circle(unsigned short int x0, unsigned short int y0, unsigned char r, unsigned short int color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		LCD_DrawPoint(x0 - b, y0 - a, color); //3
		LCD_DrawPoint(x0 + b, y0 - a, color); //0
		LCD_DrawPoint(x0 - a, y0 + b, color); //1
		LCD_DrawPoint(x0 - a, y0 - b, color); //2
		LCD_DrawPoint(x0 + b, y0 + a, color); //4
		LCD_DrawPoint(x0 + a, y0 - b, color); //5
		LCD_DrawPoint(x0 + a, y0 + b, color); //6
		LCD_DrawPoint(x0 - b, y0 + a, color); //7
		a++;
		if ((a * a + b * b) > (r * r)) //�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(unsigned short int x, unsigned short int y, unsigned char num, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	unsigned char temp, sizex, t, m = 0;
	unsigned short int i, TypefaceNum; //һ���ַ���ռ�ֽڴ�С
	unsigned short int x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';									 //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); //���ù��λ��
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i]; //����6x12����
		else if (sizey == 16)
			temp = ascii_1608[num][i]; //����8x16����
		else if (sizey == 24)
			temp = ascii_2412[num][i]; //����12x24����
		else if (sizey == 32)
			temp = ascii_3216[num][i]; //����16x32����
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode) //�ǵ���ģʽ
			{
				if (temp & (0x01 << t))
					LCD_WR_DATA(fc);
				else
					LCD_WR_DATA(bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else //����ģʽ
			{
				if (temp & (0x01 << t))
					LCD_DrawPoint(x, y, fc); //��һ����
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(unsigned short int x, unsigned short int y, const unsigned char *p, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	while (*p != '\0')
	{
		LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
unsigned int mypow(unsigned char m, unsigned char n)
{
	unsigned int result = 1;
	while (n--)
		result *= m;
	return result;
}

/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(unsigned short int x, unsigned short int y, unsigned short int num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp;
	unsigned char enshow = 0;
	unsigned char sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(unsigned short int x, unsigned short int y, float num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp, sizex;
	unsigned short int num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(unsigned short int x, unsigned short int y, unsigned short int length, unsigned short int width, const unsigned char pic[])
{
	unsigned short int i, j;
	unsigned int k = 0;
	LCD_Address_Set(x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			LCD_WR_DATA8(pic[k * 2]);
			LCD_WR_DATA8(pic[k * 2 + 1]);
			k++;
		}
	}
}

void LCD_GPIO_Init(void)
{
	// do not need in HAL lib
}

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(unsigned char dat)
{
	unsigned char i;
	LCD_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr();
		if (dat & 0x80)
		{
			LCD_MOSI_Set();
		}
		else
		{
			LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat <<= 1;
	}
	LCD_CS_Set();
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(unsigned char dat)
{
	LCD_Writ_Bus(dat);
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(unsigned short int dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(unsigned char dat)
{
	LCD_DC_Clr(); //д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set(); //д����
}

/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a); //�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //������д
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a); //�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //������д
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a); //�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //�е�ַ����
		LCD_WR_DATA(y2);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //������д
	}
	else
	{
		LCD_WR_REG(0x2a); //�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //������д
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init(); //��ʼ��GPIO

	//�����Դ���λ��·  �����ֶ���λ
	//	LCD_RES_Clr();//��λ
	//	delay_ms(100);
	//	LCD_RES_Set();
	//	delay_ms(100);

	LCD_BLK_Set(); //�򿪱���
	Pub_VM_Delay->delay(Pub_VM_Delay, 100);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11);						//Sleep out
	Pub_VM_Delay->delay(Pub_VM_Delay, 120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate---------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	//------------------------------------ST7735S Power Sequence---------------------------------//
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36); //MX, MY, RGB mode
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);
	//------------------------------------ST7735S Gamma Sequence---------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); //Display on
}

unsigned char FontBuf[130]; //�ֿ⻺��
/******************************************************************************
      ����˵�������ֿ�д������
      ������ݣ�dat  Ҫд�������
      ����ֵ��  ��
******************************************************************************/
void ZK_command(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr();
		if (dat & 0x80)
		{
			LCD_MOSI_Set();
		}
		else
		{
			LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat <<= 1;
	}
}

/******************************************************************************
      ����˵�������ֿ��ȡ����
      ������ݣ���
      ����ֵ��  ret_data ��ȡ������
******************************************************************************/
unsigned char get_data_from_ROM(void)
{
	unsigned char i;
	unsigned char ret_data = 0; //�������ݳ�ʼ��
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr(); //�ֿ�ʱ������
		ret_data <<= 1;
		if (ZK_MISO)
		{
			ret_data++;
		}
		LCD_SCLK_Set(); //�ֿ�ʱ������
	}
	return ret_data; //���ض�����һ���ֽ�
}

/******************************************************************************
      ����˵������ȡN������
      ������ݣ�AddrHigh  д��ַ���ֽ�
                AddrMid   д��ַ���ֽ�
                AddrLow   д��ַ���ֽ�
                *pBuff    ��ȡ������
                DataLen   ��ȡ���ݵĳ���
      ����ֵ��  ��
******************************************************************************/
void get_n_bytes_data_from_ROM(unsigned char AddrHigh, unsigned char AddrMid, unsigned char AddrLow, unsigned char *pBuff, unsigned char DataLen)
{
	unsigned char i;
	ZK_CS_Clr();		  //�ֿ�Ƭѡ
	ZK_command(0x03);	  //дָ��
	ZK_command(AddrHigh); //д��ַ���ֽ�
	ZK_command(AddrMid);  //д��ַ���ֽ�
	ZK_command(AddrLow);  //д��ַ���ֽ�
	for (i = 0; i < DataLen; i++)
	{
		*(pBuff + i) = get_data_from_ROM(); //��һ���ֽ�����
	}

	ZK_CS_Set(); //ȡ���ֿ�Ƭѡ
}

/******************************************************************************
      ����˵������ʾASCII��
      ������ݣ�x,y      д�������
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Asc(unsigned short int x, unsigned short int y, unsigned char zk_num, unsigned short int fc, unsigned short int bc)
{
	unsigned char i, k;
	switch (zk_num)
	{
	case 1:
	{
		LCD_Address_Set(x, y, x + 7, y + 7);
		for (i = 0; i < 7; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //5x7 ASCII

	case 2:
	{
		LCD_Address_Set(x, y, x + 7, y + 7);
		for (i = 0; i < 8; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //	  7x8 ASCII

	case 3:
	{
		LCD_Address_Set(x, y, x + 7, y + 11);
		for (i = 0; i < 12; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  6x12 ASCII

	case 4:
	{
		LCD_Address_Set(x, y, x + 7, y + 15);
		for (i = 0; i < 16; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  8x16 ASCII

	case 5:
	{
		LCD_Address_Set(x, y, x + 15, y + 24);
		for (i = 0; i < 48; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  12x24 ASCII

	case 6:
	{
		LCD_Address_Set(x, y, x + 15, y + 31);
		for (i = 0; i < 64; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  16x32 ASCII
	}
}

/******************************************************************************
      ����˵������ʾASCII��
      ������ݣ�x,y      д�������
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Asc_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //�ָߡ��С��͵�ַ
	unsigned int FontAddr = 0;				  //�ֵ�ַ
	unsigned int BaseAdd = 0;				  //�ֿ����ַ
	unsigned char n, d;						  // ��ͬ�����ֿ�ļ������
	switch (zk_num)
	{
	//n������d:�ּ��
	case 1:
		BaseAdd = 0x1DDF80;
		n = 8;
		d = 6;
		break; //	  5x7 ASCII
	case 2:
		BaseAdd = 0x1DE280;
		n = 8;
		d = 8;
		break; //   7x8 ASCII
	case 3:
		BaseAdd = 0x1DBE00;
		n = 12;
		d = 6;
		break; //  6x12 ASCII
	case 4:
		BaseAdd = 0x1DD780;
		n = 16;
		d = 8;
		break; //  8x16 ASCII
	case 5:
		BaseAdd = 0x1DFF00;
		n = 48;
		d = 12;
		break; //  12x24 ASCII
	case 6:
		BaseAdd = 0x1E5A50;
		n = 64;
		d = 16;
		break; //  16x32 ASCII
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr & 0xff;										   /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Asc(x, y, zk_num, fc, bc);								   /*��ʾһ��ascii��LCD�� */
		}
		i++;	//�¸�����
		x += d; //��һ��������
	}
}

/******************************************************************************
      ����˵������ʾASCII��(Arial&Times New Roman)
      ������ݣ�x,y      д�������
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Arial_TimesNewRoman(unsigned short int x, unsigned short int y, unsigned char zk_num, unsigned short int fc, unsigned short int bc)
{
	unsigned char i, k;
	switch (zk_num)
	{
	case 1:
	{
		LCD_Address_Set(x, y, x + 15, y + 12);
		for (i = 2; i < 26; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  6x12 ASCII

	case 2:
	{
		LCD_Address_Set(x, y, x + 15, y + 17);
		for (i = 2; i < 34; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  8x16 ASCII
	case 3:
	{
		LCD_Address_Set(x, y, x + 23, y + 23);
		for (i = 2; i < 74; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  12x24 ASCII

	case 4:
	{
		LCD_Address_Set(x, y, x + 31, y + 31);
		for (i = 2; i < 130; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  16x32 ASCII
	}
}

/******************************************************************************
      ����˵������ʾASCII(Arial����)
      ������ݣ�x,y      д�������
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Arial_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //�ָߡ��С��͵�ַ
	unsigned int FontAddr = 0;				  //�ֵ�ַ
	unsigned int BaseAdd = 0;				  //�ֿ����ַ
	unsigned char n, d;						  // ��ͬ�����ֿ�ļ������
	switch (zk_num)
	{
	//n:������d:�ּ��
	case 1:
		BaseAdd = 0x1DC400;
		n = 26;
		d = 8;
		break; //  8x12 ASCII(Arial����)
	case 2:
		BaseAdd = 0x1DE580;
		n = 34;
		d = 12;
		break; //  12x16 ASCII(Arial����)
	case 3:
		BaseAdd = 0x1E22D0;
		n = 74;
		d = 16;
		break; //  16x24 ASCII(Arial����)
	case 4:
		BaseAdd = 0x1E99D0;
		n = 130;
		d = 24;
		break; //  24x32 ASCII(Arial����)
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr & 0xff;										   /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x, y, zk_num, fc, bc);				   /*��ʾһ��ascii��LCD�� */
		}
		i++;	//�¸�����
		x += d; //��һ��������
	}
}

/******************************************************************************
      ����˵������ʾASCII(Arial����)
      ������ݣ�x,y      д�������
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_TimesNewRoman_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //�ָߡ��С��͵�ַ
	unsigned int FontAddr = 0;				  //�ֵ�ַ
	unsigned int BaseAdd = 0;				  //�ֿ����ַ
	unsigned char n, d;						  // ��ͬ�����ֿ�ļ������
	switch (zk_num)
	{
	//n:������d:�ּ��
	case 1:
		BaseAdd = 0x1DCDC0;
		n = 26;
		d = 8;
		break; //  6x12 ASCII(TimesNewRoman����)
	case 2:
		BaseAdd = 0x1DF240;
		n = 34;
		d = 12;
		break; //  12x16 ASCII	(TimesNewRoman����)
	case 3:
		BaseAdd = 0x1E3E90;
		n = 74;
		d = 16;
		break; //  12x24 ASCII(TimesNewRoman����)
	case 4:
		BaseAdd = 0x1ECA90;
		n = 130;
		d = 24;
		break; //  16x32 ASCII(TimesNewRoman����)
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr & 0xff;										   /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x, y, zk_num, fc, bc);				   /*��ʾһ��ascii��LCD�� */
		}
		i++;	//�¸�����
		x += d; //��һ��������
	}
}
