#include <iostream>
#include "EncryptedArray.h"
#include "timer.h"
#include "random.h"
#include "he.h"
using namespace std;

void validate_sum(const vector<long> &v1, const vector<long> &v2, const vector<long> &v3, long modular) {
  for (int i = 0; i < v1.size(); i++)
    assert((v1[i] + v2[i]) % modular == v3[i]);
}

void validate_inner_product(const vector<long> &v1, const vector<long> &v2, long prod, long modular) {
  long expected_prod = 0;
  for (int i = 0; i < v1.size(); i++)
    expected_prod = (expected_prod + v1[i] * v2[i]) % modular;
  assert(expected_prod == prod);
}

int main() {
  // Declare parameters
  // See: https://github.com/shaih/HElib/blob/master/src/Test_General.cpp

  // `p` and `r` define a polynomial ring - Z_{p^r}[x]
  // `p` and `d` define the plaintext space - a finite field, $\mathbb{F}_{p^d}$
  unsigned long p = 257;
  unsigned long r = 3;
  long d = 1;

  // The following 4 parameters are mainly used to find a modulus phi(`m`)
  // `c` defines # of columns in the key-switching matrices
  // `k` defines # of bits security
  // `s` defines minimum # of slots
  // `L` defines # of levels in the modulus chain
  long c = 2;
  long k = 80;
  long s = 0;
  long L = 10;

  // w is used to sample a Hamming-weight polynomial for generating a secret key
  long w = 64;

  // Find a proper modulus, the m'th cyclotomic ring, $\Phi_m(x)$
  auto m = static_cast<unsigned long>(FindM(k, L, c, p, d, s, 0));

  // Initialize context
  FHEcontext context(m, p, r);
  buildModChain(context, L, c);

  // Generate keys
  FHESecKey secKey(context);
  const FHEPubKey &pubKey = secKey;
  secKey.GenSecKey(w);
  addSome1DMatrices(secKey);

  // Test
  Timer timer;
  const int N = 100;
  auto x = Random::Vector(N);
  auto y = Random::Vector(N);

  // Compute modular
  long modular = 1;
  for (int i = 0; i < r; i++)
    modular *= p;

  // Test Inner product
  {
    // Raw
    {
      timer.tic();

      auto F = context.alMod.getFactorsOverZZ()[0];
      EncryptedArray ea(context, F);

      auto ex = EncryptVector::AsVector(x, pubKey);
      auto ey = EncryptVector::AsVector(y, pubKey);
      for (int i = 0; i < ex.size(); i++)
        ex[i] *= ey[i];
      for (int i = 1; i < ex.size(); i++)
        ex[0] += ex[i];
      auto dx = DecryptVector::FromVector(ex, secKey);

      timer.toc();
      cout << "Raw        Inner Product: " << timer.ms() << " ms" << endl;
      validate_inner_product(x, y, dx[0], modular);
    }

    // Poly
    {
      timer.tic();

      auto ry = vector<long>(y.rbegin(), y.rend());
      auto ex = EncryptVector::AsPolynomial(x, pubKey);
      auto ey = EncryptVector::AsPolynomial(ry, pubKey);
      ex *= ey;
      auto dx = DecryptVector::FromPolynomial(ex, secKey);

      timer.toc();
      cout << "Polynomial Inner Product: " << timer.ms() << " ms" << endl;
      validate_inner_product(x, y, conv<long>(dx[N - 1]), modular);
    }

    // Subfield
    {
      timer.tic();

      auto F = context.alMod.getFactorsOverZZ()[0];
      EncryptedArray ea(context, F);

      auto ex = EncryptVector::AsSubfield(x, pubKey, context, ea);
      auto ey = EncryptVector::AsSubfield(y, pubKey, context, ea);
      ex *= ey;
      totalSums(ea, ex);
      auto dx = DecryptVector::FromSubfield(ex, secKey, context, ea);

      timer.toc();
      cout << "Subfield   Inner Product: " << timer.ms() << " ms" << endl;
      validate_inner_product(x, y, conv<long>(dx[0]), modular);
    }
  }

  // Test Vector Sum
  {
    // Raw
    {
      timer.tic();
      auto ex = EncryptVector::AsVector(x, pubKey);
      auto ey = EncryptVector::AsVector(y, pubKey);
      for (int i = 0; i < ex.size(); i++)
        ex[i] += ey[i];
      auto dx = DecryptVector::FromVector(ex, secKey);
      timer.toc();
      cout << "Raw        Vector Sum: " << timer.ms() << " ms" << endl;
      validate_sum(x, y, dx, modular);
    }

    // Poly
    {
      timer.tic();
      auto ex = EncryptVector::AsPolynomial(x, pubKey);
      auto ey = EncryptVector::AsPolynomial(y, pubKey);
      ex += ey;
      auto dx = DecryptVector::FromPolynomial(ex, secKey);
      timer.toc();
      cout << "Polynomial Vector Sum: " << timer.ms() << " ms" << endl;
      validate_sum(x, y, dx, modular);
    }

    // Subfield
    {
      timer.tic();
      auto F = context.alMod.getFactorsOverZZ()[0];
      EncryptedArray ea(context, F);

      auto ex = EncryptVector::AsSubfield(x, pubKey, context, ea);
      auto ey = EncryptVector::AsSubfield(y, pubKey, context, ea);
      ex += ey;
      auto dx = DecryptVector::FromSubfield(ex, secKey, context, ea);
      timer.toc();
      cout << "Subfield   Vector Sum: " << timer.ms() << " ms" << endl;
      validate_sum(x, y, dx, modular);
    }
  }

  return 0;
}