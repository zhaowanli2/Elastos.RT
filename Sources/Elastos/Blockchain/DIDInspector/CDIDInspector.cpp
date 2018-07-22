
#include "CDIDInspector.h"
#include <string>
#include "Key.h"
#include "Utils.h"

using namespace Elastos::ElaWallet;

CAR_OBJECT_IMPL(CDIDInspector)
CAR_INTERFACE_IMPL(CDIDInspector, Object, IDIDInspector);
ECode CDIDInspector::CheckDID(
    /* [in] */ const String& publicKey,
    /* [in] */ const String& did,
    /* [out] */ Boolean* matched)
{
    VALIDATE_NOT_NULL(matched);
    *matched = FALSE;
    if (publicKey.IsNullOrEmpty() || did.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CMBlock pubKey = Utils::decodeHex(publicKey.string());
    BRKey key;
    memcpy(key.pubKey, pubKey, pubKey.GetSize());
    key.compressed = (pubKey.GetSize() <= 33);
    std::string id = Key::publicKeyToIdAddress(&key);
    *matched = did.Equals(id.c_str());

    return NOERROR;
}

ECode CDIDInspector::CheckSign(
    /* [in] */ const String& publicKey,
    /* [in] */ const String& message,
    /* [in] */ const String& signature,
    /* [out] */ Boolean* matched)
{
    VALIDATE_NOT_NULL(matched);
    *matched = FALSE;
    if (publicKey.IsNullOrEmpty() || message.IsNullOrEmpty() || signature.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *matched = Key::verifyByPublicKey(publicKey.string(), message.string(), signature.string());
    return NOERROR;
}
