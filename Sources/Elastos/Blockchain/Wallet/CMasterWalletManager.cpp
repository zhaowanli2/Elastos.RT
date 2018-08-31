
#include "CMasterWalletManager.h"
#include "MasterWallet.h"

extern nlohmann::json ToJosnFromString(const char* str);

CAR_OBJECT_IMPL(CMasterWalletManager)
CAR_INTERFACE_IMPL(CMasterWalletManager, Object, IMasterWalletManager);

CMasterWalletManager::CMasterWalletManager()
    : mSpvMasterWalletMgr(NULL)
{}

CMasterWalletManager::~CMasterWalletManager()
{
    if (mSpvMasterWalletMgr) {
        delete mSpvMasterWalletMgr;
    }

    std::map<const char*, IMasterWallet*, compare_string>::iterator it = mMasterWalletsMap.begin();
    for (; it != mMasterWalletsMap.end(); ++it) {
        IMasterWallet* masterWallet = it->second;
        if (masterWallet) delete ((MasterWallet*)masterWallet);
    }
}

ECode CMasterWalletManager::constructor(
    /* [in] */ const String& rootPath)
{
    mSpvMasterWalletMgr = new Elastos::ElaWallet::MasterWalletManager(rootPath.string());
    if (mSpvMasterWalletMgr == NULL) return E_OUT_OF_MEMORY;

    return NOERROR;
}

ECode CMasterWalletManager::CreateMasterWallet(
    /* [in] */ const String& masterWalletId,
    /* [in] */ const String& mnemonic,
    /* [in] */ const String& phrasePassword,
    /* [in] */ const String& payPassword,
    /* [in] */ const String& language,
    /* [out] */ IMasterWallet** masterWallet)
{
    VALIDATE_NOT_NULL(masterWallet);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    *masterWallet = NULL;
    std::map<const char*, IMasterWallet*, compare_string>::iterator iter = mMasterWalletsMap.find(masterWalletId.string());
    if (iter != mMasterWalletsMap.end()) {
        *masterWallet = iter->second;
        (*masterWallet)->AddRef();
        return NOERROR;
    }

    Elastos::ElaWallet::IMasterWallet* spvMasterWallet = mSpvMasterWalletMgr->CreateMasterWallet(masterWalletId.string()
        , mnemonic.string(), phrasePassword.string(), payPassword.string(), language.string());
    AutoPtr<IMasterWallet> wallet = new MasterWallet(spvMasterWallet);
    if (wallet == NULL) return E_OUT_OF_MEMORY;

    *masterWallet = wallet;
    mMasterWalletsMap[masterWalletId.string()] = wallet.Get();
    (*masterWallet)->AddRef();
    return NOERROR;
}

ECode CMasterWalletManager::GetAllMasterWallets(
    /* [out, callee] */ ArrayOf<IMasterWallet*>** wallets)
{
    VALIDATE_NOT_NULL(wallets);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    *wallets = NULL;

    std::vector<Elastos::ElaWallet::IMasterWallet *> array = mSpvMasterWalletMgr->GetAllMasterWallets();
    const Int32 length = array.size();
    if (length < 1) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<IMasterWallet*> > _masterWallets = ArrayOf<IMasterWallet*>::Alloc(length);
    if (_masterWallets == NULL) return E_OUT_OF_MEMORY;

    //If mMasterWalletsMap has the wallet, use it.
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IMasterWallet> masterWallet;

        std::string id = array[i]->GetId();
        std::map<const char*, IMasterWallet*, compare_string>::iterator iter = mMasterWalletsMap.find(id.c_str());
        if (iter == mMasterWalletsMap.end()) {
            masterWallet = new MasterWallet(array[i]);
            if (masterWallet == NULL) return E_OUT_OF_MEMORY;
        }
        else {
            masterWallet = iter->second;
        }
        _masterWallets->Set(i, masterWallet);
    }

    //Update the mMasterWalletsMap.
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IMasterWallet> masterWallet = (*_masterWallets)[i];
        String id;
        masterWallet->GetId(&id);

        std::map<const char*, IMasterWallet*, compare_string>::iterator iter = mMasterWalletsMap.find(id.string());
        if (iter == mMasterWalletsMap.end()) {
            mMasterWalletsMap[id.string()] = masterWallet.Get();
            masterWallet->AddRef();
        }
    }

    *wallets = _masterWallets;
    (*wallets)->AddRef();
    return NOERROR;
}

