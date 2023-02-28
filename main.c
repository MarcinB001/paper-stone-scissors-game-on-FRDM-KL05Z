/*----------------------------------------------------------------------------
 *      Main: Initialize
 *---------------------------------------------------------------------------*/

#include "MKL05Z4.h"                    	/* Device header */
#include "buttons.h"											/* Buttons of external keyboard */
#include "lcd1602.h"

volatile int BUT1_press =0;
volatile int BUT2_press =0;
volatile int BUT3_press =0;
volatile int BUT4_press =0;
volatile int BUT5_press =0;
volatile int BUT6_press =0;

typedef enum{ BUT1 = 0, BUT2 = 1, BUT3 = 2 , BUT4 = 10, BUT5 = 11, BUT6 = 12} ButtonType;


 /*----------------------------------------------------------------------------
	Interrupt service routine
	Button will cause PORTA_PORTB interrupt 
 *----------------------------------------------------------------------------*/

void PORTB_IRQHandler(void){		
	
		
	uint32_t buf;
	
	buf = PORTB-> ISFR & (1<<BUT1 | 1<<BUT2 | 1<<BUT3);
	
	switch(buf)
	{
		
		case (1<<BUT1): DELAY(10)
									if((!(PTB->PDIR&(1<<BUT1))))     //zabezpieczenie przed drganiami zastykow
									{
										if(!(PTB->PDIR& (1<<BUT1)))
										{
											if(!BUT1_press)
											{
												BUT1_press=1;
											}
										}
									}
									break;
		case (1<<BUT2): DELAY(10)
									if((!(PTB->PDIR&(1<<BUT2))))
									{
										if(!(PTB->PDIR& (1<<BUT2)))
										{
											if(!BUT2_press)
											{
												BUT2_press=1;
											}
										}
									}
									break;
		case (1<<BUT3): DELAY(10)
									if((!(PTB->PDIR&(1<<BUT3))))
									{
										if(!(PTB->PDIR& (1<<BUT3)))
										{
											if(!BUT3_press)
											{
												BUT3_press=1;
											}
										}
									}
									break;
		default: 			break;
								
	}
	
	PORTB->ISFR |= ( 1<<BUT1 | 1<<BUT2 | 1<<BUT3);
	NVIC_ClearPendingIRQ(PORTB_IRQn);
}

void PORTA_IRQHandler(void){		
	
	
	uint32_t buf;
	
	buf = PORTA-> ISFR & (1<<BUT4 | 1<<BUT5 | 1<<BUT6);
	
	
	
	switch(buf)
	{
		
		case (1<<BUT4): DELAY(10)
									if((!(PTA->PDIR&(1<<BUT4))))
									{
										if(!(PTA->PDIR& (1<<BUT4)))
										{
											if(!BUT4_press)
												BUT4_press=1;
										}
									}
									break;
		case (1<<BUT5): DELAY(10)
									if((!(PTA->PDIR&(1<<BUT5))))
									{
										if(!(PTA->PDIR& (1<<BUT5)))
										{
											if(!BUT5_press)
												BUT5_press=1;
										}
									}
									break;
		case (1<<BUT6): DELAY(10)
									if((!(PTA->PDIR&(1<<BUT6))))
									{
										if(!(PTA->PDIR& (1<<BUT6)))
										{
											if(!BUT6_press)
												BUT6_press=1;
										}
									}
									break;
		default: 			break;
		
	}
	
	PORTA->ISFR |= ( 1<<BUT4 | 1<<BUT5 | 1<<BUT6);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	
}

int main(void)
{

	buttonsInitialize();	  /* Initialize buttons */
	LCD1602_Init(); /* initialize LCD */
	LCD1602_Backlight(TRUE);
	LCD1602_ClearAll();

	
	char a = '0';
	char b = '0';
	int t=0;
	int win_a=0;
	int win_b=0;
	
	while(1)
	{
			if(win_a==3)
			{
				LCD1602_SetCursor(2,0);
				LCD1602_Print("WYGRAL GRACZ A");
				t=2;
			}
			else if(win_b==3)
			{
				LCD1602_SetCursor(2,0);
				LCD1602_Print("WYGRAL GRACZ B");
				t=2;
			}
			
			if(t==0)
			{
				LCD1602_SetCursor(7,0);
				LCD1602_Print("1");
				DELAY(500)
				LCD1602_ClearAll();
				LCD1602_SetCursor(7,0);	
				LCD1602_Print("2");
				DELAY(500)
				LCD1602_ClearAll();
				LCD1602_SetCursor(7,0);
				LCD1602_Print("3");
				DELAY(500)
				LCD1602_ClearAll();
				LCD1602_SetCursor(5,0);
				LCD1602_Print("START");
				DELAY(500)
				LCD1602_ClearAll();
				t=1;
			}
		
			if(t!=2)
			{
			
				if(BUT1_press==1)
				{
					b='k';
					BUT1_press=0;
				}
				if(BUT2_press==1)
				{
					b='p';
					BUT2_press=0;
				}
				if(BUT3_press==1)
				{
					b='n';
					BUT3_press=0;
				}
				if(BUT4_press==1)
				{
					a='k';
					BUT4_press=0;
				}
				if(BUT5_press==1)
				{
					a='p';
					BUT5_press=0;
				}
				if(BUT6_press==1)
				{
					a='n';
					BUT6_press=0;
				}
		
				if( (a=='k' && b=='p') || (b=='k' && a=='n') || (b=='n' && a=='p')){
					a='0';
					b='0';
					t=0;
					win_b+=1;
				}else if((a=='k' && b=='k') || (a=='n' && b=='n') || (a=='p' && b=='p')){
					LCD1602_SetCursor(5,0);
					LCD1602_Print("REMIS");
					DELAY(1000)
					LCD1602_ClearAll();
					a='0';
					b='0';
					t=0;
				}else if((b=='n' && a=='k') || (a=='p' && b=='k') || (b=='p' && a=='n')){
					a='0';
					b='0';
					t=0;
					win_a+=1;
				}
				LCD1602_SetCursor(6,0);
				char win[3]= {win_a + '0' , ':', win_b + '0'};
				LCD1602_Print(win);
				LCD1602_SetCursor(6,1);
				LCD1602_Print("A B");
				DELAY(1000)
				LCD1602_ClearAll();
			}
	}
}
