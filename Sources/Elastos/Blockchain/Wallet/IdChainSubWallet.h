
#ifndef __ELASTOS_WALLET_IDCHAINSUBWALLET_H__
#define __ELASTOS_WALLET_IDCHAINSUBWALLET_H__

#include "SidechainSubWallet.h"
#include "IIdChainSubWallet.h"

class IdChainSubWallet
    : public SidechainSubWallet
    , public IIdChainSubWallet
{
public:
    CAR_INTERFACE_DECL()

    IdChainSubWallet(
        /* [in] */ Elastos::ElaWallet::ISubWallet* spvSubWallet);

    CARAPI CreateIdTransaction(
        /* [in] */ const String& fromAddress,
        /* [in] */ const String& payloadJson,
        /* [in] */ const String& programJson,
        /* [in] */ const String& memo,
        /* [in] */ const String& remark,
        /* [out] */ String* result);
};

#endif // __ELASTOS_WALLET_IDCHAINSUBWALLET_H__
