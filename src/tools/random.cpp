#include <random>
#include <chrono>
#include <fstream>

std::mt19937_64 generator; // C++11 Mersene Twister 19937 generator (64 bit)

unsigned randomise(unsigned s=0) {
  if (!s) {std::random_device seeder; s=seeder();}
  if (!s) s = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  generator.seed(s);
  return s;
}

int32_t random(int32_t n) { return std::uniform_int_distribution<int32_t> (0,n-1) (generator); } // 0..(n-1)

double randomdouble() { return std::generate_canonical<double,std::numeric_limits<double>::digits>(generator); }
float  randomfloat()  { return std::generate_canonical<float, std::numeric_limits<float>::digits>(generator);  }

double randomdouble(double d) { return std::uniform_real_distribution<double> (0.0, d) (generator); }
float  randomfloat(float f)   { return std::uniform_real_distribution<float>  (0.0f,f) (generator); }

double randomdoubleRng(double d, double dd) { return std::uniform_real_distribution<double> (d, dd) (generator); }
float  randomfloatRng(float f, float ff)   { return std::uniform_real_distribution<float>  (f,ff) (generator); }

double randomGaussianDouble(double mean=0,    double stddev=1.0)  { return std::normal_distribution<double> (mean, stddev) (generator); }
float  randomGaussianFloat(float   mean=0.0f, float  stddev=1.0f) { return std::normal_distribution<float>  (mean, stddev) (generator); }

void random_writestate(std::ostream& sout) {sout << generator <<std::endl;}
void random_readstate(std::istream& sin) {sin >> generator;}

unsigned return_time() {return static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()); };
