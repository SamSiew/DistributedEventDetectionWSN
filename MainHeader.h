//
// Created by Siew on 10/19/2019.
//

#ifndef INC_2_MAINHEADER_H
#define INC_2_MAINHEADER_H
/**
 * a constant value that can  be used by all module that use this header
 * reduce need of copy pasting constant between module
 */
//size of message sent from nodes to base station
static const int NUM_OF_BASE_MESSAGE = 6;
//start index of message that all adjacent node is located
static const int ADJACENT_NODE_POS = 2;
//Maximum number of neighbour node
static const int MAX_NEIGHBOUR_NODE = 4;
//size of window sliding
static const int NUM_WINDOW = 3;
//start index of random position in buffer message
static const int RANDOM_POS = 1;

/**
 * constant value for all neighbour node in single node for cartesian
 */
// index of north neighbour node
static const int UP = 0;
// index of South neighbour node
static const int DOWN = 1;
// index of West neighbour node
static const int LEFT = 2;
// index of East neighbour node
static const int RIGHT = 3;
/**
 * The maximum and minimum of random number range in.
 */
static const int LOWER_BOUND = 950;
static const int UPPER_BOUND = 1000;
/**
 * The maximum numnber of iteration.
 */
static const int MAXIMUM_ITERATION = 10000;
/**
 * Ascii code of A
 */
static const char A = 'A';


#endif //INC_2_MAINHEADER_H
