#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define  GPIO_LED_BASE  0x56000000
#define rGPGCON    0x00000060	//Port G control
#define rGPGDAT    0x00000064	//Port G data

void* base_led_arr;
unsigned int  *led_ctrl, *led_data;
static int fd;

int memopen(void)
{

   fd = open("/dev/mem",O_RDWR);  //메모리 장치오픈

   if(fd<0){
       perror("/dev/mem FAIL!\n");
       exit(1);
  } 

  return 0;
}

void led_addr()
{
    base_led_arr = (unsigned int *)mmap(NULL, 8,PROT_READ|PROT_WRITE,MAP_SHARED,fd,GPIO_LED_BASE);
    if(base_led_arr == MAP_FAILED)
    		printf("mmap error \n");

    	led_ctrl = (unsigned int*)((unsigned char*)base_led_arr + rGPGCON);
    	led_data = (unsigned int*)((unsigned char*)base_led_arr + rGPGDAT);

}

void led_init()
{
    *led_data  |= (0xF<<4);
    *led_ctrl &= ~(0xFF<<8);
    *led_ctrl |= (0x55<<8);
}

void Led_Display(int data)
{

	if(data == 1)		*led_data &= ~(1<<4); 	// 1 bit만 0으로 led On
	else if(data == 2)	*led_data &= ~(1<<5);  	
	else if(data == 3)	*led_data &= ~(1<<6);	
	else				*led_data &= ~(1<<7);


    	sleep(2);

	*led_data |= (0xF<<4);

	printf("\n Yes~~ Led Test Programm..\n");
}

void memrelease(void)
{

  munmap((unsigned char * )base_led_arr,8);

}

int main()
{
	int i;

	memopen();
	printf("memopen oper..\n");
                led_addr();
                printf("led_addr oper..\n");
                led_init();
                printf("led_init oper..\n");
    
	for(i=1; i<5; i++)
		Led_Display(i);

	memrelease();

	close(fd);

	return 0;
}
