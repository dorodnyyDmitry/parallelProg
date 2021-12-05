#include <iostream>
#include <chrono>
#include <math.h>
#include <emmintrin.h>
#include <immintrin.h>

typedef unsigned long long ull;

inline ull rdtsc() {
  unsigned int lo, hi;
  asm volatile ( "rdtsc\n" : "=a" (lo), "=d" (hi) );
  return ((ull)hi << 32) | lo;
}

namespace ch = std::chrono;

int main(){
  for(int power = 3; power < 9; ++power){
    double x = 123.0;
    double xx = 456.0;
    double xy = 0;
    double xxy = 0;

    auto start = ch::system_clock::now();
    auto end = ch::system_clock::now();
    ch::duration<double> diff = start - start;
    ull cstart = rdtsc();

    for (int i = 0; i < pow(10, power); ++i){
      start = ch::system_clock::now();
      sqrt(x);
      sqrt(xx);
      end = ch::system_clock::now();
      diff += end-start;
    }

    
    ull cend = rdtsc();
    
    ull cdiff = cend - cstart;
    std::cout << "\nNon-vec";
    std::cout 
      << "\nops: " << pow(10, power) 
      << "\ntime: " << diff.count() 
      << "\nclocks: " << cdiff 
      << "\nclock/s: " << cdiff/diff.count() 
      << "\nop/s: " << pow(10, power)/diff.count() << "\n";

    
    
    __m128d y = {123.0, 456.0};

    diff = start - start;
    cstart = rdtsc();

    for (int i = 0; i < pow(10, power) ; ++i){
      start = ch::system_clock::now();
      _mm_sqrt_pd(y);
      end = ch::system_clock::now();
      diff += end-start;
    }

    
    cend = rdtsc();
    
    cdiff = cend - cstart;
  
    std::cout << "\nVectorized";
    std::cout 
      << "\nops: " << pow(10, power) 
      << "\ntime: " << diff.count() 
      << "\nclocks: " << cdiff 
      << "\nclock/s: " << cdiff/diff.count() 
      << "\nop/s: " << pow(10, power)/diff.count() << "\n";
  }
}
