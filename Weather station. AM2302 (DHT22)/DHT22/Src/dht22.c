#include "dht22.h"
//--------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}
//--------------------------------------------------
void port_init(void)
{
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
  GPIOB->CRH |= GPIO_CRH_MODE11;
  GPIOB->CRH |= GPIO_CRH_CNF11_0;
  GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
}
//--------------------------------------------------
uint8_t dht22_init(void)
{
	HAL_Delay(2000);
  GPIOB->ODR |= GPIO_ODR_ODR11;//������� �������
  return 0;
}
//----------------------------------------------------------
uint8_t dht22_GetData(uint8_t *data)
{
	uint8_t i, j = 0;
	//reset port
	GPIOB->ODR &= ~GPIO_ODR_ODR11;//������ �������
	GPIOB->ODR |= GPIO_ODR_ODR11;//������� �������
	DelayMicro(100000);
	GPIOB->ODR &= ~GPIO_ODR_ODR11;//������ �������
	DelayMicro(18000);
	GPIOB->ODR |= GPIO_ODR_ODR11;//������� �������
	//�������� ������ �������
	DelayMicro(39);//20-40 ���
	//���� ������ ��� �� ������� ������������� ����, �� ������
	if(GPIOB->IDR & GPIO_IDR_IDR11) {
		return 0;
	}
	DelayMicro(80);
	//���� ������ �� �������� ����, �� ������
	if(!(GPIOB->IDR & GPIO_IDR_IDR11)) {
		return 0;
	}
	DelayMicro(80);
	//������ ������ (���������� ����� � ������ ��������,
	//��� ��� ������� ��������� �������, ����� �����
	//�� �������������� ������������ ���������)
	for (j=0; j<5; j++)
	{
		data[4-j]=0;
		for(i=0; i<8; i++)
		{
			while(!(GPIOB->IDR & GPIO_IDR_IDR11)); //��� ���������� ����
			DelayMicro(30);
			if(GPIOB->IDR & GPIO_IDR_IDR11) //������ ��������� �� ���������� 30 �����������
				//���� ���� �� ��� ����� �� ����������� � �����, �� ������ ��� �������, � ���� �����������, �� ����
				data[4-j] |= (1<<(7-i));
			while(GPIOB->IDR & GPIO_IDR_IDR11); //���, ���� ������ �������� ���� (� ������ �������)
		}
	}
	return 1;	
}
//----------------------------------------------------------
