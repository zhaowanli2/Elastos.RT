
#include "SubWallet.h"
#include <sstream>
#include "MainchainSubWallet.h"
#include "IdChainSubWallet.h"
#include "SidechainSubWallet.h"


#include <android/log.h>

#define TAG "Elastos_CAR_Wallet"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)

String ToStringFromJson(nlohmann::json jsonValue);
nlohmann::json ToJosnFromString(const char* str);

SubWallet::SubWallet(
    /* [in] */ Elastos::ElaWallet::ISubWallet* spvSubWallet)
    : mSpvSubWallet(spvSubWallet)
{
    pthread_mutex_init(&mCallbacksLock, NULL);
}

SubWallet::~SubWallet()
{
    pthread_mutex_destroy(&mCallbacksLock);
}

PInterface SubWallet::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ISubWallet) {
        return (ISubWallet*)this;
    }
    else if (riid == EIID_IMainchainSubWallet) {
        return (IMainchainSubWallet*)(MainchainSubWallet*)this;
    }
    else if (riid == EIID_ISidechainSubWallet) {
        return (ISidechainSubWallet*)(SidechainSubWallet*)this;
    }
    else if (riid == EIID_IIdChainSubWallet) {
        return (IIdChainSubWallet*)(IdChainSubWallet*)this;
    }

    return NULL;
}

UInt32 SubWallet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SubWallet::Release()
{
    return ElRefBase::Release();
}

ECode SubWallet::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface *)(ISubWallet *)this) {
        *iid = EIID_ISubWallet;
    }
    if (object == (IInterface *)(IMainchainSubWallet *)this) {
        *iid = EIID_IMainchainSubWallet;
    }
    if (object == (IInterface *)(ISidechainSubWallet *)this) {
        *iid = EIID_ISidechainSubWallet;
    }
    if (object == (IInterface *)(IIdChainSubWallet *)this) {
        *iid = EIID_IIdChainSubWallet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode SubWallet::GetChainId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    std::string result = mSpvSubWallet->GetChainId();
    *id = String(result.c_str());
    return NOERROR;
}

ECode SubWallet::GetBalanceInfo (
    /* [out] */ String* balanceInfoJson)
{
    VALIDATE_NOT_NULL(balanceInfoJson);
    nlohmann::json result = mSpvSubWallet->GetBalanceInfo();
    *balanceInfoJson = ToStringFromJson(result);
    return NOERROR;
}

ECode SubWallet::GetBalance (
    /* [out] */ Int64* balance)
{
    VALIDATE_NOT_NULL(balance);
    *balance = mSpvSubWallet->GetBalance();
    return NOERROR;
}

ECode SubWallet::CreateAddress (
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    std::string result = mSpvSubWallet->CreateAddress();
    *address = String(result.c_str());
    return NOERROR;
}

ECode SubWallet::GetAllAddress (
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [out] */ String* addressesJson)
{
    VALIDATE_NOT_NULL(addressesJson);
    nlohmann::json addresses = mSpvSubWallet->GetAllAddress(start, count);
    *addressesJson = ToStringFromJson(addresses);
    return NOERROR;
}

ECode SubWallet::GetBalanceWithAddress (
    /* [in] */ const String& address,
    /* [out] */ Int64* balance)
{
    VALIDATE_NOT_NULL(balance);
    *balance = mSpvSubWallet->GetBalanceWithAddress(address.string());
    return NOERROR;
}

ECode SubWallet::AddCallback (
    /* [in] */ ISubWalletListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    AutoPtr<SubWalletCallback> callback = new SubWalletCallback(listener);
    if (callback == NULL) return E_OUT_OF_MEMORY;

    ECode ec = AddSubWalletCallbackNode(callback);
    if (SUCCEEDED(ec)) {
        mSpvSubWallet->AddCallback(callback);
    }

    return ec;
}

ECode SubWallet::RemoveCallback (
    /* [in] */ ISubWalletListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    SubWalletCallback* callback = NULL;
    pthread_mutex_lock(&mCallbacksLock);
    SubWalletCallbackNode* it = &mCallbacks;
    it = (SubWalletCallbackNode*)(it->Next());
    for (; NULL != it; it = (SubWalletCallbackNode*)(it->Next())) {
        if (it->mCallback->mListener.Get() == listener) {
            callback = it->mCallback;
            break;
        }
    }
    pthread_mutex_unlock(&mCallbacksLock);

    if (callback != NULL) {
        mSpvSubWallet->RemoveCallback(it->mCallback);
        RemoveSubWalletCallbackNode(it->mCallback);
    }

    return NOERROR;
}

