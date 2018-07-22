// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_KEY_H__
#define __ELASTOS_SDK_KEY_H__

#include <string>
#include <openssl/obj_mac.h>

#include "BRKey.h"
#include "BRAddress.h"
#include "CMemBlock.h"

namespace Elastos {
    namespace ElaWallet {

        class Key {
        public:
            static std::string publicKeyToIdAddress(BRKey *key);

            static bool verifyByPublicKey(const std::string &publicKey, const std::string &message,
                                          const std::string &signature);

            static bool verifyByPublicKey(const std::string &publicKey, const UInt256 &messageDigest,
                                          const CMBlock &signature);

        private:
            static std::string keyToRedeemScript(BRKey *key, int signType);
        };

    }
}

#endif //__ELASTOS_SDK_KEY_H__
