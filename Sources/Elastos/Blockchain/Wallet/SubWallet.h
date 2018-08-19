
#ifndef __ELASTOS_WALLET_SUBWALLET_H__
#define __ELASTOS_WALLET_SUBWALLET_H__

#include <elcontainer.h>
#include "elastos/core/Object.h"
#include "Elastos.Wallet.h"
#include "ISubWallet.h"
#include "ISubWalletCallback.h"

class SubWallet
    : public Object
    , public ISubWallet
{
private:
    class SubWalletCallback
        : public Object
        , public Elastos::ElaWallet::ISubWalletCallback
    {
    public:
        SubWalletCallback(
            /* [in] */ ISubWalletListener* listener);

        virtual void OnTransactionStatusChanged(
            /* [in] */ const std::string &txid,
            /* [in] */ const std::string &status,
            /* [in] */ const nlohmann::json &desc,
            /* [in] */ uint32_t confirms);

        virtual void OnBlockSyncStarted();

        virtual void OnBlockHeightIncreased(
            /* [in] */ uint32_t currentBlockHeight,
            /* [in] */ double progress);

        virtual void OnBlockSyncStopped();

        virtual void OnDestroyWallet();

    public:
        AutoPtr<ISubWalletListener> mListener;
    };

    class SubWalletCallbackNode : public SLinkNode
    {
    public:
        SubWalletCallbackNode(){};

    public:
        AutoPtr<SubWalletCallback> mCallback;
    };

public:
    SubWallet(
        /* [in] */ Elastos::ElaWallet::ISubWallet* spvSubWallet);

    virtual ~SubWallet();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid);

    CARAPI GetChainId(
        /* [out] */ String* id);

    CARAPI GetBalanceInfo (
        /* [out] */ String* balanceInfoJson);

    CARAPI GetBalance (
        /* [out] */ Int64* balance);

    CARAPI CreateAddress (
        /* [out] */ String* address);

    CARAPI GetAllAddress (
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [out] */ String* addressesJson);

    CARAPI GetBalanceWithAddress (
        /* [in] */ const String& address,
        /* [out] */ Int64* balance);

    CARAPI AddCallback (
        /* [in] */ ISubWalletListener* subCallback);

    CARAPI RemoveCallback (
        /* [in] */ ISubWalletListener* subCallback);

    CARAPI CreateTransaction (
        /* [in] */ const String& fromAddress,
        /* [in] */ const String& toAddress,
        /* [in] */ Int64 amount,
        /* [in] */ const String& memo,
        /* [in] */ const String& remark,
        /* [out] */ String* txidJson);

    CARAPI CreateMultiSignAddress (
        /* [in] */ const String& multiPublicKeyJson,
        /* [in] */ Int32 totalSignNum,
        /* [in] */ Int32 requiredSignNum,
        /* [out] */ String* multiSignAddress);

    CARAPI CreateMultiSignTransaction (
        /* [in] */ const String& fromAddress,
        /* [in] */ const String& toAddress,
        /* [in] */ Int64 amount,
        /* [in] */ const String& memo,
        /* [out] */ String* transactionJson);

    CARAPI SendRawTransaction (
        /* [in] */ const String& transactionJson,
        /* [in] */ Int64 fee,
        /* [in] */ const String& payPassword,
        /* [out] */ String* txidJson);

    CARAPI GetAllTransaction (
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ const String& addressOrTxid,
        /* [out] */ String* transactionListJson);

    CARAPI Sign (
        /* [in] */ const String& message,
        /* [in] */ const String& payPassword,
        /* [out] */ String* signature);

    CARAPI CheckSign (
        /* [in] */ const String& address,
        /* [in] */ const String& message,
        /* [in] */ const String& signature,
        /* [out] */ String* resultJson);

    CARAPI CalculateTransactionFee(
        /* [in] */ const String& rawTransaction,
        /* [in] */ Int64 feePerKb,
        /* [out] */ Int64* fee);

    ECode AddSubWalletCallbackNode(
        /* [in] */ SubWalletCallback* callback);

    void RemoveSubWalletCallbackNode(
        /* [in] */ SubWalletCallback* callback);

protected:
    Elastos::ElaWallet::ISubWallet* mSpvSubWallet;

private:
    SubWalletCallbackNode mCallbacks;
    pthread_mutex_t mCallbacksLock;
};

#endif // __ELASTOS_WALLET_SUBWALLET_H__
