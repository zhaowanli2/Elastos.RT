// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Key.h"
#include "ByteStream.h"
#include "BigIntFormat.h"
#include "Utils.h"
#include "BTCKey.h"

#define ELA_IDCHAIN                 0xAD

namespace Elastos {
    namespace ElaWallet {

        bool Key::verifyByPublicKey(const std::string &publicKey, const std::string &message,
                                    const std::string &signature) {
            CMBlock signatureData = Utils::decodeHex(signature);

            UInt256 md;
            BRSHA256(&md, message.c_str(), message.size());

            return verifyByPublicKey(publicKey, md, signatureData);
        }

        bool Key::verifyByPublicKey(const std::string &publicKey, const UInt256 &messageDigest,
                                    const CMBlock &signature) {
            CMemBlock<char> mbcPubKey;
            mbcPubKey.SetMemFixed(publicKey.c_str(), publicKey.size() + 1);
            CMBlock pubKey = Str2Hex(mbcPubKey);

            return BTCKey::ECDSA65Verify_sha256(pubKey, messageDigest, signature, NID_X9_62_prime256v1);
        }

        std::string Key::publicKeyToIdAddress(BRKey *key) {
            std::string redeedScript = keyToRedeemScript(key, ELA_IDCHAIN);

            UInt168 hash = Utils::codeToProgramHash(redeedScript);

            std::string address = Utils::UInt168ToAddress(hash);

            return address;
        }

        std::string Key::keyToRedeemScript(BRKey *key, int signType) {
            uint64_t size = (key->compressed != 0) ? 33 : 65;

            ByteStream buff(size + 2);

            buff.put((uint8_t) size);

            buff.putBytes(key->pubKey, size);

            buff.put((uint8_t) signType);

            CMBlock script = buff.getBuffer();
            return  Utils::encodeHex(script, script.GetSize());
        }
    }
}
