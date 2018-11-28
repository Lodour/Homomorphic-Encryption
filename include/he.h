//
// Created by Roll on 2018-11-28.
//

#ifndef HOMOMORPHIC_ENCRYPTION_HE_H
#define HOMOMORPHIC_ENCRYPTION_HE_H

#include <vector>
#include "FHE.h"

class EncryptVector {
 public:
  static std::vector<Ctxt> AsVector(const std::vector<long> &vec, const FHEPubKey &pubKey);
  static Ctxt AsPolynomial(const std::vector<long> &vec, const FHEPubKey &pubKey);
  static Ctxt AsSubfield(const std::vector<long> &vec, const FHEPubKey &pubKey, const FHEcontext &context);
};

class DecryptVector {
 public:
  static std::vector<long> FromVector(const std::vector<Ctxt> &cipher, const FHESecKey &secKey);
  static std::vector<long> FromPolynomial(const Ctxt &cipher, const FHESecKey &secKey);
  static std::vector<long> FromSubfield(const Ctxt &cipher, const FHESecKey &secKey, const FHEcontext &context);
};

#endif //HOMOMORPHIC_ENCRYPTION_HE_H