ECode SubWallet::CreateTransaction (
    /* [in] */ const String& fromAddress,
    /* [in] */ const String& toAddress,
    /* [in] */ Int64 amount,
    /* [in] */ const String& memo,
    /* [in] */ const String& remark,
    /* [out] */ String* txidJson)
{
    VALIDATE_NOT_NULL(txidJson);
    LOGD("FUNC=[%s]========================LINE=[%d]", __FUNCTION__, __LINE__);
    nlohmann::json result = mSpvSubWallet->CreateTransaction(fromAddress.string(), toAddress.string(), amount
            , memo.string(), remark.string());
    *txidJson = ToStringFromJson(result);
    LOGD("FUNC=[%s]========================LINE=[%d], result=[%s]", __FUNCTION__, __LINE__, (*txidJson).string());
    return NOERROR;
}

ECode SubWallet::CreateMultiSignAddress (
    /* [in] */ const String& multiPublicKeyJson,
    /* [in] */ Int32 totalSignNum,
    /* [in] */ Int32 requiredSignNum,
    /* [out] */ String* multiSignAddress)
{
    VALIDATE_NOT_NULL(multiSignAddress);
    std::string result = mSpvSubWallet->CreateMultiSignAddress(ToJosnFromString(multiPublicKeyJson.string()), totalSignNum, requiredSignNum);
    *multiSignAddress = String(result.c_str());
    return NOERROR;
}

ECode SubWallet::CreateMultiSignTransaction (
    /* [in] */ const String& fromAddress,
    /* [in] */ const String& toAddress,
    /* [in] */ Int64 amount,
    /* [in] */ const String& memo,
    /* [out] */ String* transactionJson)
{
    VALIDATE_NOT_NULL(transactionJson);
    nlohmann::json result = mSpvSubWallet->CreateMultiSignTransaction(fromAddress.string(), toAddress.string()
                , amount, memo.string());
    *transactionJson = ToStringFromJson(result);
    return NOERROR;
}

ECode SubWallet::SendRawTransaction (
    /* [in] */ const String& transactionJson,
    /* [in] */ Int64 fee,
    /* [in] */ const String& payPassword,
    /* [out] */ String* txidJson)
{
    VALIDATE_NOT_NULL(txidJson);
    nlohmann::json result = mSpvSubWallet->SendRawTransaction(ToJosnFromString(transactionJson.string()), fee, payPassword.string());
    *txidJson = ToStringFromJson(result);
    return NOERROR;
}

ECode SubWallet::GetAllTransaction (
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ const String& addressOrTxid,
    /* [out] */ String* transactionListJson)
{
    VALIDATE_NOT_NULL(transactionListJson);
    nlohmann::json result = mSpvSubWallet->GetAllTransaction(start, count, addressOrTxid.string());
    *transactionListJson = String(result.dump().c_str());
    return NOERROR;
}

ECode SubWallet::Sign (
    /* [in] */ const String& message,
    /* [in] */ const String& payPassword,
    /* [out] */ String* signature)
{
    VALIDATE_NOT_NULL(signature);
    std::string result = mSpvSubWallet->Sign(message.string(), payPassword.string());
    *signature = String(result.c_str());
    return NOERROR;
}

ECode SubWallet::CheckSign (
    /* [in] */ const String& address,
    /* [in] */ const String& message,
    /* [in] */ const String& signature,
    /* [out] */ String* resultJson)
{
    VALIDATE_NOT_NULL(resultJson);
    nlohmann::json result = mSpvSubWallet->CheckSign(address.string(), message.string(), signature.string());
    *resultJson = ToStringFromJson(result);
    return NOERROR;
}

ECode SubWallet::CalculateTransactionFee(
    /* [in] */ const String& rawTransaction,
    /* [in] */ Int64 feePerKb,
    /* [out] */ Int64* fee)
{
    VALIDATE_NOT_NULL(fee);
    *fee = mSpvSubWallet->CalculateTransactionFee(ToJosnFromString(rawTransaction.string()), feePerKb);
    return NOERROR;
}

