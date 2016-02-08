//������ ����������� 
//����������� ������: https://geektimes.ru/post/270592/
//---------------------------------------------
//������ ��� STM32Discovery-LV

#include "main.h"
	
// ������ ������� ������ ����������
u8 modes[] = {
   0B00000000, //��������� ��������
   0B11111111, //����� ���������
   0B00001111, //������� �� 0.5 ���
   0B00000001, //�������� ������� ��� � �������
   0B00000101, //��� �������� ������� ��� � �������
   0B00010101, //��� �������� ������� ��� � �������
   0B01010101  //������ �������� ������� (4 ���� � �������)
};

//Timer Functions
static u32 delay = 0;
static volatile u32 _millisCounter = 0;

u32 ms, ms1 = 0, ms2 = 0;
u8  blink_loop = 0;
u8  blink_mode = 0;
u8  modes_count = 0;

int main()
{
	setup();

	while (1)
	{
	   ms = get_mills();
	// ������� ������������� ������ 125 ��
	   if( ( ms - ms1 ) > 125|| ms < ms1 ){
	       ms1 = ms;
	// ����� ���������� ���� �� ������� �����
	       if( blink_mode & 1<<(blink_loop & 0x07) ) PIN_ON(USER_LED_green)
	       else  PIN_OFF(USER_LED_green);
	       blink_loop++;
	    }

	// ���� ��� ������ ��� ������������ ������������ �������
	// ���� ��� � 5 ������ ������ ������
	   if( ( ms - ms2 ) > 5000 || ms < ms2 ){
	       ms2 = ms;
	       blink_mode = modes[modes_count++];
	       if( modes_count >= 7 )modes_count = 0;
	   }
	}
}

void setup() {
	SystemInit();
	while(SysTick_Config(SystemCoreClock / 2400));	//configure SysTimer to 1 ms

	PIN_CONFIGURATION(USER_LED_green);
	modes_count = 1;
	blink_mode = modes[modes_count];
}

u32 get_mills(){
	return _millisCounter;
}
//==============================================================================
// ������ ����������� ������ 1��
//==============================================================================
void SysTick_Handler(void)
{
	if(delay){delay--;}
	_millisCounter++;
}
//==============================================================================
// ��������� ����������� �������� 1 ��
//==============================================================================
void delay_ms(u32 ms)
{ 
	delay=ms;
	while(delay){}
}