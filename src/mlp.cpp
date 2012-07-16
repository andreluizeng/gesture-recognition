// Multi-layer perceptron with Backpropagation algorithm
// Artificial Neural Network for the Gesture recognition Demo app.
// Developed by Andre Silva (andre.silva@freescale.com)

#include "mlp.h"


double **w1;
double **w2;


/*
int main (int argc, char *argv[])
{
	double **x;	// input
	double *d;	// output
	double **w1;	// weigths c1
	double **w2;	// weigths c2
	int i;
	int finish = FALSE;
	
	training_patterns = (double **) malloc (sizeof (double) * N_PATTERNS_T);
	
	for (i = 0; i < N_PATTERNS_T; i++)
		training_patterns[i] = (double *) malloc (sizeof (double) * N_INPUTS +  (1 * sizeof (double)));

	test_patterns = (double **) malloc (sizeof (double) * N_PATTERNS_E);
	
	for (i = 0; i < N_PATTERNS_E; i++)
		test_patterns[i] = (double *) malloc (sizeof (double) * N_INPUTS +  (1 * sizeof (double)));


	Initialize (&x, &d, &w1, &w2);	

		
	while (! finish)
	{

		// limpa o buffer do teclado
		fflush (stdin);
		// limpa o buffer do display
		fflush (stdout);

		double tt;

		switch (Menu ())
		{

			case 1	:	tt = (double)cvGetTickCount();
					ANNTraining (x, d, &w1, &w2);
					tt = (double)cvGetTickCount() - tt;
					printf( "\nProcess time = %gms\n\n", tt/(cvGetTickFrequency()*1000.));
					break;
					
			case 2	: 	GenNetWeigths (&w1, &w2);
					flag_learning = FALSE;
					break;
					
			case 3	: 	SaveNetWeigths (w1, w2);
					break;
					
			case 4	: 	LoadNetWeights (&w1, &w2);
					flag_learning = true;
					break;
					
			case 5	: 	DisplayMSE (x, d, w1, w2);
					break;
					
			case 6	: 	tt = (double)cvGetTickCount();
					ANNTest (w1, w2);
					tt = (double)cvGetTickCount() - tt;
					printf( "\nProcess time = %gms\n\n", tt/(cvGetTickFrequency()*1000.));
					break;
						
			case 7	:	if (! LoadPatterns (&x, &d))
					{
						printf ("\nError loading training file, aborting...\n");
						exit (0);
					}
			
					break;
						
			case 8	: 	finish = TRUE;
					break;

			default	: printf ("\nwrong option\n");
					  break;
		}
	}

	
	for (i = 0; i < N_PATTERNS_T; i++)
		free (training_patterns[i]);
	free (training_patterns);

	for (i = 0; i < N_PATTERNS_E; i++)
		free (test_patterns[i]);
	free (test_patterns);


	return 0;
}*/


void MLPInit (void)
{
	
	unsigned int i;
	unsigned int j;

	w1 = (double **) malloc (sizeof (double *) * N_INPUTS);

	if (! w1)
	{
		printf ("\n\nError allocating memory for the weights of C1 , aborting...");
		exit (1);
	}

	for (i = 0; i < N_INPUTS; i++)
	{
		w1[i] = (double *) malloc (sizeof (double ) * N_NEURONS_C1);

		if (! w1[i])
		{
			printf ("\n\nError allocating memory for the weights of C1 , aborting...");
			exit (1);
		}

	}

	w2 = (double **) malloc (sizeof (double *) * N_NEURONS_C1);

	if (! w2)
	{
		printf ("\n\nError allocating memory for the weights of C2 , aborting...");
		exit (1);
	}

	for (i = 0; i < N_NEURONS_C1; i++)
	{
		w2[i] = (double *) malloc (sizeof (double ) * N_NEURONS_C2);

		if (! w2[i])
		{
			printf ("\n\nError allocating memory for the weights of C2 , aborting...");
			exit (1);
		}

	}

	for (i = 0; i < N_INPUTS; i++)
		for (j = 0; j < N_NEURONS_C1; j++)
			w1[i][j] = 0;

	for (i = 0; i < N_NEURONS_C1; i++)
		for (j = 0; j < N_NEURONS_C2; j++)
			w2[i][j] = 0;


	return;
}

