
#include "MainchainSubWallet.h"

#include <android/log.h>
#define TAG "Elastos_Droid_Wallet"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)

extern String ToStringFromJson(nlohmann::json jsonValue);
extern nlohmann::json ToJosnFromString(const char* str);

CAR_INTERFACE_IMPL(MainchainSubWallet, SubWallet, IMainchainSubWallet)
MainchainSubWallet::MainchainSubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet)
    : SubWallet(subWallet)
{}

ECode MainchainSubWallet::CreateDepositTransaction(
    /* [in] */ const String& fromAddress,
    /* [in] */ const String& toAddress,
    /* [in] */ Int64 amount,
    /* [in] */ const String& sidechainAccountsJson,
    /* [in] */ const String& sidechainAmountsJson,
    /* [in] */ const String& sidechainIndexsJson,
    /* [in] */ const String& memo,
    /* [in] */ const String& remark,
    /* [out] */ String* txidJson)
{
    VALIDATE_NOT_NULL(txidJson);
    Elastos::ElaWallet::IMainchainSubWallet* mainchainSubWallet = (Elastos::ElaWallet::IMainchainSubWallet*)(void*)mSpvSubWallet;
    assert(mainchainSubWallet != NULL);

    String fAddress(fromAddress);
    if (fAddress.IsNull()) {
        fAddress = String("");
    }

    nlohmann::json json = mainchainSubWallet->CreateDepositTransaction(fAddress.string(), toAddress.string(), amount
            , ToJosnFromString(sidechainAccountsJson.string()), ToJosnFromString(sidechainAmountsJson.string()),
            ToJosnFromString(sidechainIndexsJson.string()), memo.string(), remark.string());
    *txidJson = ToStringFromJson(json);
    return NOERROR;
}
