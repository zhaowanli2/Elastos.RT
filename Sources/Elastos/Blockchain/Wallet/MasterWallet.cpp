
#include "MasterWallet.h"
#include "MainchainSubWallet.h"
#include "IdChainSubWallet.h"

CAR_INTERFACE_IMPL(MasterWallet, Object, IMasterWallet)
MasterWallet::MasterWallet(
    /* [in] */ Elastos::ElaWallet::IMasterWallet* spvMasterWallet)
    : mSpvMasterWallet(spvMasterWallet)
{}

MasterWallet::~MasterWallet()
{
    std::map<ISubWallet*, Elastos::ElaWallet::ISubWallet*>::iterator it = mSubWalletsMap.begin();
    for (; it != mSubWalletsMap.end(); ++it) {
        ISubWallet* subWallet = it->first;
        if (subWallet) subWallet->Release();
    }
}

ECode MasterWallet::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    std::string _id = mSpvMasterWallet->GetId();
    *id = String(_id.c_str());
    return NOERROR;
}

ECode MasterWallet::GetAllSubWallets(
    /* [out, callee] */ ArrayOf<ISubWallet*>** subWallets)
{
    VALIDATE_NOT_NULL(subWallets);
    *subWallets = NULL;
    std::vector<Elastos::ElaWallet::ISubWallet *> array = mSpvMasterWallet->GetAllSubWallets();
    const Int32 length = array.size();
    if (length < 1) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<ISubWallet*> > _subWallets = ArrayOf<ISubWallet*>::Alloc(length);
    if (_subWallets == NULL) return E_OUT_OF_MEMORY;

    for (Int32 i = 0; i < length; i++) {
        AutoPtr<ISubWallet> subWallet = GetCreatedSubWallet(array[i]);
        if (subWallet == NULL) {
            subWallet = _CreateSubWallet(array[i]);
            if (subWallet == NULL) return E_OUT_OF_MEMORY;
        }

        _subWallets->Set(i, subWallet);
    }

    for (Int32 i = 0; i < length; i++) {
        AutoPtr<ISubWallet> subWallet = (*_subWallets)[i];
        std::map<ISubWallet*, Elastos::ElaWallet::ISubWallet*>::iterator it = mSubWalletsMap.find(subWallet.Get());
        if (it == mSubWalletsMap.end()) {
            mSubWalletsMap[subWallet.Get()] = array[i];
            subWallet->AddRef();
        }
    }

    *subWallets = _subWallets;
    (*subWallets)->AddRef();
    return NOERROR;
}

ECode MasterWallet::CreateSubWallet(
    /* [in] */ const String& chainID,
    /* [in] */ const String& payPassword,
    /* [in] */ Boolean singleAddress,
    /* [in] */ Int64 feePerKb,
    /* [out] */ ISubWallet** subWallet)
{
    VALIDATE_NOT_NULL(subWallet);
    Elastos::ElaWallet::ISubWallet* _subWallet = mSpvMasterWallet->CreateSubWallet(chainID.string(), payPassword.string(),
                singleAddress, feePerKb);

    AutoPtr<ISubWallet> wallet = GetCreatedSubWallet(_subWallet);
    if (wallet == NULL) {
        wallet = _CreateSubWallet(_subWallet);
        if (wallet == NULL) return E_OUT_OF_MEMORY;
    }

    *subWallet = wallet;
    mSubWalletsMap[wallet.Get()] = _subWallet;
    (*subWallet)->AddRef();

    return NOERROR;
}

ECode MasterWallet::RecoverSubWallet(
    /* [in] */ const String& chainID,
    /* [in] */ const String& payPassword,
    /* [in] */ Boolean singleAddress,
    /* [in] */ Int32 limitGap,
    /* [in] */ Int64 feePerKb,
    /* [out] */ ISubWallet** subWallet)
{
    VALIDATE_NOT_NULL(subWallet);
    assert(0 && "Need the spv's support.");

    Elastos::ElaWallet::ISubWallet* _subWallet = mSpvMasterWallet->RecoverSubWallet(chainID.string(), payPassword.string()
                    , singleAddress, limitGap, feePerKb);
    AutoPtr<ISubWallet> wallet = _CreateSubWallet(_subWallet);
    if (wallet == NULL) return E_OUT_OF_MEMORY;

    *subWallet = wallet;
    (*subWallet)->AddRef();
    return NOERROR;
}

