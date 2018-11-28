//
// Created by Roll on 2018-11-28.
//

#include "he.h"
#include <algorithm>
#include "EncryptedArray.h"

std::vector<Ctxt> EncryptVector::AsVector(const std::vector<long> &vec, const FHEPubKey &pubKey) {
  std::vector<Ctxt> cipher;
  auto encrypt_one = [&](const int &x) { return EncryptVector::EncryptZZX(NTL::to_ZZX(x), pubKey); };
  std::transform(vec.begin(), vec.end(), std::back_inserter(cipher), encrypt_one);
  return cipher;
}

Ctxt EncryptVector::AsPolynomial(const std::vector<long> &vec, const FHEPubKey &pubKey) {
  // Packing an vector into a polynomial
  NTL::ZZX polynomial;
  polynomial.SetLength(vec.size());
  std::for_each(vec.begin(), vec.end(), [&, i = 0](int x) mutable { SetCoeff(polynomial, i++, x); });

  // Encrypt the packed polynomial
  return EncryptVector::EncryptZZX(polynomial, pubKey);
}

Ctxt EncryptVector::AsSubfield(const std::vector<long> &vec, const FHEPubKey &pubKey, const FHEcontext &context) {
  // Generate underlying helper
  auto F = context.alMod.getFactorsOverZZ()[0];
  EncryptedArray ea(context, F);

  // Packing an vector into a subfield
  NTL::ZZX subfield;
  ea.encode(subfield, vec);

  // Encrypt the packed subfield
  return EncryptVector::EncryptZZX(subfield, pubKey);
}

Ctxt EncryptVector::EncryptZZX(const NTL::ZZX &ptxt, const FHEPubKey &pubKey) {
  Ctxt cipher(pubKey);
  pubKey.Encrypt(cipher, ptxt);
  return cipher;
}

std::vector<long> DecryptVector::FromVector(const std::vector<Ctxt> &cipher, const FHESecKey &secKey) {
  std::vector<long> plain;
  auto decrypt_one = [&](const Ctxt &x) { return NTL::conv<int>(DecryptVector::DecryptZZX(x, secKey)[0]); };
  std::transform(cipher.begin(), cipher.end(), std::back_inserter(plain), decrypt_one);
  return plain;
}

std::vector<long> DecryptVector::FromPolynomial(const Ctxt &cipher, const FHESecKey &secKey) {
  // Decrypt the packed polynomial
  auto polynomial = DecryptVector::DecryptZZX(cipher, secKey);

  // Unpack the polynomial
  std::vector<long> vec;
  auto get_coefficient = [&, i = 0]() mutable { return conv<int>(polynomial[i++]); };
  std::generate_n(std::back_inserter(vec), polynomial.rep.length(), get_coefficient);
  return vec;
}

std::vector<long> DecryptVector::FromSubfield(const Ctxt &cipher, const FHESecKey &secKey, const FHEcontext &context) {
  // Generate underlying helper
  auto F = context.alMod.getFactorsOverZZ()[0];
  EncryptedArray ea(context, F);

  // Decrypt the packed subfield
  auto subfield = DecryptVector::DecryptZZX(cipher, secKey);

  // Unpack the subfield
  std::vector<long> vec;
  ea.decode(vec, subfield);
  return vec;
}

NTL::ZZX DecryptVector::DecryptZZX(const Ctxt &cipher, const FHESecKey &secKey) {
  NTL::ZZX plain;
  secKey.Decrypt(plain, cipher);
  return plain;
}
