#include "mbed.h"
#include "arm_book_lib.h"

int main()

{
 
    DigitalIn b0(D2);
    DigitalIn b1(D3);
    DigitalIn b2(D4);
    DigitalIn b3(D5);
    DigitalIn b4(D6);
    DigitalIn b5(D7);

    DigitalOut alarm(LED1);
    DigitalOut lockdown(LED2);
    DigitalOut warning(LED3);

    b0.mode(PullDown);
    b1.mode(PullDown);
    b2.mode(PullDown);
    b3.mode(PullDown);
    b4.mode(PullDown);
    b5.mode(PullDown);

    alarm = OFF;
    lockdown = OFF;
    warning = OFF;

    int step = 0; 
    int digit1 = -1;
    int digit2 = -1;
    int digit3 = -1;
    int digit4 = -1;

    int countfail = 0; 
    int lockdowncount = 0; 
    bool LockCheck = false;  
    int LockTime = 0;

    while (true){ 
        
        int Buttonpress = -1;

        if (b0){
            Buttonpress = 0;
        } else if (b1 ){
            Buttonpress = 1;
        } else if(b2){
            Buttonpress = 2;
        } else if (b3){
           Buttonpress = 3;
        } else if (b4){
            Buttonpress = 4;
        } else if (b5 ){
            Buttonpress = 5;}

        if(Buttonpress != -1) {

            if (step == 0){
                digit1 = Buttonpress;  
            } else if (step == 1){
                digit2 = Buttonpress;
            } else if (step == 2){
                digit3 = Buttonpress;
            } else if (step == 3){
                digit4 = Buttonpress; }

            step = step + 1;  

            while (b0 || b1 || b2 || b3 || b4 || b5){
                ThisThread::sleep_for(15ms);
            }

            if(step == 4){ 
                
                if (LockCheck == false){
                    if (digit1 == 5 && digit2 == 1 && digit3 == 3 && digit4 == 2){ 
                        alarm = ON;
                        ThisThread::sleep_for(2000ms);
                        alarm = OFF;
                        countfail = 0; 
                    } else{
                        countfail = countfail + 1;

                        if ( countfail == 3){ 
                            int w = 0;
                            while ( w < 30 ){
                                warning = ON;
                                ThisThread::sleep_for(500ms);
                                warning =OFF;
                                ThisThread::sleep_for(500ms);
                                w = w + 1;
                            }
                        } else if ( countfail == 4 ) { 
                            LockCheck = true;
                            lockdowncount = lockdowncount + 1;
                            LockTime = 0; 
                        }
                        else {
                            
                            warning = ON;
                            ThisThread::sleep_for(2000ms);
                            warning = OFF;
                        } }
                } 
                
                else{
                    if (digit1 == 1 && digit2 == 1 && digit3 == 3 && digit4 == 0){ 
                        
                        LockCheck = false;
                        countfail = 0;
                        lockdown = OFF;
                        warning = OFF;
                        
                        int blinks = 0;
                        while(blinks < lockdowncount){
                            alarm = ON;
                            lockdown = ON;
                            warning = ON;
                            ThisThread::sleep_for(800ms);
                            alarm = OFF;
                            lockdown = OFF;
                            warning = OFF;
                            ThisThread::sleep_for(800ms);
                            blinks++;
                        }
                        }
                }

                step = 0;
                digit1 = -1;
                digit2 = -1;
                digit3 = -1;
                digit4 = -1;
            }
        }

        if(LockCheck == true ){
            
            lockdown = ON;
            
            if (LockTime < 1200){ 
                if ( (LockTime / 10) % 2 == 0 ){
                    warning = ON;
                } 
                
                else{
                    warning = OFF;
                }

                LockTime++;
            } else{
                
                warning = OFF;
            }
        }
        ThisThread::sleep_for(50ms);
    }  }
