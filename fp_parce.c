#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int isolateExp(unsigned int hex, unsigned int nExp, unsigned int nFrac); //Prototype for function to isolate the exponent bits
double calculateExp(unsigned int iExp); //Prototype for function to calculate the exponent
int isolateFrac(unsigned int hex, unsigned int iExp); //Prototype for function to isolate the fraction bits
double calculateFrac(unsigned int iFrac, unsigned int nFrac); //Prototype for function to calculate the fraction 
int isolateSign(unsigned int hex, unsigned int nExp, unsigned int nFrac); //Prototype for function to isolate the sign bit
double calculateIeee(double exp, double fraction, unsigned int nExp, unsigned int sign, double bias); //Prototype for function to calculate the IEEE floating point number

int main(int argc, char * argv[])
{
	char * ptr = NULL;
	unsigned int check = 0;			/* Check one and check 2 used to check for string inputs */
	unsigned int check2 = 0;		
	unsigned int nFrac, nExp, hex = 0;	//Number of fraction bits, exponent bits, and hex all passed by command line
	unsigned int iFrac, iExp, sign = 0;	//Isolated fraction bits, exponent bits, and sign bits
	double fraction = 0;			//Fraction calculated using fraction bits
	double exp = 0; 			//Exponent calculated using exponent bits
	double base = 2;			//Base of 2 used for calculations
	double bias = 0;			//Bias used for IEEE calculation 
	double iEEE = 0;			//IEEE representation
		
	for(int i = 0; i < strlen(argv[3]); i++)
	{
		if(!isxdigit(argv[3][i]))
		{
			printf("A string was detected\n");
			exit(0);
		}
	}

	sscanf(argv[1], "%d", &nFrac);		//Sscanf used to convert string into ints
	sscanf(argv[2], "%d", &nExp);
	sscanf(argv[3], "%x", &hex);

	
	
	if(nFrac < 2 || nFrac > 10)		//Checking fraction bits for correct input
	{
		printf("The fraction field is out of range, enter a number between 2 - 10\n");
		return(0);
	}

	if(nExp < 3 || nExp > 5)		//Cheching exponent bits for correct input
	{
		printf("The exponent field is out of range, enter a number between 3 - 5\n");
		return(0);
	}

	
	bias = pow(base, (nExp - 1)) - 1;	//Calculating bias using base and number of exponent bits
	
	iFrac = isolateFrac(hex, nFrac);	//Function call to isolateFrac to isolate fraction bits
	iExp = isolateExp(hex, nExp, nFrac);	//Function call to isolateExp to isolate exponent bits
	sign = isolateSign(hex, nExp, nExp);	//Function call to isolateSign to isolate sign bits


	fraction = calculateFrac(iFrac, nFrac);	//Function call to calculateFrac to calculate fraction taking in iFrac and nFrac
	exp = calculateExp(iExp);		//Function call to calculateExp to calculate exponent bits taking in iExp
	
	iEEE = calculateIeee(exp, fraction, nExp, sign, bias);		//Funtion call to calculateIeee to calculate the IEEE floating point number, taking in the exponent, fraction, number of exponent bits, sign, and bias
	

	printf("Fraction Bits: %d ", iFrac);		//Printing out all variable and the final IEEE floating point number
	printf("Fraction: %f ", fraction);
	printf("Exponent Bits: %d ", iExp);
	printf("Exponent: %f ", exp);
	printf("Sign Bits: %d \n", sign);
	printf("IEEE Number: %f \n", iEEE);

		

	return(0);	
}

int isolateExp(unsigned int hex, unsigned int nExp, unsigned int nFrac)
{
	unsigned int iExp = hex << (32 - (nFrac + nExp)) >> (32 - nExp);  //Bit shifting the hex value to isolate the exponent bits 
	return iExp;
}

double calculateExp(unsigned int iExp)		//Uses isolated exponent bits and math to calculate the exponent representation
{
	unsigned int eMask = 1;
	double base = 2;
	double exp = 0;
	double eCount = 0;

	while(eMask <= 16)
	{
		if(eMask & iExp)
		{
			if(!exp)
				exp = pow(base, eCount);
			else
				exp += pow(base, eCount);
		}	
		eCount += 1;
		eMask <<= 1;
	}

	return exp;
}

int isolateFrac(unsigned int hex, unsigned int nFrac)
{
	unsigned int iFrac = hex << (32 - nFrac) >> (32 - nFrac);		//Bit shifting to isolate the fraction bits 
	return iFrac;
}

double calculateFrac(unsigned int iFrac, unsigned int nFrac)		//Uses isolated fraction bits and math to calculate the fraction 
{
	unsigned int fMask = 0x80000000;
	double fraction = 0;
	double base = 2;
	double fCount = 1;

	fMask >>= (32 - nFrac);	
	while(fMask >= 1)
	{
		if(fMask & iFrac)
		{
			if(!fraction)
				fraction = 1/(pow(base, fCount));
			else 
				fraction += 1/(pow(base, fCount));
		}	
		fCount += 1;
		fMask >>= 1;
	}
	
	return fraction;
}

int isolateSign(unsigned int hex, unsigned int nExp, unsigned int nFrac)	
{
	unsigned int sign = hex >> nFrac + nExp;		//Uses bit shifting to isolate the sign bit
	return sign;
}

double calculateIeee(double exp, double fraction, unsigned int nExp, unsigned int sign, double bias)	//Uses fraction, exponent, number of exponentbits, sign bits, and bias to calculate the final IEEE floating point number
{
	double iEEE = 0;
	double M = 0;
	double E = 0;	
	int check = (1 << nExp) - 1;
	if(check == exp)
	{
		if(fraction == 0)
		{
			if(sign)
				printf("-infinity\n");
			else
				printf("+infinity\n");
			exit(0);
		}
		else{
			printf("NaN\n");
			exit(0);
		}
	}	
	if(exp == 0)
	{
		E = 1 - bias;	
		M = fraction;
	}
	
	else{
		E = exp - bias;	
		M = fraction + 1;
	}

	iEEE = pow(-1, sign) * M * pow(2, E);
	
	return iEEE;
}