ECode CMasterWalletManager::DestroyWallet(
    /* [in] */ const String& masterWalletId)
{
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    mSpvMasterWalletMgr->DestroyWallet(masterWalletId.string());
    std::map<const char*, IMasterWallet*, compare_string>::iterator iter = mMasterWalletsMap.find(masterWalletId.string());
    if (iter != mMasterWalletsMap.end()) {
        IMasterWallet* wallet = iter->second;
        if (wallet) delete ((MasterWallet*)wallet);
        mMasterWalletsMap.erase(iter);
    }

    return NOERROR;
}

ECode CMasterWalletManager::ImportWalletWithKeystore(
    /* [in] */ const String& masterWalletId,
    /* [in] */ const String& keystoreContent,
    /* [in] */ const String& backupPassword,
    /* [in] */ const String& payPassword,
    /* [in] */ const String& phrasePassword,
    /* [out] */ IMasterWallet** masterWallet)
{
    VALIDATE_NOT_NULL(masterWallet);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    *masterWallet = NULL;

    Elastos::ElaWallet::IMasterWallet* spvWallet = mSpvMasterWalletMgr->ImportWalletWithKeystore(masterWalletId.string()
            , ToJosnFromString(keystoreContent.string()) , backupPassword.string(), payPassword.string(), phrasePassword.string());
    if (spvWallet == NULL) return E_INVALID_ARGUMENT;

    *masterWallet = new MasterWallet(spvWallet);
    if ((*masterWallet) == NULL) return E_OUT_OF_MEMORY;

    std::string _id = spvWallet->GetId();
    mMasterWalletsMap[_id.c_str()] = *masterWallet;
    (*masterWallet)->AddRef();
    return NOERROR;
}

ECode CMasterWalletManager::ImportWalletWithMnemonic(
    /* [in] */ const String& masterWalletId,
    /* [in] */ const String& mnemonic,
    /* [in] */ const String& phrasePassword,
    /* [in] */ const String& payPassword,
    /* [in] */ const String& language,
    /* [out] */ IMasterWallet** masterWallet)
{
    VALIDATE_NOT_NULL(masterWallet);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    *masterWallet = NULL;

    Elastos::ElaWallet::IMasterWallet* spvWallet = mSpvMasterWalletMgr->ImportWalletWithMnemonic(masterWalletId.string(), mnemonic.string()
                , phrasePassword.string(), payPassword.string(), language.string());
    if (spvWallet == NULL) return E_INVALID_ARGUMENT;

    *masterWallet = new MasterWallet(spvWallet);
    if ((*masterWallet) == NULL) return E_OUT_OF_MEMORY;

    std::string _id = spvWallet->GetId();
    mMasterWalletsMap[_id.c_str()] = *masterWallet;
    (*masterWallet)->AddRef();
    return NOERROR;
}

ECode CMasterWalletManager::ExportWalletWithKeystore(
    /* [in] */ IMasterWallet* masterWallet,
    /* [in] */ const String& backupPassword,
    /* [in] */ const String& payPassword,
    /* [out] */ String* keystoreContent)
{
    VALIDATE_NOT_NULL(keystoreContent);
    VALIDATE_NOT_NULL(masterWallet);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);

    nlohmann::json result = mSpvMasterWalletMgr->ExportWalletWithKeystore(((MasterWallet*)masterWallet)->GetSpvMasterWallet()
            , backupPassword.string(), payPassword.string());
    *keystoreContent = String(result.dump().c_str());
    return NOERROR;
}

ECode CMasterWalletManager::ExportWalletWithMnemonic(
    /* [in] */ IMasterWallet *masterWallet,
    /* [in] */ const String& payPassword,
    /* [out] */ String* mnemonic)
{
    VALIDATE_NOT_NULL(mnemonic);
    VALIDATE_NOT_NULL(masterWallet);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);

    std::string result = mSpvMasterWalletMgr->ExportWalletWithMnemonic(((MasterWallet*)masterWallet)->GetSpvMasterWallet(), payPassword.string());
    *mnemonic = String(result.c_str());
    return NOERROR;
}

ECode CMasterWalletManager::GenerateMnemonic(
    /* [in] */ const String& language,
    /* [out] */ String* mnemonic)
{
    VALIDATE_NOT_NULL(mnemonic);
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    std::string result = mSpvMasterWalletMgr->GenerateMnemonic(language.string());
    *mnemonic = String(result.c_str());
    return NOERROR;
}

ECode CMasterWalletManager::SaveConfigs()
{
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    mSpvMasterWalletMgr->SaveConfigs();
    return NOERROR;
}
