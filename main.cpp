#include "mbed.h"
#include "uLCD_4DGL.h"
DigitalIn b0(D10);
DigitalIn b1(D11);

AnalogOut aout(PA_4);
AnalogIn ain(A0);

uLCD_4DGL uLCD(D1, D0, D2);

Thread t1;
Thread t2;
Thread t3;

int T = 8;              // counting sleep time
InterruptIn b2(D12);    // to change the 
int rate = 8;
EventQueue queue(16 * EVENTS_EVENT_SIZE);

void sampling(){
  float adcdata;
  int rate = 1000; 
  while(1){   
    adcdata = ain;
    printf("%f\r\n", adcdata);
    ThisThread::sleep_for(1000ms/rate);
  }
}

void generate_wave(){
  float i;
  int midT = (240 - (20 * rate))*10;     // middle sleep
  while (1) {
      // change the voltage on the digital output pin by 0.1 * (sleep time_
    for (i = 0.0f; i < 1.0f; i += 0.01f) {
      aout = i;
      ThisThread::sleep_for(rate * 1ms);
    }

    ThisThread::sleep_for(midT * 1ms);

    for (; i > 0.0f; i -= 0.01f) {
      aout = i;
      ThisThread::sleep_for(rate * 1ms);
    }    
  }
} 

void button()
{
  while(1){
    if(b0.read()){
      if (T>0){
        T = T / 2;
        ThisThread::sleep_for(100ms);
        uLCD.cls();
        uLCD.printf("rate: %d / 8",T);
      }
    }
    if(b1.read()){
      if (T<9){
        T = T * 2;
        ThisThread::sleep_for(100ms);
        uLCD.cls();
        uLCD.printf("rate: %d / 8 ",T);

      }
    }
  }
}

void Yes(){
  rate = T;
  ThisThread::sleep_for(100ms);
  uLCD.cls();
  uLCD.printf("rate*: %d / 8",rate);
}



main()
{
  t1.start(sampling);
  t2.start(callback(&queue, &EventQueue::dispatch_forever));
  t3.start(button);
  b2.rise(queue.event(Yes));
  generate_wave();
}
