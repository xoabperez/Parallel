/*Homework O: Write serial code for CPUs in C++. In upcoming homework
this code will be parallelized with OpenMP and MPI and compared.

Outline: Define two 2D arrays (x,y). Initialize one array (x) with random 
numbers between 0 and 1. Derive the elements of the second array (y) 
from the first by smoothing over the elements of the first array, using
three constants a, b, and c:

y(i,j) = a * (x(i-1,j-1) + x(i-1,j+1) + x(i+1,j-1) + x(i+1,j+1)) +
         b * (x(i-1,j+0) + x(i+1,j+0) + x(i+0,j-1) + x(i+0,j+1)) +
         c *  x(i+0,j+0) 

Count the elements that are smaller than a threshold t in both arrays and 
print the number for both arrays. Collect timings for three major steps:
1. Populating the first array (x) with random numbers
2. Calculating the second array (y)
3. Counting of elements below threshold */

#include <iostream>
#include <cstdlib>
#include <chrono>
using namespace std;

void initialize(float array[][16384+2], int size){
    srand((unsigned)time(0));
    for (int i = 1; i <= size; i++){
        for (int j = 1; j <= size; j++) {
            array[i][j] = rand()/(float)RAND_MAX; // Populate x with random integer
        }
    }
}

void smooth(float arr_in[][16384+2], float arr_out[][16384+2],int size,
	    const float a, const float b, const float c ){
    for (int i = 2; i< size; i++){
	for (int j = 2; j< size; j++){ //Calculate Y
	    arr_out[i][j] = a*(arr_in[i-1][j-1] + arr_in[i-1][j+1] + arr_in[i+1][j-1] 
		 + arr_in[i+1][j+1])+b*(arr_in[i-1][j+0] +arr_in[i+1][j+0] +
		 arr_in[i+0][j-1] + arr_in[i+0][j+1]) + c*arr_in[i+0][j+0];
	}
    } 
}

void count(float array_in[][16384+2],int size,int threshold, int number){
    for (int i = 2; i < size; i++){
        for (int j = 2; j < size; j++) {
            if(array_in[i][j] <= threshold){number++;}
        }
    } 
}

int main()
{
    // Declaration of variables: size of arrays, smoothing constants, init. counts
    const int size = 16384+2; 
    const float a = .05, b = .1, c = .4, t = .1;
    int k = 0, l=0;

    // Allocate arrays and time
    auto x_alloc = std::chrono::high_resolution_clock::now(); //Begin stopwatch
    float x[size][size] = { }; 
    auto x_stop = std::chrono::high_resolution_clock::now(); //End stopwatch
    auto y_alloc = std::chrono::high_resolution_clock::now(); 
    float y[size][size] = { }; 
    auto y_stop = std::chrono::high_resolution_clock::now(); 

    // Populate array x with random numbers, then populate y by smoothing
    auto pop_x = std::chrono::high_resolution_clock::now(); 
    initialize(x,size);
    auto pop_x_stop = std::chrono::high_resolution_clock::now(); 

    auto smoothy = std::chrono::high_resolution_clock::now(); 
    smooth(x,y,size,a,b,c);
    auto smoothy_stop = std::chrono::high_resolution_clock::now(); 

    // Count number of elements below threshold in both arrays
    auto count_x = std::chrono::high_resolution_clock::now(); 
    count(x,size,t,k);
    auto count_x_stop = std::chrono::high_resolution_clock::now(); 
    auto count_y = std::chrono::high_resolution_clock::now(); 
    count(y,size,t,l);
    auto count_y_stop = std::chrono::high_resolution_clock::now(); 

    chrono::duration<double> allocate_x = x_stop-x_alloc; 
    chrono::duration<double> allocate_y = y_stop-y_alloc;
    chrono::duration<double> init_x = pop_x_stop-pop_x;
    chrono::duration<double> smooth_t = smoothy_stop-smoothy;
    chrono::duration<double> count_x_t = count_x_stop-count_x;
    chrono::duration<double> count_y_t = count_y_stop-count_y;
    const int outer = size*size, inner = (size-2)*(size-2);

    cout << "Summary" << endl; cout << "--------" << endl;
    cout << "Number of elements in a row/column : " << size << endl;
    cout << "Number of inner elements in a row/column : " << size-2 << endl;
    cout << "Total number of elements : " << outer << endl;
    cout << "Total number of inner elements : " << inner << endl;
    cout << "Memory used per array (GB) : " <<  sizeof(x) << endl;
    cout << "Threshold : " << t << endl;
    cout << "Smoothing constants (a,b,c) : " << a << ", " << b << ", " << c << endl;
    cout << "Number of elements below threshold (X) : " << k << endl;
    cout << "Fraction of elements below threshold (X) : " << k/inner << endl;
    cout << "Number of elements below threshold (Y) : " << l << endl;
    cout << "Fraction of elements below threshold (Y) : " << l/inner << endl;
    
    cout << "Action : time(s)" << endl; cout << "--------" << endl;
    cout << "Allocate X : " << allocate_x.count() << endl;
    cout << "Allocate Y : " << allocate_y.count() << endl;
    cout << "Initialize X : " << init_x.count() << endl;
    cout << "Smooth : " << smooth_t.count() << endl;
    cout << "Count X : " << count_x_t.count() << endl;
    cout << "Count Y : " << count_y_t.count()  << endl;

    return 0;
}
