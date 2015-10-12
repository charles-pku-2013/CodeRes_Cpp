#include "zigbee_common.h"

#define TRUE 0
#define FALSE -1

static
int speed_arr[] = {B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300,B115200,
          B57600,B38400, B19200, B9600, B4800, B2400, B1200, B300};

static
int name_arr[] = {115200,57600,38400, 19200, 9600, 4800,  2400,  1200,  300,  115200,
          57600, 38400,19200,  9600, 4800, 2400, 1200,  300};

int OpenSerial( int fd, const char* port)
{
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);

	if (FALSE == fd)
	{
		perror("Can't Open Serial Port\n");
		return (FALSE);
	}

	//判断串口的状态是否为阻塞状态							
	if(fcntl(fd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		return(FALSE);
	} 	

	//测试是否为终端设备	  
	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}

	return fd;
}

static
int SetSerial(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)

{
	int   i;
	int   status;
	struct termios options;

	/*tcgetattr(fd,&options)得到与fd指向对象的相关参数，
	并将它们保存于options,该函数,还可以测试配置
	是否正确，该串口是否可用等。若调用成功，
	函数返回值为0，若调用失败，函数返回值为1.
	*/

	if  ( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial error");    
		return (FALSE); 
	}

	//设置串口输入波特率和输出波特率
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
       {
	      if  (speed == name_arr[i])
	      {       
	              cfsetispeed(&options, speed_arr[i]); 
	              cfsetospeed(&options, speed_arr[i]);  
	      }
	}     

	//修改控制模式，保证程序不会占用串口
	options.c_cflag |= CLOCAL;

	//修改控制模式，使得能够从串口中读取输入数据
	options.c_cflag |= CREAD;

	//设置数据流控制
	switch(flow_ctrl)
	{
	case 0 ://不使用流控制
	        options.c_cflag &= ~CRTSCTS;
	        break;   
	case 1 ://使用硬件流控制
	        options.c_cflag |= CRTSCTS;
	        break;
	case 2 ://使用软件流控制
	        options.c_cflag |= IXON | IXOFF | IXANY;
	        break;
	}

	//设置数据位
	options.c_cflag &= ~CSIZE; //屏蔽其他标志位

	switch (databits)
	{  
	case 5    :
	        options.c_cflag |= CS5;
	        break;
	case 6    :
	         options.c_cflag |= CS6;
	         break;
	case 7    :    
	         options.c_cflag |= CS7;
	         break;
	case 8:    
	         options.c_cflag |= CS8;
	         break;  
	default:   
	         fprintf(stderr,"Unsupported data size\n");
	         return (FALSE); 
	}

	//设置校验位
	switch (parity)
	{  
	case 'n':
	case 'N': //无奇偶校验位。
	         options.c_cflag &= ~PARENB; 
	         //options.c_iflag &= ~INPCK;    
	         options.c_iflag =  IGNPAR;    
	         break; 
	case 'o':  
	case 'O'://设置为奇校验    
	         options.c_cflag |= (PARODD | PARENB); 
	         options.c_iflag |= INPCK;             
	         break; 
	case 'e': 
	case 'E'://设置为偶校验  
	         options.c_cflag |= PARENB;       
	         options.c_cflag &= ~PARODD;       
	         options.c_iflag |= INPCK;       
	         break;
	case 's':
	case 'S': //设置为空格 
	         options.c_cflag &= ~PARENB;
	         options.c_cflag &= ~CSTOPB;
	         break; 
	default:  
	         fprintf(stderr,"Unsupported parity\n");   
	         return (FALSE); 
	} 

	// 设置停止位 

	switch (stopbits)
	{  
	case 1:   
	         options.c_cflag &= ~CSTOPB; 
	         break; 
	case 2:   
	         options.c_cflag |= CSTOPB; 
	         break;
	default:   
	         fprintf(stderr,"Unsupported stop bits\n"); 
	         return (FALSE);
	}

	//修改输出模式，原始数据输出
	//options.c_oflag &= ~OPOST;
	//options.c_oflag = 0;
	//options.c_lflag = 0;
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/

	//设置等待时间和最小接收字符
	options.c_cc[VTIME] = 0; /* 读取一个字符等待1*(1/10)s */  
	options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */

	//如果发生数据溢出，接收数据，但是不再读取
	tcflush(fd,TCIFLUSH);

	//激活配置 (将修改后的termios数据设置到串口中）
	if (tcsetattr(fd,TCSANOW,&options) != 0)  
	{
	        perror("com set error!\n");  
		return (FALSE); 
	}

	return (TRUE); 
}

int InitSerial(int fd,  int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
	int err;

	//设置串口数据帧格式
	if (SetSerial(fd, speed, flow_ctrl, databits, stopbits, parity) == FALSE)
	{                                                     
		return FALSE;
	}
	else
	{
	       return  TRUE;
	}
}

		  

