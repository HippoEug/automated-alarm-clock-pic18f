#include <xc.h>
#include "delays.h"
#include "lcd.h"
#include "keypad.h"

unsigned char hour, minute, second, hourA, minuteA, minuteG;
unsigned char hour10, hour1, minute10, minute1, second10, second1;

char int_2_char (unsigned char int1)
{
	char char1;
	switch (int1)
	{
		case 0 : char1 = '0' ; break;
		case 1 : char1 = '1' ; break;
		case 2 : char1 = '2' ; break;
		case 3 : char1 = '3' ; break;
		case 4 : char1 = '4' ; break;
		case 5 : char1 = '5' ; break;
		case 6 : char1 = '6' ; break;
		case 7 : char1 = '7' ; break;
		case 8 : char1 = '8' ; break;
		case 9 : char1 = '9' ; break;
		default : char1 = '?' ;
	}
	return(char1);
}

unsigned char char_2_int (char char1)
{
	unsigned char int1;
	switch (char1)
	{
		case '0' : int1 = 0 ; break;
		case '1' : int1 = 1 ; break;
		case '2' : int1 = 2 ; break;
		case '3' : int1 = 3 ; break;
		case '4' : int1 = 4 ; break;
		case '5' : int1 = 5 ; break;
		case '6' : int1 = 6 ; break;
		case '7' : int1 = 7 ; break;
		case '8' : int1 = 8 ; break;
		case '9' : int1 = 9 ; break;
		default : int1 = 0 ;
	}
	return(int1);
}

void interrupt ISR_Timer0_Int()			// Timer0 Interrupt Service Routine (ISR)
{
  if (INTCONbits.TMR0IF)		// TMR0IF:- Timer0 Overflow Interrupt Flag Bit
  								// 1 = TMR0 reg has overflowed
								// 0 = TMR0 reg has not overflowed
    {
        TMR0H = 0x48; // Timer0 start value = 0x48E5 for 1 second
        TMR0L = 0xE5;
       second = second + 1;
		if ( second > 59 )
		{
			second = 0;
			minute = minute + 1;
			if ( minute > 59 )
			{
				minute = 0;
				hour = hour + 1;
				if ( hour > 23 )
					hour = 0;
			}
		}

        INTCONbits.TMR0IF = 0; // Reset TMR0IF to "0" since the end of
								// the interrupt function has been reached
    }
}

void initial_display_time()
{
    lcd_write_cmd(0x80);
		lcd_write_data('T');
		lcd_write_data('i');
		lcd_write_data('m');
		lcd_write_data('e');
		lcd_write_data(' ');
		lcd_write_data('=');
		lcd_write_data(' ');
}

void initial_display_alarm()
{
    unsigned char msgindex, outchar;
	char Message[ ] = "Alarm ";

	lcd_write_cmd(0xC0);
	for (msgindex = 0; msgindex < 6; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}
}

void display_time()
{
    hour10 = hour / 10;
		hour1 = hour - hour10 * 10;

		minute10 = minute / 10;
		minute1 = minute - minute10 * 10;

		second10 = second / 10;
		second1 = second - second10 * 10;

        lcd_write_cmd(0x86);
		lcd_write_data(int_2_char (hour10));
		lcd_write_data(int_2_char (hour1));
		lcd_write_data(':');
		lcd_write_data(int_2_char (minute10));
		lcd_write_data(int_2_char (minute1));
		lcd_write_data(':');
		lcd_write_data(int_2_char (second10));
		lcd_write_data(int_2_char (second1));
        
}
void displayAlarmOn()
{
	lcd_write_cmd(0xC6);
		lcd_write_data('O');
		lcd_write_data('n');
        lcd_write_data(' ');
        lcd_write_data(' ');
}

void displayAlarmOff()
{
	lcd_write_cmd(0xC6);
		lcd_write_data('O');
		lcd_write_data('f');
        lcd_write_data('f');
        lcd_write_data(' ');
}

void displayAlarmGrace()
{
	lcd_write_cmd(0xC6);
		lcd_write_data('S');
		lcd_write_data('T');
        lcd_write_data('O');
        lcd_write_data('P');
}

void SetupTime()
{
	unsigned char msgindex, outchar, ctemp;
	char Message[ ] = "Set time(hhmmss)";

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 16; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}

	lcd_write_cmd(0xC0); // Move cursor to line 2 position 1

	ctemp=getkey(); // waits and get an ascii key number when pressed
	lcd_write_data(ctemp); //display on LCD
	hour10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	hour1 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	minute10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	minute1 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	second10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	second1 = char_2_int (ctemp);

	hour = hour10 * 10 + hour1;
	minute = minute10 * 10 + minute1;
	second = second10 * 10 + second1;

	delay_ms(500);
	lcd_write_cmd(0x01);
}

void SetupAlarmTime()
{
	unsigned char msgindex, outchar, ctemp;
	unsigned char hour10, hour1, minute10, minute1;    
	char Message[ ] = "Set alarm (hhmm)";

	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 16; msgindex++)
	{
		outchar = Message[msgindex];
		lcd_write_data(outchar);
	}

	lcd_write_cmd(0xC0); // Move cursor to line 2 position 1

	ctemp=getkey(); // waits and get an ascii key number when pressed
	lcd_write_data(ctemp); //display on LCD
	hour10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	hour1 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	minute10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	minute1 = char_2_int (ctemp);

	hourA = hour10 * 10 + hour1;
	minuteA = minute10 * 10 + minute1;

	delay_ms(500);
	lcd_write_cmd(0x01);
}