void LoadNetWeights (void)
{
	int i, j;
	
	FILE *fp;
	
	fp = fopen ("netweights.txt", "r");
	
	if (! fp)
	{
		printf ("\nError opening the netweights file\n");
		return;
	}
	
	for (i = 0; i < N_INPUTS; i++)
	{
		for (j = 0; j < N_NEURONS_C1; j++)
		{
			fscanf (fp, "%lf", &w1[i][j]);
		}
	}
			
	for (i = 0; i < N_NEURONS_C1; i++)
	{
		for (j = 0; j < N_NEURONS_C2; j++)
		{
			fscanf (fp, "%lf", &w2[i][j]);
		}
	}
	fclose (fp);	
	
	return;
}

// sigmoid function
double SigFunc (double val)
{
	double Res;

	Res = 1.0 / (1.0 + exp (-val));

	return Res;
}

// hiperboly tangent function 
double HiperTangFunc (double val)
{
	double Res;

	Res = (1.0 - exp (- val)) / (1.0 + exp (-val));

	return Res;
}

//absolute value calculation
double Absolute (double V)
{
	return (sqrt (pow (V, 2)));
}

double *MLPGestureRecog	(double *x)
{
	int i;
	int j;
	int k;
	int test = FALSE;


	double *i1;
	double *y1;
	double *i2;
	double *y2;

	double mean 		= 0;
	double variance 	= 0;
	double relative_error   = 0;


	if (! (i1 = (double *) malloc (sizeof (double) * N_NEURONS_C1)))
	{
		printf ("\nError allocating memory for the ANN test\n");
		exit (1);
	}

	if (! (y1 = (double *) malloc (sizeof (double) * N_NEURONS_C1)))
	{
		printf ("\nError allocating memory for the ANN test\n");
		exit (1);
	}

	if (! (i2 = (double *) malloc (sizeof (double) * N_NEURONS_C2)))
	{
		printf ("\nError allocating memory for the ANN test\n");
		exit (1);
	}

	if (! (y2 = (double *) malloc (sizeof (double) * N_NEURONS_C2)))
	{
		printf ("\nError allocating memory for the ANN test\n");
		exit (1);
	}

	for (i = 0; i < N_NEURONS_C1; i++) i1[i] = 0;
	for (i = 0; i < N_NEURONS_C1; i++) y1[i] = 0;
	for (i = 0; i < N_NEURONS_C2; i++) i2[i] = 0;
	for (i = 0; i < N_NEURONS_C2; i++) y2[i] = 0;


	// i1
	for (i = 0; i < N_INPUTS; i++)
	{
		for (j = 0; j < N_NEURONS_C1; j++)
		{
			i1[j] = i1[j] + w1[i][j] * x[i];
		}
	}

	// y1
	for (i = 0; i < N_NEURONS_C1; i++)
	{
		//y1[i] = SigFunc (i1[i]);
		y1[i] = HiperTangFunc (i1[i]);
	}

	for (i = 0; i < N_NEURONS_C1; i++)
	{
		for (j = 0; j < N_NEURONS_C2; j++)
		{
			i2[j] = i2[j] + w2[i][j] * y1[i];
		}
	}

	// y2
	double a;
	for (i = 0; i < N_NEURONS_C2; i++)
	{
		//y2[i] = SigFunc (i2[i]);
		y2[i] = HiperTangFunc (i2[i]);
	}
	


	free (i1);
	free (y1);
	free (i2);
//	free (y2);

	return y2;
}



