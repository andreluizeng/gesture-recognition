// Multi-layer perceptron with Backpropagation algorithm
// Artificial Neural Network for the Gesture recognition Demo app.
// Developed by Andre Silva (andre.silva@freescale.com)


#ifndef __MLP_H__
#define __MLP_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>

#define TRUE			1
#define FALSE 			0

#define ANN_W 240/4
#define ANN_H 320/4

// number of inputs
#define N_INPUTS		ANN_W*ANN_H+1

// number of neurons in the hidden layer
#define N_NEURONS_C1		50

// number of neurons in the output layer
#define N_NEURONS_C2		4



extern double **w1;
extern double **w2;


// Functions Prototypes 
void 	MLPInit			(void);
void	LoadNetWeights		(void);
double 	SigFunc 		(double val);
double 	HiperTangFunc 		(double val);
double 	Absolute 		(double V);
double 	*MLPGestureRecog	(double *x);



#endif