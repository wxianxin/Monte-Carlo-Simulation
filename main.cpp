//Author: Steven Wang    Date: 20151102

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

double normalDistribution(double mean, double standard_deviation)
{
	static double V1, V2, S;
	double z;

	for (int i = 0; i < 1000; i++)
	{
		static int phase = 0;

		if (phase == 0)
		{
			do
			{
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2.0 * U1 - 1.0;
				V2 = 2.0 * U2 - 1.0;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			z = ((V1 * sqrt(-2.0 * log(S) / S)) * standard_deviation) + mean;
		}
		else z = ((V2 * sqrt(-2.0 * log(S) / S)) * standard_deviation) + mean;

		phase = 1 - phase;

	}
	return z;
}

int main()
{

	srand(time(0));

	double underlyingPrice;
	double v;
	double strike;
	double r;
	double t;

	int numSimulations;
	int numTimeIncrements;


	cout << "Initial Underlying Price: ";
	cin >> underlyingPrice;

	cout << "Volatility (%): ";
	cin >> v;
	v /= 100.0;

	cout << "Strike Price: ";
	cin >> strike;

	cout << "Risk Free Rate (%): ";
	cin >> r;
	r /= 100.0;

	cout << "TimeToExpiration: ";
	cin >> t;

	cout << "Number of Simulations: ";
	cin >> numSimulations;

	cout << "Number of Time Increments: ";
	cin >> numTimeIncrements;

	double deltaT = t / numTimeIncrements;
	double brownianStandardDeviation = sqrt(deltaT);


	double sum = 0.0;
	for (int i = 0; i < numSimulations; i++)
	{
		double underlyingPriceAtTimeT = underlyingPrice;
		for (int j = 0; j < numTimeIncrements; j++)
		{
			underlyingPriceAtTimeT += (r*underlyingPriceAtTimeT*deltaT + v*underlyingPriceAtTimeT*normalDistribution(0.0, brownianStandardDeviation));
		}

		if ((underlyingPriceAtTimeT - strike) > 0.0)
		{
			sum += (underlyingPriceAtTimeT - strike);
		}
	}

	double optionFutureValue = (sum / numSimulations);

	double expectedOptionValue = optionFutureValue*exp(-r*t);

	cout << "Price of Call option is: " << expectedOptionValue << endl;

	return 0;
}