void SetupGraceTime()
{
	unsigned char msgindex, outchar, ctemp;	
	unsigned char minute10, minute1;
	char Message1[ ] = "GraceTime(01-30)";
	char Message2[ ] = "                ";
	lcd_write_cmd(0x80);

	for (msgindex = 0; msgindex < 16; msgindex++)
	{
		outchar = Message1[msgindex];
		lcd_write_data(outchar);
	}

	lcd_write_cmd(0xC0); // Move cursor to line 2 position 1

	ctemp=getkey(); // waits and get an ascii key number when pressed
	lcd_write_data(ctemp); //display on LCD
	minute10 = char_2_int (ctemp);

	ctemp=getkey();
	lcd_write_data(ctemp);
	minute1 = char_2_int (ctemp);

	minuteG = minute10 * 10 + minute1;
    
    delay_ms(500);
	lcd_write_cmd(0x01);
}

int light_sensor()     //RA0
{
#define THRESHOLD 76    //1.49v
    unsigned char linhao;
    ADCON0bits.GO=1;
    while(ADCON0bits.GO==1);
    linhao=ADRESH;
    if (linhao >= THRESHOLD)
        return 1;
    else
        return 0;
}

int motion_sensor()    //RC0
{
    if (PORTCbits.RC0==0)
        return 1;
    else
        return 0;
}

void onetone(void) //Function to generate one tone
{
    unsigned int k;
    for (k = 0; k < 500; k++) //Determines duration of tone
    {
        delay_us(500);  // usable values from 100  to 5000
        PORTCbits.RC2 = !PORTCbits.RC2; //Invert logic level at RC0
    }
}

void buzzer()           //RC2
{
    int i,j;
    j=0;
    for (i=0;i<375;i++) //roughly 3 min
    {
        if(light_sensor()==1 && motion_sensor()==1)
        {
            PORTCbits.RC2=0;
            break;
        }
        onetone();
        delay_ms(750-(j*25));
        if(j<=20)
        j++;
    }
}

void main(void) 
{
    unsigned int grace_h,grace_m,grace_status,grace_stop;
    grace_status=0;
    grace_stop=0;
                        //grace status: 0=grace off     1=grace on      2=grace read
    ADCON0=0b00000001;  
    ADCON1=0b00001110;
    ADCON2=0b00010110;
    
    TRISAbits.TRISA0=1; //in or out
    TRISCbits.TRISC0=1;
    TRISCbits.TRISC2=0;
    
    lcd_init();         //initialize keypad & lcd
    SetupTime();
	SetupAlarmTime();
    SetupGraceTime();
    
    RCONbits.IPEN=1;    //interrupt clock timer
    INTCONbits.GIEH=1;
    T0CON=0b00000111;
    INTCON2=0b10000100;
    TMR0H = 0x48;
    TMR0L = 0xE5;
    T0CONbits.TMR0ON = 1;
	INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    
    unsigned char displayUpdated,hour_negative;
    displayUpdated=1;
    
    /*if (minuteA <5)
    {
        grace_m = 55+minuteA;
        grace_h = hourA-1;
        hour_negative = 1;
    }
    else
    {
        grace_m = minuteA-5;
    	grace_h = hourA;
        hour_negative = 0;
    }*/
    if (minuteA < minuteG)
    {
        grace_m = (60-minuteG)+minuteA;
        grace_h = hourA-1;
        hour_negative = 1;
    }
    else
    {
        grace_m = minuteA-minuteG;
    	grace_h = hourA;
        hour_negative = 0;
    }
    
    initial_display_time();
    initial_display_alarm();
    displayAlarmOff();
    
    for(;;)
    {
        display_time();
     
        if (hour_negative==0 && grace_stop!=1)
        {
            if(hour==grace_h && hour==hourA && minute>=grace_m && minute<minuteA)
            {
                     if(light_sensor()==1 && motion_sensor()==1)
                     {
                     grace_status=1;
                     grace_stop=1;
                     displayAlarmGrace();
                     }
            }
        }   
        else
        {
            if(hour_negative==1 && grace_stop!=1)
            {
            if(hour>=grace_h && hour<=hourA && (minute>=grace_m || minute<minuteA))
                {
                    if(light_sensor()==1 && motion_sensor()==1)
                    {
                    grace_status=1;
                    grace_stop=1;
                    displayAlarmGrace();
                    }
                }
            }
        }
        
        if(hour==hourA && minute==minuteA)
		{
			if(displayUpdated == 2)
			{
                if(grace_status==1)
                {
                    displayAlarmOff();
                    displayUpdated = 1;
                    grace_status=2;
                }
                
                if(grace_status==0)
                {
                    displayAlarmOn();
                    buzzer();
                    displayAlarmOff();
                    displayUpdated = 1;
                    grace_status=2;
                }
			}
		}
		else
		{
			if(displayUpdated == 1)
			{
                displayUpdated = 2;
                grace_status=0;
                grace_stop=0;
			}
		}
    }
}
