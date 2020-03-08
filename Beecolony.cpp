/**********************************************
Code designed by Somyajit Chakraborty and Marcus
**********************************************/


#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<cmath>
#include<fstream>
#include<iomanip>
using namespace std;

#define PI         3.1415926

const int NP = 40;//The number of colony size (employed bees+onlooker bees)
const int FoodNumber = NP / 2;//The number of food sources equals the half of the colony size
const int limit = 20;//A food source which could not be improved through limit trials is abandoned by its employed bee
const int maxCycle = 100;//The number of cycles for search

						
const int D = 2;//The number of parameters of the problem to be optimized
const double lb = -100;//lower bounds of the parameters
const double ub = 100;//upper bound of the parameters
double result[maxCycle] = { 0 };

//struct of Bees
struct BeeGroup
{
	double code[D];//the number of weights and bias is D 
	double trueFit;//objective function value 
	double fitness;//fitness is a vector holding fitness (quality) values associated with food sources
	double rfitness;//a vector holding probabilities of food sources (solutions) to be chosen
	int trail;//trial is a vector holding trial numbers through which solutions can not be improved
}Bee[FoodNumber];

/************************************************************************************/
//Foods(NectarSource) is the population of food sources. 
//Each row of Foods matrix is a vector holding D parameters to be optimized. 
//The number of rows of Foods matrix equals to the FoodNumber
/**************************************************************************************/

BeeGroup NectarSource[FoodNumber];
BeeGroup EmployedBee[FoodNumber];
BeeGroup OnLooker[FoodNumber];
BeeGroup BestSource;//The best food source is memorized

					
double random(double, double);
void initilize();
double calculationTruefit(BeeGroup);
double calculationFitness(double);
void CalculateProbabilities();
							 
void sendEmployedBees();
void sendOnlookerBees();
void sendScoutBees();
void MemorizeBestSource();



int main()
{
	char c;
	srand((unsigned)time(NULL));
	initilize();
	MemorizeBestSource();

						
	int gen = 0;
	while (gen<maxCycle)
	{
		sendEmployedBees();

		CalculateProbabilities();

		sendOnlookerBees();

		MemorizeBestSource();

		sendScoutBees();

		MemorizeBestSource();


		cout << "Time= " << gen << ", BestSource.trueFit= " << BestSource.trueFit << endl;
		cout << "Solutions= " << BestSource.code[0] << ", " << BestSource.code[1] << endl;

		gen++;
	}


	cout << "PROGRAM ENDED!!" << endl;
	cin >> c;
	return 0;
}

