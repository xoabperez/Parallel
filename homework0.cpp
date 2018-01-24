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

int main()
{
    srand((unsigned)time(0));
    const int rows = 4, cols = 5; // Size of matrix y
    int x[rows+2][cols+2] = { }; //x larger to hold zeroes
    int y[rows][cols] = { }; 
    int a = 1, b = 1, c = 1, t = 4, k = 0; //Constants for y and count
    auto pop = std::chrono::high_resolution_clock::now(); //Begin stopwatch
    for (int i = 1; i <= rows; i++){
        for ( int j = 1; j <= cols; j++ ) {
            x[i][j] = rand()%10; // Populate x with random integer
        }
    }
    auto pop_stop = std::chrono::high_resolution_clock::now(); //End stopwatch
    // Display array x
    /*
    for (int i = 0; i <= (rows+1); i++){
        for ( int j = 0; j <= (cols+1); j++ ) {
            cout << x[i][j] << " " ;
        }
	cout << endl;
	}*/
    auto array = std::chrono::high_resolution_clock::now(); 
    for (int i = 1; i<= rows; i++){
      for (int j = 1; j<= cols; j++){ //Calculate Y
	y[i][j] = a*(x[i-1][j-1] + x[i-1][j+1] + x[i+1][j-1] + x[i+1][j+1]) +
         b*(x[i-1][j+0] + x[i+1][j+0] + x[i+0][j-1] + x[i+0][j+1]) +
	    c*x[i+0][j+0];
      }
    } 
    auto array_stop = std::chrono::high_resolution_clock::now(); 
    auto count = std::chrono::high_resolution_clock::now(); 
    for (int i = 0; i<rows; i++){
      for (int j = 0; j<cols; j++){
        if (y[i][j]<t){
	    k++; // If element is less than threshold, increase count
	}
      }
    } 
    auto count_stop = std::chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed_pop = pop_stop-pop;
    chrono::duration<double> elapsed_array = array_stop-array;
    chrono::duration<double> elapsed_count = count_stop-count;
    cout << "Number of elements below threshold: " << k << endl;
    cout << "Elapsed time for populating x = " << elapsed_pop.count() << endl;
    cout << "Elapsed time for calculating y = " << elapsed_array.count() << endl;
    cout << "Elapsed time for counting elements below threshold = " << 
	elapsed_count.count() << endl;
    return 0;
}
