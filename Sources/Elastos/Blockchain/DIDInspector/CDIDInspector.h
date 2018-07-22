
#ifndef __ELASTOS_DIDINSPECTOR_CDIDCHECKER_H__
#define __ELASTOS_DIDINSPECTOR_CDIDCHECKER_H__

#include "_CDIDInspector.h"
#include "elastos/core/Object.h"

CarClass(CDIDInspector)
    , public Object
    , public IDIDInspector
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI CheckDID(
        /* [in] */ const String& publicKey,
        /* [in] */ const String& did,
        /* [out] */ Boolean* matched);

    CARAPI CheckSign(
        /* [in] */ const String& publicKey,
        /* [in] */ const String& message,
        /* [in] */ const String& signature,
        /* [out] */ Boolean* matched);
};

#endif // __ELASTOS_DIDINSPECTOR_CDIDCHECKER_H__
