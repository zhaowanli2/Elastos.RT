// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include <algorithm>
#include <iterator>

#include "BRCrypto.h"
#include "Utils.h"
#include "BTCBase58.h"
#include "BRAddress.h"

namespace Elastos {
    namespace ElaWallet {
        void Utils::decodeHex(uint8_t *target, size_t targetLen, const char *source, size_t sourceLen) {
            if (2 * targetLen < sourceLen || 0 != sourceLen % 2) {
                return;
            }

            for (int i = 0; i < targetLen; i++) {
                target[i] = (uint8_t) ((_hexu(source[2 * i]) << 4) | _hexu(source[(2 * i) + 1]));
            }
        }

        size_t Utils::decodeHexLength(size_t stringLen) {
            if (0 != stringLen % 2)
                throw std::logic_error("Invalid string length.");
            return stringLen / 2;
        }

        uint8_t *Utils::decodeHexCreate(size_t *targetLen, char *source, size_t sourceLen) {
            size_t length = decodeHexLength(sourceLen);
            if (nullptr != targetLen) *targetLen = length;
            uint8_t *target = (uint8_t *) malloc(length);
            decodeHex(target, length, source, sourceLen);
            return target;
        }

        void Utils::encodeHex(char *target, size_t targetLen, const uint8_t *source, size_t sourceLen) {
            assert (targetLen >= 2 * sourceLen + 1);

            for (int i = 0; i < sourceLen; i++) {
                target[2 * i] = (uint8_t) _hexc (source[i] >> 4);
                target[2 * i + 1] = (uint8_t) _hexc (source[i]);
            }
            target[2 * sourceLen] = '\0';
        }

        std::string Utils::encodeHex(const uint8_t *hex, size_t hexLen) {
            if (hexLen == 0) {
                return std::string();
            }

            char buf[hexLen * 2 + 1];
            encodeHex(buf, hexLen * 2 + 1, hex, hexLen);
            std::string str(buf);

            return str;
        }

        size_t Utils::encodeHexLength(size_t byteArrayLen) {
            return 2 * byteArrayLen + 1;
        }

        std::string Utils::encodeHexCreate(size_t *targetLen, uint8_t *source, size_t sourceLen) {
            size_t length = encodeHexLength(sourceLen);
            if (nullptr != targetLen) *targetLen = length;
            char target[length];
            encodeHex(target, length, source, sourceLen);
            return std::string(target, length - 1);
        }

        UInt128 Utils::generateRandomSeed() {
            UInt128 result;
            for (size_t i = 0; i < sizeof(result); ++i) {
                result.u8[i] = getRandomByte();
            }
            return result;
        }

        std::string Utils::UInt168ToAddress(const UInt168 &u) {
            UInt256 hash = UINT256_ZERO;
            size_t uSize = sizeof(UInt168);

            BRSHA256_2(&hash, u.u8, uSize);

            size_t dataLen = uSize + 4;
            uint8_t data[sizeof(UInt168) + 4] = {0};
            memcpy(data, u.u8, uSize);
            memcpy(data + uSize, hash.u8, 4);

            return BTCBase58::EncodeBase58(data, dataLen);
        }

        uint32_t Utils::getAddressTypeBySignType(const int signType) {
            if (signType == ELA_STANDARD) {
                return ELA_STAND_ADDRESS;
            } else if (signType == ELA_MULTISIG) {
                return ELA_MULTISIG_ADDRESS;
            } else if (signType == ELA_CROSSCHAIN) {
                return ELA_CROSSCHAIN_ADDRESS;
            } else if (signType == ELA_IDCHAIN) {
                return ELA_IDCHAIN_ADDRESS;
            } else if (signType == ELA_DESTROY) {
                return ELA_DESTROY_ADDRESS;
            } else {
                throw std::logic_error("error signType.");
            }
        }

        UInt168 Utils::codeToProgramHash(const std::string &redeemScript) {
            size_t scriptLen = 0;
            uint8_t *script = Utils::decodeHexCreate(&scriptLen, (char *)(redeemScript.c_str()), redeemScript.size());
            CMBlock redeedScript;
            redeedScript.SetMem(script, scriptLen);

            return codeToProgramHash(redeedScript);
        }

        UInt168 Utils::codeToProgramHash(const CMBlock &redeemScript) {
            UInt160 hash = UINT160_ZERO;
            size_t len = redeemScript.GetSize();
            BRHash160(&hash, redeemScript, len);
            int signType = redeemScript[len - 1];
            uint32_t addressType = Utils::getAddressTypeBySignType(signType);

            UInt168 uInt168 = UINT168_ZERO;
            memcpy(&uInt168.u8[1], &hash.u8[0], sizeof(hash.u8));
            uInt168.u8[0] = addressType;
            return uInt168;
        }

        std::string Utils::encodeHex(const CMBlock &in) {
            return encodeHexCreate(nullptr, in, in.GetSize());
        }

        CMBlock Utils::decodeHex(const std::string &s) {
            size_t dataLen = 0;
            char *str = const_cast<char *>(s.c_str());
            uint8_t *data = decodeHexCreate(&dataLen, str, strlen(str));

            CMBlock ret;
            ret.SetMem(data, dataLen);

            return ret;
        }
    }
}
