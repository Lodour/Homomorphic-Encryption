#include <iostream>
#include "timer.h"
#include "random.h"
#include "he.h"
using namespace std;

void validate(const vector<long> &v1, const vector<long> &v2, const vector<long> &v3) {
  for (int i = 0; i < v1.size(); i++)
    assert(v1[i] + v2[i] == v3[i]);
}

int main() {
  // Declare parameters
  // See: https://github.com/shaih/HElib/blob/master/src/Test_General.cpp
  unsigned long p = 257;
  unsigned long r = 3;
  long L = 10;
  long c = 2;
  long w = 64;
  long d = 1;
  long k = 80;
  long s = 0;

  // Find a proper modulus
  long m = FindM(k, L, c, p, d, s, 0);

  // Initialize context
  FHEcontext context(static_cast<unsigned long>(m), p, r);
  buildModChain(context, L, c);

  // Generate keys
  FHESecKey secKey(context);
  const FHEPubKey &pubKey = secKey;
  secKey.GenSecKey(w);
  addSome1DMatrices(secKey);

  // Test
  Timer timer;
  auto x = Random::Vector(100);
  auto y = Random::Vector(100);

  // Test Raw
  {
    timer.tic();
    auto ex = EncryptVector::AsVector(x, pubKey);
    auto ey = EncryptVector::AsVector(y, pubKey);
    for (int i = 0; i < ex.size(); i++)
      ex[i] += ey[i];
    auto dx = DecryptVector::FromVector(ex, secKey);
    timer.toc();
    cout << "Raw: " << timer.ms() << " ms" << endl;
    validate(x, y, dx);
  }

  // Test Poly
  {
    timer.tic();
    auto ex = EncryptVector::AsPolynomial(x, pubKey);
    auto ey = EncryptVector::AsPolynomial(y, pubKey);
    ex += ey;
    auto dx = DecryptVector::FromPolynomial(ex, secKey);
    timer.toc();
    cout << "Polynomial: " << timer.ms() << " ms" << endl;
    validate(x, y, dx);
  }

  // Test Subfield
  {
    timer.tic();
    auto ex = EncryptVector::AsSubfield(x, pubKey, context);
    auto ey = EncryptVector::AsSubfield(y, pubKey, context);
    ex += ey;
    auto dx = DecryptVector::FromSubfield(ex, secKey, context);
    timer.toc();
    cout << "Subfield: " << timer.ms() << " ms" << endl;
    validate(x, y, dx);
  }

  return 0;
}