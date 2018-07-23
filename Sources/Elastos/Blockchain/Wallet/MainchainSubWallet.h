
#ifndef __ELASTOS_WALLET_MAINCHAINSUBWALLET_H__
#define __ELASTOS_WALLET_MAINCHAINSUBWALLET_H__

#include "SubWallet.h"
#include "IMainchainSubWallet.h"

class MainchainSubWallet
    : public SubWallet
    , public IMainchainSubWallet
{
public:
    CAR_INTERFACE_DECL()

    MainchainSubWallet(
        /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet);

    CARAPI CreateDepositTransaction(
        /* [in] */ const String& fromAddress,
        /* [in] */ const String& toAddress,
        /* [in] */ Int64 amount,
        /* [in] */ const String& sidechainAccountsJson,
        /* [in] */ const String& sidechainAmountsJson,
        /* [in] */ const String& sidechainIndexsJson,
        /* [in] */ const String& memo,
        /* [in] */ const String& remark,
        /* [out] */ String* txidJson);
};

#endif // __ELASTOS_WALLET_MAINCHAINSUBWALLET_H__
