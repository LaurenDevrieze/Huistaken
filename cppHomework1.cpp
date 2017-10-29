#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>
// misschien nog andere packages ontbreken

// bucket sort
void bucketSort(std::vector<float>& v, float range, int n){
	std::vector<float> buckets[n];
	for(int k=0; k< n; ++k){
		int ind = std::floor(v[k]*((float)(n-1)/range));
		buckets[ind].push_back(v[k]);
	}
	for(int k=0; k <n ;++k){
		sort(buckets[k].begin(),buckets[k].end());
		//bucketSort(buckets[k], 'wat is range?')
	}
	int index = 0;
	for(int k=0; k <n ;++k){
		for (int m = 0; m < buckets[k].size(); ++m)
		v[index++] = buckets[k][m];
	}
	delete[] buckets[n];
}


int main( int argc, char* argv[]){
	/* 	argv[0] = size
		argv[1] = range
		argv[2] = number of experiments
		argv[3] = number of discarded timings
		argv[4] = number of buckets */
	
	// String nog omzetten naar int
	std::cout<<argv[1]<<std::endl;
	int size = std::stoi(argv[1]);
	float r = std::stof(argv[2]);
	int numExp = std::stoi(argv[3]);
	int disExp = std::stoi(argv[4]);
	int m = std::stoi(argv[5]);

//	
for(int j = 1;j < size + 1 ; ++j){
	
	//initialeer vector with size j
	std::vector<float> v(j,1);
	
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<> distribution(0, r);
	for(auto& vi:v){
		vi=distribution(generator);
	}
	
	float meanExp1 = 0;
	float meanExp2 = 0;
	for(int i = 0; i < numExp+1; ++i){
		
		//shuffle v with the given random number generator
		std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
		
		//Start timing
		struct timespec l_start, l_end;
		clock_gettime(CLOCK_MONOTONIC, &l_start);
		
		std::sort(v.begin(), v.end());
	
		//Stop timing
		clock_gettime(CLOCK_MONOTONIC, &l_end);
		auto elapsed_time1 =(l_end.tv_sec - l_start.tv_sec) + (l_end.tv_nsec - l_start.tv_nsec)/1.0e9;
		
		if(i > disExp){
			meanExp1 = meanExp1 + elapsed_time1;
		}
		
		//Start timing
		clock_gettime(CLOCK_MONOTONIC, &l_start);
		
		bucketSort(v,r,m);
	
		//Stop timing
		clock_gettime(CLOCK_MONOTONIC, &l_end);
		auto elapsed_time2 =(l_end.tv_sec - l_start.tv_sec) + (l_end.tv_nsec - l_start.tv_nsec)/1.0e9;
		
		if(i > disExp){
			meanExp2 = meanExp2 + elapsed_time2;
		}
	}
	meanExp1 = meanExp1/((float)(numExp-disExp));
	meanExp2 = meanExp2/((float)(numExp-disExp));
	
	std::cout<<meanExp1<<" "<<meanExp2<<std::endl;
	
	delete[] v;
	
}
	return 0;
}



//pdflatex -shell-escape -interaction=nonstopmode plot.tex