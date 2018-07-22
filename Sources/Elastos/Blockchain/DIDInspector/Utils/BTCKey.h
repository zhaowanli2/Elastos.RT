// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_BTCKEY_H__
#define __ELASTOS_SDK_BTCKEY_H__

//Add * accorded with breadwallet by zhangcl 791398105@qq.com

#include <string>
#include <openssl/obj_mac.h>

#include "BRInt.h"
#include "CMemBlock.h"

#define BIP32_SEED_KEY "ELA SEED"

namespace Elastos {
    namespace ElaWallet {
        class BTCKey {
        public:

            /** Signatures data for ECDSA.
               *  \param  privKey CMemBlock for Sign's PrivateKey.
             *  \param  md UInt256 to be signed.
             *     \param  signedData CMemBlock for returned SignedData.
             *     \param  nid int for style of ECDSA.
             *  \return true on success and false on failure.
             */
            static bool
            ECDSA65Sign_sha256(const CMBlock &privKey, const UInt256 &md, CMBlock &signedData,
                               int nid = NID_secp256k1);

            /** Verifys signature's for ECDSA.
              *  \param  pubKey CMemBlock for verify's PublicKey.
              *  \param  md UInt256 for PlainText.
              *     \param  signedData CMemBlock for SignedData.
              *     \param  nid int for style of ECDSA.
              *  \return true on success and false on failure.
              */
            static bool
            ECDSA65Verify_sha256(const CMBlock &pubKey, const UInt256 &md, const CMBlock &signedData,
                                 int nid = NID_secp256k1);

            /** Get PublicKey from corresponding to PrivateKey for ECDSA.
              *  \param  privKey CMemBlock vary from getMasterPrivkey or generateKey for ECDSA.
              *     \param  nid int for style of ECDSA.
              *  \return PublicKey CMemBlock.
              */
            static CMBlock getPubKeyFromPrivKey(const CMBlock &privKey, int nid = NID_secp256k1);

            /** Check PublicKey validation for ECDSA.
              *  \param  pubKey CMemBlock for PrivateKey.
              *     \param  nid int for style of ECDSA.
              *  \return true on success and false on failure.
              */
            static bool PublickeyIsValid(const CMBlock &pubKey, int nid = NID_secp256k1);
        };
    }
}


#endif //__ELASTOS_SDK_BTCKEY_H__
