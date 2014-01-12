#include <stdio.h>  
#include <stdlib.h>  
#include <sys/ioctl.h>  
int main(int argc, char **argv)  
{  
    unsigned int led_cmd = 0;  
    unsigned int led_num = 0;  
    int fd = 0;
	
    //参数赋值
    if(argc == 2)
    {
        sscanf(argv[1], "%d", &led_cmd);
    }
    else if(argc == 3)
    {
	sscanf(argv[1], "%d", &led_cmd);
	sscanf(argv[2], "%d", &led_num);
    }
    else
    {
	printf("open tiny6410 led device file failed\n");
	exit(1);
    }
	
    printf("led_num = %d, led_cmd = %d \n", led_num, led_cmd);
    fd = open("/dev/tiny6410_leds", 0);

    if(fd < 0)
    {
        printf("open tiny6410 leds device file failed \n");
        exit(1);
    }
    
    ioctl(fd, led_cmd, led_num);

    close(fd);
    return 0;  
} 
