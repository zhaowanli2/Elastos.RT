// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_UTILS_H__
#define __ELASTOS_SDK_UTILS_H__

#include <string>
#include <assert.h>
#include <random>
#include <functional>

#include "BRInt.h"
#include "CMemBlock.h"

namespace Elastos {
    namespace ElaWallet {

        class Utils {
        public:
            static UInt128 generateRandomSeed();

            static inline uint8_t getRandomByte() {
                std::random_device rd;
                std::mt19937_64 gen(rd());
                std::uniform_int_distribution<> dis(0, 255);
                auto dice = std::bind(dis, gen);
                return dice();
            }

            static std::string encodeHex(const CMBlock &in);

            static void encodeHex(char *target, size_t targetLen, const uint8_t *source, size_t sourceLen);

            static std::string encodeHex(const uint8_t *hex, size_t hexLen);

            static size_t encodeHexLength(size_t byteArrayLen);

            static std::string encodeHexCreate(size_t *targetLen, uint8_t *source, size_t sourceLen);

            static CMBlock decodeHex(const std::string &s);

            static void decodeHex(uint8_t *target, size_t targetLen, const char *source, size_t sourceLen);

            static size_t decodeHexLength(size_t stringLen);

            static uint8_t *decodeHexCreate(size_t *targetLen, char *source, size_t sourceLen);

            static std::string convertToString(const CMBlock &data) {
                char p[data.GetSize()];
                memcpy(p, data, data.GetSize());
                std::string ret(p, data.GetSize());
                return ret;
            }

            static CMBlock convertToMemBlock(const std::string &str) {
                CMBlock result(str.size());
                memcpy(result, str.c_str(), str.size());
                return result;
            }

            static std::string UInt168ToAddress(const UInt168 &u);

            static uint32_t getAddressTypeBySignType(const int signType);

            static UInt168 codeToProgramHash(const std::string &redeemScript);

            static UInt168 codeToProgramHash(const CMBlock &redeemScript);
        };
    }
}

#endif //__ELASTOS_SDK_UTILS_H__
