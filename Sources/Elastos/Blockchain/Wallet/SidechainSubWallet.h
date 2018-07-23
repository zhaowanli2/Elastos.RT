
#ifndef __ELASTOS_WALLET_SIDECHAINSUBWALLET_H__
#define __ELASTOS_WALLET_SIDECHAINSUBWALLET_H__

#include "SubWallet.h"
#include "ISidechainSubWallet.h"

class SidechainSubWallet
    : public SubWallet
    , public ISidechainSubWallet
{
public:
    CAR_INTERFACE_DECL()

    SidechainSubWallet(
        /* [in] */ Elastos::ElaWallet::ISubWallet* spvSubWallet);

    CARAPI CreateWithdrawTransaction(
        /* [in] */ const String& fromAddress,
        /* [in] */ const String& toAddress,
        /* [in] */ Int64 amount,
        /* [in] */ const String& mainchainAccountsJson,
        /* [in] */ const String& mainchainAmountsJson,
        /* [in] */ const String& mainchainIndexsJson,
        /* [in] */ const String& memo,
        /* [in] */ const String& remark,
        /* [out] */ String* txidJson);

    CARAPI GetGenesisAddress(
        /* [out] */ String* address);
};

#endif // __ELASTOS_WALLET_SIDECHAINSUBWALLET_H__
