#include "mbed.h"
#include "uLCD_4DGL.h"
using namespace std::chrono;

InterruptIn but1(D6);
InterruptIn but2(D8);
InterruptIn but3(D5);
AnalogOut Aout(PA_5);
AnalogIn Ain(A0);
uLCD_4DGL uLCD(D1, D0, D2); 

int temp_sel = 0;
int sel = 0;
EventQueue Draw;
Thread t;
Thread thread(osPriorityHigh);
Thread sample(osPriorityAboveNormal);
EventQueue sampleQueue;
const int sample_times = 200; 

void draw();
void wave_gen();
void wave_sample();

void Up()
{
    if (temp_sel < 3) {
        temp_sel++;
    } else {
        temp_sel = 3;
    }
}

void Down()
{
    if (0 < temp_sel) {
        temp_sel--;
    } else {
        temp_sel = 0;
    }
}

void Cofirm()
{
    sel = temp_sel;
    Draw.call(&draw);
    sampleQueue.call(&wave_sample);
}

void draw(){
    if (sel == 0) {
            uLCD.locate(0,0);
            uLCD.printf("1/8  \n");
        } else if (sel == 1) {
            uLCD.locate(0,0);
            uLCD.printf("1/4  \n");
        } else if (sel == 2) {
            uLCD.locate(0,0);
            uLCD.printf("1/2  \n");
        } else {
            uLCD.locate(0,0);
            uLCD.printf("1    \n");
        }
}

int main(void)
{
    but1.rise(Up);
    but2.rise(Down);
    but3.rise(Cofirm);

    uLCD.reset();
    uLCD.locate(0,0);
    uLCD.printf("1/8  \n");
    t.start(callback(&Draw, &EventQueue::dispatch_forever));
    thread.start(wave_gen);
    sample.start(callback(&sampleQueue, &EventQueue::dispatch_forever));
    while (1) {
        ThisThread::sleep_for(2ms);
    }
}

void wave_gen()
{
    int i = 0;
    float out = 0, add_0 = 0.909/80, add_1 = 0.909/40, add_2 = 0.909/20, add_3 = 0.909/10;

    while (true) {
        for (i = 0, out = 0; i < 240; i++) {
            if ((0 <= i) && (i < 80)) {
                if ((sel == 0) && (out < 0.909)) {
                    out += add_0;
                } else if ((sel == 1) && (out < 0.908)) {
                    out += add_1;
                } else if ((sel == 2) && (out < 0.908)) {
                    out += add_2;
                } else if ((sel == 3) && (out < 0.908)) {
                    out += add_3;
                }
            } else if ((160 < i) && (i <= 240)) {
                if ((sel == 0) && (i > 160)) {
                    out -= add_0;
                } else if ((sel == 1) && (i > 200)) {
                    out -= add_1;
                } else if ((sel == 2) && (i > 220)) {
                    out -= add_2;
                } else if ((sel == 3) && (i > 230)) {
                    out -= add_3;
                }
            } else {
                out = 0.909;
            }
            Aout = out;
            ThisThread::sleep_for(1ms);
        }
    }
}

void wave_sample()
{
    int i = 0;
    float ADC_value[sample_times];

    ThisThread::sleep_for(2s);
    for (i = 0; i < sample_times; i++) {
        ADC_value[i] = Ain;
        ThisThread::sleep_for(10ms);
    }
    for (i = 0; i < sample_times; i++) {
        printf("%f\r\n", ADC_value[i]);
        ThisThread::sleep_for(50ms);
    }
}