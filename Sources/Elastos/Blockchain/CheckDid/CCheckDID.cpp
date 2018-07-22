
#include "CCheckDID.h"

CAR_OBJECT_IMPL(CCheckDID)
CAR_INTERFACE_IMPL(CCheckDID, Object, ICheckDID);

ECode CCheckDID::PublicKeyToIdAddress(
    /* [in] */ const String& publicKey,
    /* [out] */ String * pIdAddress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCheckDID::VerifyByPublicKey(
    /* [in] */ const String& publicKey,
    /* [in] */ const String& message,
    /* [in] */ const String& signature,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
