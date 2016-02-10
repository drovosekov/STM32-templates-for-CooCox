//мигаем светодиодом 
//Оригнальная статья: https://geektimes.ru/post/270592/
//---------------------------------------------
//пример для STM32Discovery-LV

#include "main.h"
	
// Массив режимов работы светодиода
u8 modes[] = {
   0B00000000, //Светодиод выключен
   0B11111111, //Горит постоянно
   0B00001111, //Мигание по 0.5 сек
   0B00000001, //Короткая вспышка раз в секунду
   0B00000101, //Две короткие вспышки раз в секунду
   0B00010101, //Три короткие вспышки раз в секунду
   0B01010101  //Частые короткие вспышки (4 раза в секунду)
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
	// Событие срабатывающее каждые 125 мс
	   if( ( ms - ms1 ) > 125|| ms < ms1 ){
	       ms1 = ms;
	// Режим светодиода ищем по битовой маске
	       if( blink_mode & 1<<(blink_loop & 0x07) ) PIN_ON(USER_LED_green)
	       else  PIN_OFF(USER_LED_green);
	       blink_loop++;
	    }

	// Этот код служит для демонстрации переключения режимов
	// Один раз в 5 секунд меняем эффект
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
// Таймер срабатывает каждую 1мс
//==============================================================================
void SysTick_Handler(void)
{
	if(delay){delay--;}
	_millisCounter++;
}
//==============================================================================
// Процедура программной задержки 1 мс
//==============================================================================
void delay_ms(u32 ms)
{ 
	delay=ms;
	while(delay){}
}