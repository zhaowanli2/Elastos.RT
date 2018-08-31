
#include "IdChainSubWallet.h"

extern nlohmann::json ToJosnFromString(const char* str);

CAR_INTERFACE_IMPL(IdChainSubWallet, SidechainSubWallet, IIdChainSubWallet)
IdChainSubWallet::IdChainSubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet)
    : SidechainSubWallet(subWallet)
{}

ECode IdChainSubWallet::CreateIdTransaction(
    /* [in] */ const String& fromAddress,
    /* [in] */ const String& payloadJson,
    /* [in] */ const String& programJson,
    /* [in] */ const String& memo,
    /* [in] */ const String& remark,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    Elastos::ElaWallet::IIdChainSubWallet* idchainSubWallet = (Elastos::ElaWallet::IIdChainSubWallet*)(void*)mSpvSubWallet;
    assert(idchainSubWallet != NULL);

    nlohmann::json json = idchainSubWallet->CreateIdTransaction(fromAddress.string(), ToJosnFromString(payloadJson.string())
            , ToJosnFromString(programJson.string()), memo.string(), remark.string());
    *result = String(json.dump().c_str());
    return NOERROR;
}
