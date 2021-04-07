#include "mbed.h"
#include "TextLCD.h"
#include "uLCD_4DGL.h"

using namespace std::chrono;

//TextLCD lcd(D0, D1, D2, D3, D4, D5);    // RS, E, DB4-DB7
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;

DigitalIn b0(D10);
DigitalIn b1(D11);
DigitalIn b2(D12);

AnalogOut aout(PA_4);
AnalogIn ain(A0);

Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;
InterruptIn YES(D10);
EventQueue queue(16 * EVENTS_EVENT_SIZE);

void samplef();
int button();

void LCD(int sel);
void ulcd();

void Fre_change();
void wave_g();

int rate = 8;
int T = 8;
int r = 0;

int main()
{
  t1.start(samplef);
  t2.start(button);
  t3.start(callback(&queue, &EventQueue::dispatch_forever));
  YES.rise(queue.event(Fre_change));
  t4.start(wave_g);
  t5.start(ulcd);
}

void wave_g()
{
  float i;
  int midT = 240 - 20 * T;     // middle sleep
  while (1) {
      // change the voltage on the digital output pin by 0.1 * (sleep time_
    for (i = 0.0f; i < 1.0f; i += 0.1f) {
      aout = i;
      ThisThread::sleep_for(T * 1ms);
    }
    ThisThread::sleep_for(midT * 1ms);
    for (; i > 0.0f; i -= 0.1f) {
      aout = i;
      ThisThread::sleep_for(T * 1ms);
    }    
  }
}

void samplef()
{
  int sample = 1000;
  float ain_data;
  int rate = 1000 / sample;
  while(1) {
    ain_data = ain;
    printf("%f\r\n", ain_data);
    ThisThread::sleep_for(rate * 1ms);
  }
}

int button()
{
  // basic printf demo = 16 by 18 characters on screen
  uLCD.printf("\nHello uLCD World\n"); //Default Green on black text
  uLCD.printf("\n  Starting Demo...");
  
  while (1)
  {
    
    if (b1.read() == 1 && rate > 1)
    {
      rate /= 2;
      uLCD.cls();
      ThisThread::sleep_for(500ms);
    }
    else if (b2.read() == 1 && rate < 8)
    {
      rate *= 2;
      uLCD.cls();
      ThisThread::sleep_for(500ms);
    }
  }
}

void Fre_change()
{
    T = rate;
    ThisThread::sleep_for(300ms);
}
void ulcd()
{
    uLCD.text_width(3); //4X size text
    uLCD.text_height(3);  
    uLCD.color(RED);
    uLCD.locate(1,2);
    uLCD.printf("rate = %d", rate);
    ThisThread::sleep_for(500ms);
}