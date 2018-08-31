
#include "SidechainSubWallet.h"

extern nlohmann::json ToJosnFromString(const char* str);

CAR_INTERFACE_IMPL(SidechainSubWallet, SubWallet, ISidechainSubWallet)
SidechainSubWallet::SidechainSubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet)
    : SubWallet(subWallet)
{}

ECode SidechainSubWallet::CreateWithdrawTransaction(
    /* [in] */ const String& fromAddress,
    /* [in] */ const String& toAddress,
    /* [in] */ Int64 amount,
    /* [in] */ const String& mainchainAccountsJson,
    /* [in] */ const String& mainchainAmountsJson,
    /* [in] */ const String& mainchainIndexsJson,
    /* [in] */ const String& memo,
    /* [in] */ const String& remark,
    /* [out] */ String* txidJson)
{
    VALIDATE_NOT_NULL(txidJson);
    Elastos::ElaWallet::ISidechainSubWallet* sidechainSubWallet = (Elastos::ElaWallet::ISidechainSubWallet*)(void *)mSpvSubWallet;
    assert(sidechainSubWallet != NULL);

    nlohmann::json json = sidechainSubWallet->CreateWithdrawTransaction(fromAddress.string(), toAddress.string(), amount,
                ToJosnFromString(mainchainAccountsJson.string()) , ToJosnFromString(mainchainAmountsJson.string()),
                ToJosnFromString(mainchainIndexsJson.string()), memo.string(), remark.string());
    *txidJson = String(json.dump().c_str());
    return NOERROR;
}

ECode SidechainSubWallet::GetGenesisAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    Elastos::ElaWallet::ISidechainSubWallet* sidechainSubWallet = (Elastos::ElaWallet::ISidechainSubWallet*)(void *)mSpvSubWallet;
    assert(sidechainSubWallet != NULL);

    std::string addr = sidechainSubWallet->GetGenesisAddress();
    *address = String(addr.c_str());
    return NOERROR;
}