double random(double start, double end)
{
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

void initilize()
{
	int i, j;
	for (i = 0;i<FoodNumber;i++)
	{
		for (j = 0;j<D;j++)
		{
			//initialized food matrix FoodNumber*D and fulled with random values
			NectarSource[i].code[j] = random(lb, ub);
			EmployedBee[i].code[j] = NectarSource[i].code[j];
			OnLooker[i].code[j] = NectarSource[i].code[j];
			BestSource.code[j] = NectarSource[0].code[j];
		}
		//initialized the food
		NectarSource[i].trueFit = calculationTruefit(NectarSource[i]);
		NectarSource[i].fitness = calculationFitness(NectarSource[i].trueFit);
		NectarSource[i].rfitness = 0;
		NectarSource[i].trail = 0;
		//initialized the employed
		EmployedBee[i].trueFit = NectarSource[i].trueFit;
		EmployedBee[i].fitness = NectarSource[i].fitness;
		EmployedBee[i].rfitness = NectarSource[i].rfitness;
		EmployedBee[i].trail = NectarSource[i].trail;
		//initialized the onlooker
		OnLooker[i].trueFit = NectarSource[i].trueFit;
		OnLooker[i].fitness = NectarSource[i].fitness;
		OnLooker[i].rfitness = NectarSource[i].rfitness;
		OnLooker[i].trail = NectarSource[i].trail;
	}
	//initialized the best food
	BestSource.trueFit = NectarSource[0].trueFit;
	BestSource.fitness = NectarSource[0].fitness;
	BestSource.rfitness = NectarSource[0].rfitness;
	BestSource.trail = NectarSource[0].trail;
}

double calculationTruefit(BeeGroup bee)//calculate the value of the function
{
	double truefit = 0;
	//truefit = 0.5 + (sin(sqrt(bee.code[0] * bee.code[0] + bee.code[1] * bee.code[1]))*sin(sqrt(bee.code[0] * bee.code[0] + bee.code[1] * bee.code[1])) - 0.5)
	// ((1 + 0.001*(bee.code[0] * bee.code[0] + bee.code[1] * bee.code[1]))*(1 + 0.001*(bee.code[0] * bee.code[0] + bee.code[1] * bee.code[1])));
	truefit = 4 - (bee.code[0] * sin(4 * PI * bee.code[0]) - bee.code[1] * sin(4 * PI * bee.code[1] + PI + 1));

	return truefit;
}

double calculationFitness(double truefit)
{
	double fitnessResult = 0;
	if (truefit >= 0)
	{
		fitnessResult = 1 / (truefit + 1);
	}
	else
	{
		fitnessResult = 1 + abs(truefit);
	}
	return fitnessResult;
}


/***********************************************************************************/
//EMPLOYED BEE PHASE 
/***********************************************************************************/

void sendEmployedBees()
{
	int i, j, k;
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	for (i = 0;i<FoodNumber;i++)
	{
		//The parameter to be changed is determined randomly
		param2change = (int)random(0, D);

		//A randomly chosen solution is used in producing a mutant solution of the solution i
		//Randomly selected solution must be different from the solution i
		while (1)
		{
			k = (int)random(0, FoodNumber);
			if (k != i)
			{
				break;
			}
		}

		for (j = 0;j<D;j++)
		{
			EmployedBee[i].code[j] = NectarSource[i].code[j];
		}

		
		Rij = random(-1, 1);

		//sol(Param2Change)=Foods(i,Param2Change)+(Foods(i,Param2Change)-Foods(neighbour,Param2Change))*(rand-0.5)*2;
		//v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij})
		EmployedBee[i].code[param2change] = NectarSource[i].code[param2change] + Rij*(NectarSource[i].code[param2change] - NectarSource[k].code[param2change]);
		
		//shifted onto the boundaries if generated parameter value is out of boundaries
		if (EmployedBee[i].code[param2change]>ub)
		{
			EmployedBee[i].code[param2change] = ub;
		}
		if (EmployedBee[i].code[param2change]<lb)
		{
			EmployedBee[i].code[param2change] = lb;
		}

		//evaluate new solution
		EmployedBee[i].trueFit = calculationTruefit(EmployedBee[i]);
		EmployedBee[i].fitness = calculationFitness(EmployedBee[i].trueFit);

		//a greedy selection is applied between the current solution i and its mutant
		//If the mutant solution is better than the current solution i, 
		//replace the solution with the mutant and reset the trial counter of solution i
		if (EmployedBee[i].trueFit<NectarSource[i].trueFit)
		{
			for (j = 0;j<D;j++)
			{
				NectarSource[i].code[j] = EmployedBee[i].code[j];
			}
			NectarSource[i].trail = 0;
			NectarSource[i].trueFit = EmployedBee[i].trueFit;
			NectarSource[i].fitness = EmployedBee[i].fitness;
		}
		else
		{
			//if the solution i can not be improved, increase its trial counter
			NectarSource[i].trail++;
		}
	}
}

/***********************************************************************************/
//Calculate Probabilities 
//A food source is chosen with the probability which is proportioal to its quality
/***********************************************************************************/

void CalculateProbabilities()
{
	int i;
	double maxfit;
	maxfit = NectarSource[0].fitness;
	for (i = 1;i<FoodNumber;i++)
	{
		if (NectarSource[i].fitness>maxfit)
			maxfit = NectarSource[i].fitness;
	}

	for (i = 0;i<FoodNumber;i++)
	{
		NectarSource[i].rfitness = (0.9*(NectarSource[i].fitness / maxfit)) + 0.1;
	}
}

/***********************************************************************************/
//ONLOOKER BEE PHASE 
/***********************************************************************************/

void sendOnlookerBees()
{
	int i, j, t, k;
	double R_choosed;//the rate to be choosed
	int param2change;//parameter to be changed
	double Rij;//random number between [-1,1]
	i = 0;
	t = 0;
	while (t<FoodNumber)
	{

		R_choosed = random(0, 1);
		//A food source is chosen with the probability which is proportioal to its quality
		if (R_choosed<NectarSource[i].rfitness)
		{
			t++;
			//The parameter to be changed is determined randomly
			param2change = (int)random(0, D);

			//A randomly chosen solution is used in producing a mutant solution of the solution i
			//Randomly selected solution must be different from the solution i
			while (1)
			{
				k = (int)random(0, FoodNumber);
				if (k != i)
				{
					break;
				}
			}

			for (j = 0;j<D;j++)
			{
				OnLooker[i].code[j] = NectarSource[i].code[j];
			}

			//sol(Param2Change)=Foods(i,Param2Change)+(Foods(i,Param2Change)-Foods(neighbour,Param2Change))*(rand-0.5)*2;
			Rij = random(-1, 1);
			OnLooker[i].code[param2change] = NectarSource[i].code[param2change] + Rij*(NectarSource[i].code[param2change] - NectarSource[k].code[param2change]);

			//shifted onto the boundaries if generated parameter value is out of boundaries
			if (OnLooker[i].code[param2change]<lb)
			{
				OnLooker[i].code[param2change] = lb;
			}
			if (OnLooker[i].code[param2change]>ub)
			{
				OnLooker[i].code[param2change] = ub;
			}
			OnLooker[i].trueFit = calculationTruefit(OnLooker[i]);
			OnLooker[i].fitness = calculationFitness(OnLooker[i].trueFit);

			//a greedy selection is applied between the current solution i and its mutant
			//If the mutant solution is better than the current solution i, 
			//replace the solution with the mutant and reset the trial counter of solution i
			if (OnLooker[i].trueFit<NectarSource[i].trueFit)
			{
				for (j = 0;j<D;j++)
				{
					NectarSource[i].code[j] = OnLooker[i].code[j];
				}
				NectarSource[i].trail = 0;
				NectarSource[i].trueFit = OnLooker[i].trueFit;
				NectarSource[i].fitness = OnLooker[i].fitness;
			}
			else
			{
				NectarSource[i].trail++;
			}
		}
		i++;
		if (i == FoodNumber)
		{
			i = 0;
		}
	}
}

/*****************************************************************************/
//SCOUT BEE PHASE
//determine the food sources whose trial counter exceeds the "limit" value
//only one scout is allowed to occur in each cycle
/*****************************************************************************/

void sendScoutBees()
{
	int maxtrialindex, i, j;
	double R;//random number between [0,1]
	maxtrialindex = 0;
	for (i = 1;i<FoodNumber;i++)
	{
		if (NectarSource[i].trail>NectarSource[maxtrialindex].trail)
		{
			maxtrialindex = i;
		}
	}

	//determine the food sources whose trial counter exceeds the "limit" value
	if (NectarSource[maxtrialindex].trail >= limit)
	{
		//reinitailized food
		for (j = 0;j<D;j++)
		{
			R = random(0, 1);
			NectarSource[maxtrialindex].code[j] = lb + R*(ub - lb);
		}
		NectarSource[maxtrialindex].trail = 0;
		NectarSource[maxtrialindex].trueFit = calculationTruefit(NectarSource[maxtrialindex]);
		NectarSource[maxtrialindex].fitness = calculationFitness(NectarSource[maxtrialindex].trueFit);
	}
}

void MemorizeBestSource()//The best food source is memorized
{
	int i, j;
	for (i = 1;i<FoodNumber;i++)
	{
		if (NectarSource[i].trueFit<BestSource.trueFit)
		{
			for (j = 0;j<D;j++)
			{
				BestSource.code[j] = NectarSource[i].code[j];
			}
			BestSource.trueFit = NectarSource[i].trueFit;
		}
	}
}
