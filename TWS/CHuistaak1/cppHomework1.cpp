#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <time.h>

/* Tijdsbesteding:
	7-8 uur
*/

/* Commando's:
	./cpphomework 20 10000 100 5 500 >>sum.out
	 g++ -Wall -std=c++14 -O1 -o cpphomework cppHomework1.cpp 
		=> sum.out en de optimalisatie -O1 veranderen naargelang gevraagde
*/

/* Vergelijking bucketSort en standaard sorting algoritme:
	De hoeveelheid buckets m wordt als volgt gekozen:
		Hoe meer buckets hoe meer kans dat er minder gesorteerd moet worden in de buckets zelf omdat
		de elementen van de vector verdeeld zijn.
		Als er evenveel buckets als element zijn en de vector uniform verdeeld is dan kan de 2de stap
		overgeslagen worden.
	Bucket sort kan heel voordelig zijn als de range van de vector gekend is en als de waarden in de
	vector uniform verdeeld zijn, want dan worden de elementen van de vector ongeveer gelijk verdeelt
	over de buckets, maar als weinig over de vector gekend is, is het moeilijk om te bepalen wat de 
	grenzen van de buckets moeten zijn en is het beter om de standaard sort te gebruiken.
	Als we beide plots van de 2 algoritmes bekijken zien we ook dat bucket sort bij grote vectors minder
	tijd nodig heeft dan de standaard sort.
	Complexiteit: n = aantal elementen in vector, m = aantal buckets
				  gemiddeld gezien O(n*log(n)) voor standaard sorting algoritme
				  O(n) in het beste geval bij bucket sort (evenveel buckets als vectorelementen en uniform)
				  O(n*log(n/m)) als er minder buckets dan elementen zijn.
				  O(n*log(n) als niet uniform verdeeld zou zijn.
*/


// Bucket sort function
void bucketSort(std::vector<double>& v, double range, int n){
	//n = amount of buckets
	if(n > (int) v.size()){
		n = (int) v.size();
	}
	std::vector<double> buckets[n];
	//Step 1: Put the elements of v in buckets according to hash function 'ind'
	for(int k=0; k< (int) v.size(); ++k){
		int ind = std::floor((v[k]*((double)(n-1)/(double) range))); 
		buckets[ind].push_back(v[k]);
	}
	//Step 2: Sort the elements in the buckets
	for(int k=0; k <n ;++k){
		sort(buckets[k].begin(),buckets[k].end());
	}
	//Step 3: Put the elements of the buckets back in the vector
	int index = 0;
	for(int k=0; k < n ;++k){
		
		if(!buckets[k].empty()){
			for(auto el: buckets[k]){
				v[index] = el;
				index++;
			}
		}
	}

}


int main( int argc, char* argv[]){
	/* argv[1] = size
	argv[2] = range
	argv[3] = number of experiments
	argv[4] = number of discarded timings
	argv[5] = number of buckets */
	
	//Assign the input arguments to variables
	int size = std::atoi(argv[1]);
	double r = std::atof(argv[2]);
	int numExp = std::atoi(argv[3]);
	int disExp = std::atoi(argv[4]);
	int m = std::atoi(argv[5]);
	
	//Sort the vectors from length 1 to length 2^'size'
	for(int j = 1;j < pow(2,size); j = j*2){
	
		//Initialize vector with size j
		std::vector<double> v(j,1);
	
		//Give the vector a random distribution with range r
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_real_distribution<> distribution(0, r);
		for(auto& vi:v){
			vi=distribution(generator);
		}
	
		double meanExp1 = 0;
		double meanExp2 = 0;
		double stdev1 = 0;
		double stdev2 = 0;
		struct timespec l_start, l_end;
		std::vector<double> timeSample1, timeSample2;
	
		for(int i = 0; i < numExp+1; ++i){
		
			//shuffle v with the given random number generator
			std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
		
			//Start timing
			clock_gettime(CLOCK_MONOTONIC, &l_start);
			
			std::sort(v.begin(), v.end());
	
			//Stop timing
			clock_gettime(CLOCK_MONOTONIC, &l_end);
			auto elapsed_time1 =(l_end.tv_sec - l_start.tv_sec) + (l_end.tv_nsec - l_start.tv_nsec)/1.0e9;
		
			if(i > disExp){
				meanExp1 = meanExp1 + elapsed_time1;
				timeSample1.push_back(elapsed_time1);
			}
			
			//Start timing
			clock_gettime(CLOCK_MONOTONIC, &l_start);
		
			bucketSort(v,r,m);
	
			//Stop timing
			clock_gettime(CLOCK_MONOTONIC, &l_end);
			auto elapsed_time2 =(l_end.tv_sec - l_start.tv_sec) + (l_end.tv_nsec - l_start.tv_nsec)/1.0e9;
		
			if(i > disExp){
				meanExp2 = meanExp2 + elapsed_time2;
				timeSample2.push_back(elapsed_time2);
			}
		}
		//Calculate mean and standard deviation of all the experiments
		double dif = numExp-disExp;
		meanExp1 = meanExp1/(dif);
		meanExp2 = meanExp2/(dif);
		for(int i = 0; i < numExp-disExp; ++i){
			stdev1 += pow(timeSample1[i] - meanExp1,2);
			stdev2 += pow(timeSample2[i] - meanExp2,2);
		}
		stdev1 = sqrt((stdev1)/(dif));
		stdev2 = sqrt((stdev2)/(dif));

		//Output timings of sort and bucket sort (Uncomment the one that's needed) 
		//std::cout<<j<<" "<<meanExp1<<" "<<stdev1<<std::endl;
		//std::cout<<j<<" "<<meanExp2<<" "<<stdev2<<std::endl;
	}	
	return 0;
}

