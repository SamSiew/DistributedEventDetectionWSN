//
// Created by Siew on 10/19/2019.
//
#include <stdio.h>
#include <omp.h>
#include "MainHeader.h"

int encrypt(int value);
/**
 *  this encryptng functions is based on caesar cipher cryptography.
 *  encrypt functions takes in plain text of message and transform plain text to cipher text message.
 *  encrypt functions is used to secure message by encrypting message with new message that hide the original message.
 *
 * @param value, cipher text of message
 * @return message, plain text received from cipher text
 */
int encrypt(int value){
    int digit;
    int message=0;
    /**
     * assign each threads to replace respective digits of number to another value
     * main thread will reduce all message variables with sum.
     * it reads one digits at a time and shift up the value back to a new value by alphabet A that hide the original message.
     *  message is a variable that is always needs two extra digits to fit.
     *
     * 8 threads is used because it has the optimal speed up boost
     */
    #pragma omp private for num_threads(8) shared(message) reduction(+:message)
    for (digit = value; digit != 0; digit = digit/10)
    {
        message = (message*100) + (digit % 10 + A);
    }
    return message;
}