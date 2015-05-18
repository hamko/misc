/*
 * File:   main.c
 * Author: ryo
 *
 * Created on 2014/12/19, 15:39
 */

#include <xc.h>
#include <stdint.h>

#pragma config BOREN = OFF //4V???????????OFF
#pragma config CPD = OFF //??????????????OFF
//#pragma config FOSC = INTRC_NOCLKOUT //??????
#pragma config FOSC = HS //??????
#pragma config MCLRE = ON //??????????OFF(RA5????????)
#pragma config WDTE = OFF //???????????OFF
#pragma config CP = OFF //???????????????OFF
#pragma config LVP = OFF //????????????OFF
#pragma config PWRTE = ON //??????????ON

#define _XTAL_FREQ 20000000    // delay????(????4MHz???)

// I2C
#define sda RC4
#define sclk RC3

// I2C communication start
void start(void)
{
 sclk=1;
 sda=1;
 sda=0;
 sclk=0;
}
// I2C communication stop
void stop(void)
{
 sclk=1;
 sda=0;
 sda=1;
 sclk=0;
}
// I2C communication function
void write_byte(unsigned char byte)
{
    unsigned char i;
    for(i=8;i>0;i--) {
        if((byte & 0x80)==0x80)
            sda=1;
        else
            sda=0;
        sclk=1;
        sclk=0;
        byte=byte<<1;
    }
    sda=1;
    sclk=1;
    TRISC4=1;
    while(sda);
    sclk=0;
    TRISC4=0;
    __delay_us(100);
}
// Write command to LCD
void WriteCommand(unsigned int command)
{
 start();
 write_byte(0xa0); // slave address
 write_byte(0x00); // control byte = command mode
 write_byte(command);
 stop();
 __delay_ms(5);
}
// Write data to LCD
void WriteData(unsigned int data)
{
 start();
 write_byte(0xa0); // slave address
 write_byte(0x80); // control byte = data mode
 write_byte(data);
 stop();
 __delay_ms(5);
}

// Write Numeric (int32_t)
#define INT_32T_LEN 10
void WriteNumeric(int32_t in)
{
    if (in < 0)
        WriteData('-');
    unsigned char tmp[INT_32T_LEN];
    int i;
    for (i = 0; i < INT_32T_LEN; i++) {
        tmp[INT_32T_LEN-1-i] = in % 10;
        in /= 10;
    }
    int f = 0;
    for (i = 0; i < INT_32T_LEN; i++) {
        if (!f)
            f = tmp[i] != 0;
        if (f)
            WriteData(tmp[i] + '0');
    }
}

void inline initI2C(void) {
    __delay_ms(15);
    sda=1;
    sclk=1;
    WriteCommand(0x01); // clear disp (clean and return need wait)
    WriteCommand(0x38);
    WriteCommand(0x0f);
    WriteCommand(0x06);
}


// Timer Interrupt Function
unsigned int count = 0;
void interrupt InterTimer(void)
{
    if (T0IF) {
        TMR0 = 10, T0IF = 0; // reset TMR0 and T0IF
        RA1 = (count++ & 0xFFF > 0x8FF); // blink
    }
}

// Timer 2 if used for PWM generation.
// PWM = Timer 2 + GPIO + Comparator
#define T2_DIV_BY_1 0b00000000
#define T2_DIV_BY_4 0b00000001
#define T2_DIV_BY_16 0b00000010
#define CCP_PWM 0b00001100
#define PR2_DATA 0xFF
void inline setPWM1Duty(unsigned int duty)
{
    CCPR1L = duty >> 2;
    CCP1CON &= 0b11001111;
    CCP1CON |= 0b00110000 & (duty << 4);
}
void inline setPWM2Duty(unsigned int duty)
{
    CCPR2L = duty >> 2; // set lower 8 bit
    CCP2CON &= 0b11001111; // clear and
    CCP2CON |= 0b00110000 & (duty << 4); // set upper 2 bit
}

void inline initTimer2(unsigned char pr2, unsigned char t2ckps)
{
    PR2 = pr2;
    T2CON = t2ckps; // set prescalar
    T2CON |= 0x04; // timer 2 on
}
void inline initPWM1(void)
{
    CCP1CON = CCP_PWM; // single PWM
    setPWM1Duty(0);
}
void inline initPWM2(void)
{
    CCP2CON = CCP_PWM; // simgle PWM
    setPWM2Duty(0);
}

int main(void) {
    unsigned char i;

    // Clock setting
    OSCCON |= 0b00010000;

    // GPIO settings
    ANSEL = 0b10000000; // addddddd
    TRISA = 0b11110000; // iiiioooo
    TRISC = 0b00000000; // oooooooo for PWM and I2C
    
    // Analog setting
    ADCON0 = 0b00000001;  // enagle ADC
    ADCON0 &= 0b11000011; // clear reading mode
    ADCON0 |= 0b00011100; // reading-AN7 mode
    ADCON1 = 0b10000000;  // right justified, base vol. is VSS/VDD

    /* Timer Interrupt */
    // Timer 0 interrupt (prescalar 111=1/256)
    OPTION_REG = 0b00000111;
    TMR0   = 10;   // Init timer 0
    T0IF   = 0;     // Timer 0 Interrupt Flag
    T0IE   = 1;     // Timer 0 Interrupt Enable
    GIE    = 1;     // Global Interrupt Enable (if 0, all is disabled)

    // PWM (Use Timer 2 and Comparator 1, 2)
    initTimer2(PR2_DATA, T2_DIV_BY_1);
    initPWM1();
    initPWM2();
    setPWM1Duty(0);
    setPWM2Duty(0);

    // I2C initialize
    initI2C();

    // LCD write
    WriteCommand(0x80);
    const unsigned char table[]="testtest";
    for(i=0;i<8;i++)
        WriteData(table[i]);
    WriteData(' ');

    // int32_t caluculation
    int32_t a = 300;
    int32_t b = 300;
    int32_t c = a * b / 2;
    WriteNumeric(c);

    // Float calclation
    float x = 1.3;
    float y = x * 4 / 1.02;
    WriteCommand(0xc0); // set cursor the second line (101... don't work)
    WriteNumeric((int)y);

    while (1) {
        // Digital IO (RA5)
        RA3 = RA5;

        // PWM
        setPWM1Duty(1023);

        // Analog Read (the output is PWM2)
        GO_DONE = 1;
        while(GO_DONE);
        setPWM2Duty((ADRESH << 8) | ADRESL);
    }
}




