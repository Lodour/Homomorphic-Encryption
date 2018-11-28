//
// Created by Roll on 2018-11-28.
//

#include "he.h"
#include <algorithm>
#include "EncryptedArray.h"

std::vector<Ctxt> EncryptVector::AsVector(const std::vector<long> &vec, const FHEPubKey &pubKey) {
  // Prepare cipher space
  std::vector<Ctxt> cipher;
  cipher.reserve(vec.size());

  // Encrypt by element
  Ctxt ct(pubKey);
  for (auto &x : vec) {
    pubKey.Encrypt(ct, NTL::to_ZZX(x));
    cipher.push_back(ct);
  }

  return cipher;
}

std::vector<long> DecryptVector::FromVector(const std::vector<Ctxt> &cipher, const FHESecKey &secKey) {
  // Prepare plain space
  std::vector<long> plain;
  plain.reserve(cipher.size());

  // Decrypt by element
  NTL::ZZX p;
  for (auto &x : cipher) {
    secKey.Decrypt(p, x);
    plain.push_back(conv<long>(p[0]));
  }

  return plain;
}

Ctxt EncryptVector::AsPolynomial(const std::vector<long> &vec, const FHEPubKey &pubKey) {
  // Prepare the underlying polynomial
  NTL::ZZX polynomial;
  polynomial.SetLength(vec.size());

  // Packing vec into the polynomial
  for (int i = 0; i < vec.size(); i++)
    NTL::SetCoeff(polynomial, i, vec[i]);

  // Encrypt the packed polynomial
  Ctxt cipher(pubKey);
  pubKey.Encrypt(cipher, polynomial);
  return cipher;
}

std::vector<long> DecryptVector::FromPolynomial(const Ctxt &cipher, const FHESecKey &secKey) {
  // Decrypt the packed polynomial
  NTL::ZZX polynomial;
  secKey.Decrypt(polynomial, cipher);

  // Prepare plain space
  std::vector<long> plain;
  plain.reserve(static_cast<std::size_t>(polynomial.rep.length()));

  // Unpack the polynomial
  for (int i = 0; i < polynomial.rep.length(); i++)
    plain.push_back(conv<long>(polynomial[i]));

  return plain;
}

Ctxt EncryptVector::AsSubfield(const std::vector<long> &vec, const FHEPubKey &pubKey, const FHEcontext &context) {
  // Generate underlying helper
  auto F = context.alMod.getFactorsOverZZ()[0];
  EncryptedArray ea(context, F);

  // Align
  std::vector<long> aligned(static_cast<std::size_t>(ea.size()));
  std::move(vec.begin(), vec.end(), aligned.begin());

  // Encrypt & pack an vector into a subfield
  Ctxt cipher(pubKey);
  ea.encrypt(cipher, pubKey, aligned);

  return cipher;
}

std::vector<long> DecryptVector::FromSubfield(const Ctxt &cipher, const FHESecKey &secKey, const FHEcontext &context) {
  // Generate underlying helper
  auto F = context.alMod.getFactorsOverZZ()[0];
  EncryptedArray ea(context, F);

  // Decrypt & unpack the packed subfield
  std::vector<long> plain(static_cast<std::size_t>(ea.size()));
  ea.decrypt(cipher, secKey, plain);

  return plain;
}
