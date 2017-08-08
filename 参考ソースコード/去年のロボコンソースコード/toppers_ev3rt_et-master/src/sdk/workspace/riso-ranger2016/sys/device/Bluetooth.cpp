///////////////////////////////////////////////////////////
//  Motor.cpp
//  Implementation of the Class Motor
//  Created on:      2016/07/26 18:58:12
//  Original author: 039389
///////////////////////////////////////////////////////////


#include "balancer_private.h"
#include "Bluetooth.h"

FILE *btlog = NULL;

static bool s_is_start = false;


void console_task(intptr_t unused) {

    btlog = ev3_serial_open_file(EV3_SERIAL_BT);

    while(1) {
        // 接続検知.
        while (!ev3_bluetooth_is_connected()) tslp_tsk(100);

        char c = fgetc(btlog);

        switch(c) {
            case '!':
                if(!s_is_start) {
                    fprintf(btlog, "Start signal reception\r\n");
                    s_is_start = true;
                }
                break;
            case 'q':
                K_F[0] += 0.1;
                fprintf(btlog, "K_F[0]=%f\r\n", K_F[0]);
                break;
            case 'a':
                K_F[0] -= 0.1;
                fprintf(btlog, "K_F[0]=%f\r\n", K_F[0]);
                break;
            case 'w':
                K_F[1] += 0.1;
                fprintf(btlog, "K_F[1]=%f\r\n", K_F[1]);
                break;
            case 's':
                K_F[1] -= 0.1;
                fprintf(btlog, "K_F[1]=%f\r\n", K_F[1]);
                break;
            case 'e':
                K_F[2] += 0.1;
                fprintf(btlog, "K_F[2]=%f\r\n", K_F[2]);
                break;
            case 'd':
                K_F[2] -= 0.1;
                fprintf(btlog, "K_F[2]=%f\r\n", K_F[2]);
                break;
            case 'r':
                K_F[3] += 0.1;
                fprintf(btlog, "K_F[3]=%f\r\n", K_F[3]);
                break;
            case 'f':
                K_F[3] -= 0.1;
                fprintf(btlog, "K_F[3]=%f\r\n", K_F[3]);
                break;
            case 't':
                K_I += 0.01;
                fprintf(btlog, "K_I=%f\r\n", K_I);
                break;
            case 'g':
                K_I -= 0.01;
                fprintf(btlog, "K_I=%f\r\n", K_I);
                break;
            case 'y':
                K_PHIDOT += 0.1;
                fprintf(btlog, "K_PHIDOT=%f\r\n", K_PHIDOT);
                break;
            case 'h':
                K_PHIDOT -= 0.1;
                fprintf(btlog, "K_PHIDOT=%f\r\n", K_PHIDOT);
                break;
            case 'u':
                K_THETADOT += 0.1;
                fprintf(btlog, "K_THETADOT=%f\r\n", K_THETADOT);
                break;
            case 'j':
                K_THETADOT -= 0.1;
                fprintf(btlog, "K_THETADOT=%f\r\n", K_THETADOT);
                break;
            default:
                fprintf(btlog, "Unknown key '%c' pressed.\r\n", c);
                break;
        }
    }
}

bool isStartSignal() {
    return s_is_start;
}
