
#include "CMasterWalletManager.h"
#include "MasterWallet.h"

extern const char* ToStringFromJson(nlohmann::json jsonValue);
extern nlohmann::json ToJosnFromString(const char* str);

CAR_OBJECT_IMPL(CMasterWalletManager)
CAR_INTERFACE_IMPL(CMasterWalletManager, Object, IMasterWalletManager);

CMasterWalletManager::CMasterWalletManager()
    : mSpvMasterWalletMgr(NULL)
{}

CMasterWalletManager::~CMasterWalletManager()
{
    printf("======================= CMasterWalletManager =========================LINE=[%d]\n", __LINE__);
    if (mSpvMasterWalletMgr) {
        delete mSpvMasterWalletMgr;
    }
}

ECode CMasterWalletManager::constructor(
    /* [in] */ const String& rootPath)
{
    mSpvMasterWalletMgr = new Elastos::ElaWallet::MasterWalletManager(rootPath.string());
    if (mSpvMasterWalletMgr == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

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

    Elastos::ElaWallet::IMasterWallet* spvMasterWallet = mSpvMasterWalletMgr->CreateMasterWallet(masterWalletId.string()
        , mnemonic.string(), phrasePassword.string(), payPassword.string(), language.string());
    AutoPtr<IMasterWallet> wallet = new MasterWallet(spvMasterWallet);
    if (wallet == NULL) return E_OUT_OF_MEMORY;

    *masterWallet = wallet;
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
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IMasterWallet> masterWallet = new MasterWallet(array[i]);
        if (masterWallet == NULL) return E_OUT_OF_MEMORY;

        _masterWallets->Set(i, masterWallet);
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
    *masterWallet = new MasterWallet(spvWallet);
    if ((*masterWallet) == NULL) return E_OUT_OF_MEMORY;
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

    *masterWallet = new MasterWallet(spvWallet);
    if ((*masterWallet) == NULL) return E_OUT_OF_MEMORY;

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
    *keystoreContent = String(ToStringFromJson(result));
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
    printf("======================= CMasterWalletManager =========================LINE=[%d], lan=[%s]\n", __LINE__, language.string());
    std::string result = mSpvMasterWalletMgr->GenerateMnemonic(language.string());
    printf("======================= CMasterWalletManager =========================LINE=[%d]\n", __LINE__);
    *mnemonic = String(result.c_str());
    printf("======================= CMasterWalletManager =========================LINE=[%d]\n", __LINE__);
    return NOERROR;
}

ECode CMasterWalletManager::SaveConfigs()
{
    VALIDATE_NOT_NULL(mSpvMasterWalletMgr);
    mSpvMasterWalletMgr->SaveConfigs();
    return NOERROR;
}
