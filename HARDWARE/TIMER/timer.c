#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/07
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20140306 
//增加TIM1_CH1，PWM输出设置相关内容 
//V1.2 20140307
//增加TIM2输入捕获初始化函数TIM2_Cap_Init及其中断处理
////////////////////////////////////////////////////////////////////////////////// 	  
extern vu16 USART2_RX_STA; 
//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{
		USART2_RX_STA|=1<<15;	//标记接收完成	
		TIM4->CR1&=~(1<<0);		//关闭定时器4
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值 
	TIM4->PSC=psc;  	//预分频器设置
	TIM4->DIER|=1<<0;   //允许更新中断				
	TIM4->CR1|=0x01;    //使能定时器4
  MY_NVIC_Init(0,2,TIM4_IRQn,2);//抢占1，子优先级3，组2									 
}
//TIM1_CH1 PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能    
	GPIOA->CRH&=0XFFFFFFF0;	//PA8清除之前的设置
	GPIOA->CRH|=0X0000000B;	//复用功能输出 
	
	TIM1->ARR=arr;			//设定计数器自动重装值 
	TIM1->PSC=psc;			//预分频器设置
  
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM1->CCMR1|=1<<3; 		//CH1预装载使能	 
 	TIM1->CCER|=1<<0;   	//OC1 输出使能	   
	TIM1->BDTR|=1<<15;   	//MOE 主输出使能	   

	TIM1->CR1=0x0080;   	//ARPE使能 
	TIM1->CR1|=0x01;    	//使能定时器1 										  
}  
















