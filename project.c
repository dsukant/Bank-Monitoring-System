#include"LPC2148.H"
void delay(int);
void data_lcd( unsigned int);
void cmd_lcd( unsigned int);
void lcd_putstring(unsigned char *);
void lcd_putchar( unsigned int );
void process_adc(void);
unsigned short adc_read();
 unsigned short int adc_value = 0;
main()
{
int k;
unsigned char *ch0="AT";
unsigned char *ch="AT+CMGF=1";
unsigned char *ch1="AT+CMGS=\"*********\"";
char *ch2="hello";
//unsigned char *ch2="ATH";
int ctrl=0x1A;
int ent=0x0D;

unsigned short IR;
int ir=0;
SetTime();		  //Real time clock
PINSEL2 |= 0X00000000;
lcd_init();
IO0CLR=0X80000000;
IO0DIR |= IO0DIR|(0<<17)|(1<<31);
init_adc();
uart_init();
    while(1)
    {
      if(HOUR==17 && MIN==0 && SEC==0)
      {
       process_adc();
           if(adc_value==0)
           {
            delay(1000);
           }
           else
           {
           	 while(*ch0!='\0')
           {
            sendbyte(*ch0);
            ch++;
           }
            sendbyte(ent);
             for(k=0;k<10;k++)
             {
              delay_msg();
             }
           while(*ch!='\0')
           {
            sendbyte(*ch);
            ch++;
           }
            sendbyte(ent);
             for(k=0;k<10;k++)
             {
              delay_msg();
             }

           while(*ch1!='\0')
           {
            sendbyte(*ch1);
            ch1++;
           }
            sendbyte(ent);
              for(k=0;k<10;k++)
              {
               delay_msg();
              }
           while(*ch2!='\0')
            {
             sendbyte(*ch2);
             ch2++;
            }
             sendbyte(ctrl);
             sendbyte(ent);
              for(k=0;k<10;k++)
              {
               delay_msg();
              }
             }

       }

     else
     {
      IR = IO0PIN & (1<<17);

       if(IR==0)
       {
        IO0SET=0X80000000;
        DELAY();
         ir++;
         data_lcd(ir+0x30);

       }
       else
       {
        IO0CLR=0X80000000;
        DELAY();
       }
     }
  }
}


     DELAY()
     {
       int i,j;
       for(i=0;i<1000;i++)
        {
         for(j=0;j<1000;j++);
        }
     }
  lcd_init()
  {
IO1DIR |= 1<<24;//rs
  IO1DIR |= 1<<23;//rw
  IO1DIR |= 1<<22;//en
  IO0DIR |= 1<<10;//d4
  IO0DIR |= 1<<11;//d5
  IO0DIR |= 1<<12;//d6
  IO0DIR |= 1<<13;//d7

	delay(1000);

	cmd_lcd(0x28);      /* 4-bit interface, two line, 5X7 dots.        */
	cmd_lcd(0x01);	    /*  LCD clear                                   */
	cmd_lcd(0x06);     /*  cursor move direction                       */
	cmd_lcd(0x0c);    /*  display on      */
	cmd_lcd(0x80);
  }
 void cmd_lcd(unsigned int w)
{
	unsigned int q;
	q=w;
IO1CLR |=1<<24;   //register select = 0 for command
IO1CLR |=1<<23;		//read/write = 0 for write
	q=q<<6;
IO1SET |=1<<22;		//enable to 1
IO0CLR =0xffffffff;
IO0SET =q;
	delay(1000);
IO1CLR |=1<<22;		//enable to 0
	delay(1000);
	w=w<<10;
IO1SET |=1<<22;		//enable to 1
IO0CLR =0xffffffff;
IO0SET =w;
	delay(1000);
IO1CLR |=1<<22;		//enable to 0
	delay(1000);
}

void data_lcd(unsigned int w)
{
	unsigned int q;
	q=w;
IO1SET |=1<<24;			//register select = 1 for data
IO1CLR |=1<<23;			//read/write = 0 for write
	q=q<<6;
IO1SET |=1<<22;		//enable to 1
IO0CLR =0xffffffff;
IO0SET =q;
	delay(1000);
IO1CLR |=1<<22;		//enable to 0
	delay(1000);
	w=w<<10;
IO1SET |=1<<22;		//enable to 1
IO0CLR =0xffffffff;
IO0SET =w;
	delay(1000);
IO1CLR |=1<<22;		//enable to 0
	delay(1000);
}
 void delay(int count)
{
  int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}
/*void str_lcd( unsigned int *str)
{
while(*str)
data_lcd(*str++);
}*/
SetTime()
{
CCR = 0x02;  // CCR ->clock control register , resets CTC clock tick counter
HOUR = 1;      //HOUR ,MIN and SEC are different registers of RTC
MIN = 19;
SEC = 55;
CCR = 0x11; // trigger the clock tick counter, enable the time counter and the elements in the Clock Tick Counter are
// reset. and configure to use external 32Khz freq on RTXC1 and RTXc2 as clock source
}
/*void lcd_putstring(unsigned char *string )
{
  while(*string != '\0')
  {
		  lcd_putchar( *string );
    string++;
  }
}
void lcd_putchar( unsigned int c )
{
  data_lcd( c );
} */
/*time()
{

cmd_lcd(0x80);
//str_lcd("HH:MM:SS");
lcd_putstring("HH:MM:SS");

cmd_lcd(0xc0);
data_lcd(48+(HOUR/10));        //  to get tens place data ,character representation of the number we add 48
data_lcd(48+(HOUR%10));         // to get ones place data
data_lcd(':');
data_lcd(48+(MIN/10));
data_lcd(48+(MIN%10));
data_lcd(':');
data_lcd(48+(SEC/10));
data_lcd(48+(SEC%10));
delay(1000);
delay(1000);
cmd_lcd(0x01);
}*/
uart_init()
{
VPBDIV=0X00000002;
PINSEL0|=0X00000005;
U0LCR=0X00000083;
U0DLL=0XC3;
U0DLM=0x0;
U0LCR=0X03;
}
init_adc()
{
PINSEL1= 0X10000000	;	// Initialize ADC
}
void process_adc(void)
{

unsigned char index = 0;

adc_value = adc_read();
}
unsigned short adc_read()
{
unsigned int i=0;

  AD0CR = 0x00200D00 | (1<<3);    // select channel number 3, divide PCLK by (13+1) and also make ADC operational
  AD0CR |= 0x01000000; // Start A/D Conversion=1

      do
      {
        i = AD0GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion i.e DONE signal (bit 31 of AD0GDR)=1


  return (i >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value, hence shifted by 6 and masked to directly give 10-bit result
}
sendbyte(char data)
{
while(!(U0LSR&0X20));
U0THR=data;
}
int recieved_byte()
{
while(!(U0LSR&0X01));
return(U0RBR);
}
delay_msg()
{
int i,j;
for(i=0;i<1000;i++)
{
for(j=0;j<50;j++);
}
}