ECode MasterWallet::DestroyWallet(
    /* [in] */ ISubWallet *wallet)
{
    if (wallet == NULL) {
        return E_INVALID_ARGUMENT;
    }

    std::map<ISubWallet*, Elastos::ElaWallet::ISubWallet*>::iterator iter = mSubWalletsMap.find(wallet);
    if (iter != mSubWalletsMap.end()) {
        mSpvMasterWallet->DestroyWallet(iter->second);
        mSubWalletsMap.erase(iter);
        wallet->Release();
    }
    else {
        assert(0 && "There is an error when DestroyWallet.");
    }

    return NOERROR;
}

ECode MasterWallet::GetPublicKey(
    /* [out] */ String* publicKey)
{
    VALIDATE_NOT_NULL(publicKey);
    std::string key = mSpvMasterWallet->GetPublicKey();
    *publicKey = String(key.c_str());
    return NOERROR;
}

ECode MasterWallet::Sign(
    /* [in] */ const String& message,
    /* [in] */ const String& payPassword,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    std::string spvResult = mSpvMasterWallet->Sign(message.string(), payPassword.string());
    *result = String(spvResult.c_str());
    return NOERROR;
}

ECode MasterWallet::CheckSign(
    /* [in] */ const String& publicKey,
    /* [in] */ const String& message,
    /* [in] */ const String& signature,
    /* [out] */ String* resultJson)
{
    VALIDATE_NOT_NULL(resultJson);
    nlohmann::json jsonVal = mSpvMasterWallet->CheckSign(publicKey.string(), message.string(), signature.string());

    *resultJson = String(jsonVal.dump().c_str());
    return NOERROR;
}

ECode MasterWallet::IsAddressValid(
    /* [in] */ const String& address,
    /* [out] */ Boolean* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = mSpvMasterWallet->IsAddressValid(address.string());
    return NOERROR;
}

ECode MasterWallet::GetSupportedChains(
    /* [out, callee] */ ArrayOf<String>** supportedChains)
{
    VALIDATE_NOT_NULL(supportedChains);
    *supportedChains = NULL;

    std::vector<std::string> chains = mSpvMasterWallet->GetSupportedChains();
    const int length = chains.size();
    if (length < 1) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(length);
    if (result == NULL) return E_OUT_OF_MEMORY;

    for (int i = 0; i < length; i++) {
        result->Set(i, String(chains[i].c_str()));
    }

    *supportedChains = result.Get();
    (*supportedChains)->AddRef();
    return NOERROR;
}

ECode MasterWallet::ChangePassword(
    /* [in] */ const String& oldPassword,
    /* [in] */ const String& newPassword)
{
    mSpvMasterWallet->ChangePassword(oldPassword.string(), newPassword.string());
    return NOERROR;
}

Elastos::ElaWallet::IMasterWallet* MasterWallet::GetSpvMasterWallet()
{
    return mSpvMasterWallet;
}

ISubWallet* MasterWallet::GetCreatedSubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet)
{
    std::map<ISubWallet*, Elastos::ElaWallet::ISubWallet*>::iterator it = mSubWalletsMap.begin();
    for (; it != mSubWalletsMap.end(); ++it) {
        if (it->second == subWallet) {
            return it->first;
        }
    }
    return NULL;
}

ISubWallet* MasterWallet::_CreateSubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* subWallet)
{
    assert(subWallet != NULL);
    ISubWallet* wallet = NULL;
    std::string id = subWallet->GetChainId();
    if (id.compare("ELA") == 0) {
        IMainchainSubWallet* main = new MainchainSubWallet(subWallet);
        wallet = ISubWallet::Probe(main);
        assert(IMainchainSubWallet::Probe(wallet) != NULL);
    }
    else if (id.compare("IdChain") == 0) {
        ISidechainSubWallet* side = new IdChainSubWallet(subWallet);
        wallet = ISubWallet::Probe(side);
        assert(ISidechainSubWallet::Probe(wallet) != NULL);
        assert(IIdChainSubWallet::Probe(wallet) != NULL);
    }
    else {
        assert(0 && "TODO: Not support this chain id.");
    }

    return wallet;
}
