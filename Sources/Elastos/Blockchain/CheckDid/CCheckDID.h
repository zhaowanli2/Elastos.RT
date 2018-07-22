
#ifndef __ELASTOS_CHECKDID_CCHECKDID_H__
#define __ELASTOS_CHECKDID_CCHECKDID_H__

#include "_CCheckDID.h"
#include "elastos/core/Object.h"

CarClass(CCheckDID)
    , public Object
    , public ICheckDID
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI PublicKeyToIdAddress(
        /* [in] */ const String& publicKey,
        /* [out] */ String * pIdAddress);

    CARAPI VerifyByPublicKey(
        /* [in] */ const String& publicKey,
        /* [in] */ const String& message,
        /* [in] */ const String& signature,
        /* [out] */ Boolean * pResult);

private:
    // TODO: Add your private member variables here.
};

#endif // __ELASTOS_CHECKDID_CCHECKDID_H__
