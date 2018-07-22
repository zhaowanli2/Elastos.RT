/*
 * Copyright (c) 2012-2018 The Elastos Open Source Project
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

 * Add * accorded with breadwallet by zhangcl 791398105@qq.com

 * Copyright 2002-2018 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <fstream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <assert.h>

#include "BRCrypto.h"
#include "BTCKey.h"
#include "BTCBase58.h"

#define BIP32_HARD                  0x80000000

namespace Elastos {
    namespace ElaWallet {

        // Come form OpenSSL's source codes.
        void ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps)
        {
            if (pr != NULL)
                *pr = sig->r;
            if (ps != NULL)
                *ps = sig->s;
        }

        // Come form OpenSSL's source codes.
        int ECDSA_SIG_set0(ECDSA_SIG *sig, BIGNUM *r, BIGNUM *s)
        {
            if (r == NULL || s == NULL)
                return 0;
            BN_clear_free(sig->r);
            BN_clear_free(sig->s);
            sig->r = r;
            sig->s = s;
            return 1;
        }

        bool
        BTCKey::ECDSA65Sign_sha256(const CMBlock &privKey, const UInt256 &md, CMBlock &signedData, int nid) {
            bool out = false;
            if (0 == privKey.GetSize()) {
                return out;
            }
            EC_KEY *key = EC_KEY_new_by_curve_name(nid);
            if (key) {
                BIGNUM *privkeyIn = BN_bin2bn((const unsigned char *) (uint8_t *) privKey, (int) privKey.GetSize(),
                                              nullptr);
                if (privkeyIn) {
                    if (1 == EC_KEY_set_private_key(key, privkeyIn)) {
                        ECDSA_SIG *sig = ECDSA_do_sign((unsigned char *) &md, sizeof(md), key);
                        if (nullptr != sig) {
                            const BIGNUM *r = nullptr;
                            const BIGNUM *s = nullptr;
                            ECDSA_SIG_get0(sig, &r, &s);
                            int nBitsR = BN_num_bits(r);
                            int nBitsS = BN_num_bits(s);
                            if (nBitsR <= 256 && nBitsS <= 256) {
                                signedData.Resize(65);
                                signedData.Zero();
                                signedData[0] = 64;
                                uint8_t arrBIN[256] = {0};
                                size_t szLen = 0;
                                szLen = BN_bn2bin(r, arrBIN);
                                memcpy(signedData + 1 + (32 - szLen), arrBIN, szLen);
                                memset(arrBIN, 0, sizeof(arrBIN));
                                szLen = BN_bn2bin(s, arrBIN);
                                memcpy(signedData + 1 + 32 + (32 - szLen), arrBIN, szLen);
                                out = true;
                            }
                            ECDSA_SIG_free(sig);
                        }
                    }
                    BN_free(privkeyIn);
                }
                EC_KEY_free(key);
            }
            return out;
        }

        bool
        BTCKey::ECDSA65Verify_sha256(const CMBlock &pubKey, const UInt256 &md, const CMBlock &signedData, int nid) {
            bool out = false;
            if (0 == pubKey.GetSize() || 65 != signedData.GetSize()) {
                return out;
            }
            if (PublickeyIsValid(pubKey, nid)) {
                BIGNUM *_pubkey = nullptr;
                _pubkey = BN_bin2bn((const unsigned char *) (uint8_t *) pubKey, (int) pubKey.GetSize(), nullptr);
                EC_KEY *key = EC_KEY_new_by_curve_name(nid);
                if (nullptr != _pubkey && nullptr != key) {
                    const EC_GROUP *curve = EC_KEY_get0_group(key);
                    EC_POINT *ec_p = EC_POINT_bn2point(curve, _pubkey, nullptr, nullptr);
                    if (nullptr != ec_p) {
                        if (1 == EC_KEY_set_public_key(key, ec_p)) {
                            const uint8_t *p64 = &signedData[1];
                            ECDSA_SIG *sig = ECDSA_SIG_new();
                            if (nullptr != sig) {
                                BIGNUM *r = BN_bin2bn(&p64[0], 32, nullptr);
                                BIGNUM *s = BN_bin2bn(&p64[32], 32, nullptr);
                                ECDSA_SIG_set0(sig, r, s);
                                if (1 == ECDSA_do_verify((uint8_t *) &md, sizeof(md), sig, key)) {
                                    out = true;
                                }
                                ECDSA_SIG_free(sig);
                            }
                        }
                        EC_POINT_free(ec_p);
                    }
                    EC_KEY_free(key);
                    BN_free(_pubkey);
                } else {
                    if (nullptr != _pubkey) {
                        BN_free(_pubkey);
                    }
                    if (nullptr != key) {
                        EC_KEY_free(key);
                    }
                }
            }
            return out;
        }

        CMBlock BTCKey::getPubKeyFromPrivKey(const CMBlock &privKey, int nid) {
            CMBlock out;
            BIGNUM *_privkey = nullptr;
            if (32 != privKey.GetSize()) {
                return out;
            } else {
                _privkey = BN_bin2bn((const unsigned char *) (uint8_t *) privKey, (int) privKey.GetSize(), nullptr);
            }
            EC_KEY *key = EC_KEY_new_by_curve_name(nid);
            if (nullptr != _privkey && nullptr != key) {
                const EC_GROUP *curve = EC_KEY_get0_group(key);
                EC_POINT *_pubkey = EC_POINT_new(curve);
                if (_pubkey) {
                    if (1 == EC_POINT_mul(curve, _pubkey, _privkey, nullptr, nullptr, nullptr)) {
                        BIGNUM *__pubkey = EC_POINT_point2bn(curve, _pubkey, POINT_CONVERSION_COMPRESSED, nullptr,
                                                             nullptr);
                        if (nullptr != __pubkey) {
                            uint8_t arrBN[256] = {0};
                            int len = BN_bn2bin(__pubkey, arrBN);
                            if (0 < len) {
                                out.Resize(len);
                                memcpy(out, arrBN, (size_t) len);
                            }
                            BN_free(__pubkey);
                        }
                    }
                    EC_POINT_free(_pubkey);
                }
                BN_free(_privkey);
                EC_KEY_free(key);
            }
            return out;
        }

        bool BTCKey::PublickeyIsValid(const CMBlock &pubKey, int nid) {
            bool out = false;
            if (0 == pubKey.GetSize()) {
                return out;
            }
            EC_KEY *key = EC_KEY_new_by_curve_name(nid);
            if (nullptr != key) {
                BIGNUM *_pubkey = BN_bin2bn((const unsigned char *) (uint8_t *) pubKey, (int) pubKey.GetSize(),
                                            nullptr);
                if (nullptr != _pubkey) {
                    const EC_GROUP *curve = EC_KEY_get0_group(key);
                    EC_POINT *ec_p = EC_POINT_bn2point(curve, _pubkey, nullptr, nullptr);
                    if (nullptr != ec_p) {
                        if (1 == EC_KEY_set_public_key(key, ec_p)) {
                            if (1 == EC_KEY_check_key(key)) {
                                out = true;
                            }
                        }
                        EC_POINT_free(ec_p);
                    }
                    BN_free(_pubkey);
                }
                EC_KEY_free(key);
            }
            return out;
        }
    }
}
