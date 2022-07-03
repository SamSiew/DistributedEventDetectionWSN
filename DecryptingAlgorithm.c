//
// Created by Siew on 10/19/2019.
//
//
// Created by Siew on 10/19/2019.
//
#include <stdio.h>
#include <omp.h>
#include "MainHeader.h"

int decrypt(int value);
/**
 * this decryptng functions is based on caesar cipher cryptography.
 * decrypt functions takes in cipher text of message and transform cipher text to plain text message.
 * decrypt functions is used to retrieved back the original message from cipher text.
 *
 * @param value, cipher text of message
 * @return message, plain text received from cipher text.
 */
int decrypt(int value){
    int digit;
    int message=0;
    /**
     * assign each threads to replace respective digits of number to another value
     * main thread will reduce all message variables with sum.
     * it reads two digits at a time and shift down the value back by alphabet A to original value
     *  message is a variable that is always needs one extra digits to fit.
     *
     * 8 threads is used because it has the optimal speed up boost
     */
    #pragma omp private for num_threads(8) shared(message) reduction(+:message)
    for (digit = value; digit != 0; digit = digit/100)
    {
        message = (message*10) + (digit%100 - A);
    }
    return message;
}