String ToStringFromJson(nlohmann::json jsonValue)
{
    const char* value = jsonValue.dump().c_str();
    LOGD("FUNC=[%s]========================LINE=[%d], len=[%d], value=[%s]", __FUNCTION__, __LINE__, strlen(value), value);
    if (!strcmp(value, "null")) {
        return String(NULL);
    }

    const Int32 len = strlen(value);
    if (len > 800) {
        std::string str(value);
        Int32 tmpLen = 0;
        Int32 pos = 0;
        while(tmpLen >= 0) {
            LOGD("1===FUNC=[%s]======LINE=[%d], len=[%d], value[%d]=[%s]", __FUNCTION__, __LINE__,
                            len, pos, str.substr(pos * 800, 800).c_str());
            pos++;
            tmpLen = len - pos * 800;
        }

        String tmp(value);
        tmpLen = 0;
        pos = 0;
        while(tmpLen >= 0) {
            LOGD("2===FUNC=[%s]======LINE=[%d], elLen=[%d], len=[%d], value[%d]=[%s]", __FUNCTION__, __LINE__, tmp.GetLength(),
                            len, pos, tmp.Substring(pos * 800, pos * 800 + 800).string());
            pos++;
            tmpLen = len - pos * 800;
        }

        String tmp2(value);
        LOGD("FUNC=[%s]========================LINE=[%d], len=[%d], strValue=[%s]", __FUNCTION__, __LINE__, tmp2.GetLength(), tmp2.string());
    }

    return String(value);
}

nlohmann::json ToJosnFromString(const char* str)
{
    nlohmann::json jsonValue;
    std::stringstream ss;
    ss << str;
    jsonValue << ss;
    return jsonValue;
}

SubWallet::SubWalletCallback::SubWalletCallback(
    /* [in] */ ISubWalletListener* listener)
    : mListener(listener)
{}

void SubWallet::SubWalletCallback::OnTransactionStatusChanged(
    /* [in] */ const std::string &txid,
    /* [in] */ const std::string &status,
    /* [in] */ const nlohmann::json &desc,
    /* [in] */ uint32_t confirms)
{
    if (mListener == NULL) return;
    mListener->OnTransactionStatusChanged(String(txid.c_str()), String(status.c_str()), ToStringFromJson(desc), confirms);
}

void SubWallet::SubWalletCallback::OnBlockSyncStarted()
{
    if (mListener == NULL) return;
    mListener->OnBlockSyncStarted();
}

void SubWallet::SubWalletCallback::OnBlockHeightIncreased(
    /* [in] */ uint32_t currentBlockHeight,
    /* [in] */ double progress)
{
    if (mListener == NULL) return;
    mListener->OnBlockHeightIncreased(currentBlockHeight, progress);
}

void SubWallet::SubWalletCallback::OnBlockSyncStopped()
{
    if (mListener == NULL) return;
    mListener->OnBlockSyncStopped();
}

void SubWallet::SubWalletCallback::OnDestroyWallet()
{
    if (mListener == NULL) return;
    mListener->OnDestroyWallet();
}

ECode SubWallet::AddSubWalletCallbackNode(
    /* [in] */ SubWalletCallback* callback)
{
    pthread_mutex_lock(&mCallbacksLock);
    SubWalletCallbackNode* it = &mCallbacks;
    it = (SubWalletCallbackNode*)(it->Next());
    for (; NULL != it; it = (SubWalletCallbackNode*)(it->Next())) {
        if (it->mCallback.Get() == callback) {
            pthread_mutex_unlock(&mCallbacksLock);
            return NOERROR;
        }
    }

    SubWalletCallbackNode* listenerNode = new SubWalletCallbackNode;
    if (listenerNode == NULL) {
        pthread_mutex_unlock(&mCallbacksLock);
        return E_OUT_OF_MEMORY;
    }

    listenerNode->mCallback = callback;
    mCallbacks.InsertFirst(listenerNode);
    pthread_mutex_unlock(&mCallbacksLock);
    return NOERROR;
}

void SubWallet::RemoveSubWalletCallbackNode(
    /* [in] */ SubWalletCallback* callback)
{
    pthread_mutex_lock(&mCallbacksLock);
    SubWalletCallbackNode* prev = &mCallbacks;
    SubWalletCallbackNode* head = &mCallbacks;
    SubWalletCallbackNode* it = (SubWalletCallbackNode*)(head->Next());
    for (; NULL != it; it = (SubWalletCallbackNode*)(it->Next())) {
        if (it->mCallback.Get() == callback) {
            delete it->mCallback;
            it->mCallback = NULL;

            if (it != head) {
                it->Detach(prev);
                delete it;
            }

            pthread_mutex_unlock(&mCallbacksLock);
            return;
        }
        prev = it;
    }

    pthread_mutex_unlock(&mCallbacksLock);
}
