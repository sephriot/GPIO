#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "GPIOClass.h"
#include <thread>
#include <chrono>
#include <termios.h>
using namespace std;

void sig_handler(int sig);

bool ctrl_c_pressed = false;

int main (void)
{

    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }

    string inputstate;

    GPIOClass* pin16 = new GPIOClass("23");
    GPIOClass* pin18 = new GPIOClass("24");
    GPIOClass* pin22 = new GPIOClass("25"); // ENABLE
    GPIOClass* pin19 = new GPIOClass("10"); // ENABLE
    GPIOClass* pin21 = new GPIOClass("9");
    GPIOClass* pin23 = new GPIOClass("11"); 


    pin16->export_gpio();
    pin18->export_gpio();
    pin22->export_gpio();

    pin19->export_gpio();
    pin21->export_gpio();
    pin23->export_gpio();

    cout << " GPIO pins exported" << endl;

    pin16->setdir_gpio("out");
    pin18->setdir_gpio("out");
    pin22->setdir_gpio("out");

    pin19->setdir_gpio("out");
    pin21->setdir_gpio("out");
    pin23->setdir_gpio("out");

    cout << " Set GPIO pin directions" << endl;

    struct termios old_tio, new_tio;
    unsigned char c = '*';

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while (c != 'q' && !ctrl_c_pressed)
    {
        /*  usleep(500000);
          pin17->getval_gpio(inputstate);
          cout << "Current input pin state is " << inputstate  << endl;
          if (inputstate == "0")
          {
              cout << "input pin state is Pressed. Will check input pin state again in 20ms " << endl;
              usleep(20000);
              cout << "Checking again ....." << endl;
              pin17->getval_gpio(inputstate);
              if (inputstate == "0")
              {
                  cout << "input pin state is definitely Pressed Turning LED ON" << endl;
                  gpio4->setval_gpio("1");

                  cout << " Waiting until pin is unpressed....." << endl;
                  while (inputstate == "0") {
                      pin17->getval_gpio(inputstate);
                  };
                  cout << "pin is unpressed" << endl;

              }
              else
                  cout << "input pin state is definitely UnPressed. That was just noise." << endl;

          }
          gpio4->setval_gpio("0");
        */


        c = getchar();
        printf("%c\n", c);
        switch (c) {
            case 'w':
                pin16->setval_gpio("1");
                pin18->setval_gpio("0");
                pin22->setval_gpio("1");
                break;
            case 's':
                pin16->setval_gpio("0");
                pin18->setval_gpio("1");
                pin22->setval_gpio("1");
                break;
            case 'a':
                pin21->setval_gpio("1");
                pin23->setval_gpio("0");
                pin19->setval_gpio("1");
                break;
            case 'd':
                pin21->setval_gpio("0");
                pin23->setval_gpio("1");
                pin19->setval_gpio("1");
                break;

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pin22->setval_gpio("0");
        pin19->setval_gpio("0");
        c = '*';

        if (ctrl_c_pressed)
        {
            cout << "Ctrl^C Pressed" << endl;
            cout << "unexporting pins" << endl;
            pin16->unexport_gpio();
            pin18->unexport_gpio();
            pin22->unexport_gpio();

            pin19->unexport_gpio();
            pin21->unexport_gpio();
            pin23->unexport_gpio();
            cout << "deallocating GPIO Objects" << endl;

            delete pin16;
            delete pin18;
            delete pin22;
            delete pin19;
            delete pin21;
            delete pin23;

            break;

        }

    }
    cout << "Exiting....." << endl;

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}

void sig_handler(int sig)
{
    write(0, "nCtrl^C pressed in sig handlern", 32);
    ctrl_c_pressed = true;
}