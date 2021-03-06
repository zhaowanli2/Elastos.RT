//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELDELEGATE_H__
#define __ELDELEGATE_H__

#include <assert.h>
#include <elautoptr.h>

#define _Impl_Memset memset


typedef struct _Delegate
{
    union
    {
        struct
        {
            void* mFunc;
            void* mThis;
        };
        _ELASTOS UInt64 mDelegate;
    };
} _Delegate;

typedef enum {
    CallbackType_PlainC = 0x00,
    CallbackType_CPP = 0x01,
    CallbackType_CAR = 0x02,
} CallbackType;

#define CallbackTypeMask 0x00000003
#define CallbackPtrMask  0xFFFFFFFC
/*
* The pointer must align at 4-byte boundary, so its lowest 2 bits
* are always 0. So, we can use that 2 bits saving FunctionType
* of _Delegate.mFunc
*
* if ((_Delegate.mFunc & CallbackTypeMask) == 0x00)
*     mFunc is a C style function
* else if ((_Delegate.mFunc & CallbackTypeMask) == 0x01)
*     mFunc is a normal CPP class member method
* else if ((_Delegate.mFunc & CallbackTypeMask) == 0x02)
*     mFunc is a car Class member method
*/

_ELASTOS_NAMESPACE_BEGIN

class EventHandler
{
public:
    EventHandler()
    {
        mValue.mThis = NULL;
        mValue.mFunc = NULL;
    }

    EventHandler(
        /* [in] */ void* funcPtr)
    {
        mValue.mThis = NULL;
        mValue.mFunc = funcPtr;
    }

    _ELASTOS Boolean operator ==(
        /* [in] */ EventHandler & er)
   {
        return ((mValue.mThis == er.mValue.mThis)
                && (GetFuncPtr() == er.GetFuncPtr()));
    }

    _ELASTOS Boolean operator !=(
        /* [in] */ EventHandler & er)
    {
        return !(*this == er);
    }

    void *GetThisPtr()
    {
        return mValue.mThis;
    }

    void *GetFuncPtr()
    {
        if (NULL == mValue.mFunc) return NULL;
#ifndef _GNUC
        return (void *)((uintptr_t)mValue.mFunc & CallbackPtrMask);
#else
#if defined(_arm) || defined(_x86)
        // for gnu-arm-pe compiler, it is another story
        // an virtual function's address &Foo::Bar() is the offset of &vptr[0]
        //
        // In gnu-x86 compiler, the value of pointer pointing to virtual function
        // is index * 4 + 1. The code below calculates the true address of
        // function according to mThis and mFunc.
        //
        if (*(int*)&mValue.mFunc & 0xFFFFF000) {
            return (void *)((uintptr_t)mValue.mFunc & CallbackPtrMask);
        }
        else {
            mValue.mFunc = (void*)(
                    (uintptr_t)(*(*(void***)mValue.mThis + ((uintptr_t)mValue.mFunc >> 2)))
                    | ((uintptr_t)mValue.mFunc & CallbackTypeMask) );
            return (void *)((uintptr_t)mValue.mFunc & CallbackPtrMask);
        }
#else
        assert(0 && "your compiler is not support yet!\n");
        return NULL;
#endif  //_arm | _x86
#endif // _GNUC
    }

    CallbackType GetFuncType()
    {
        return (CallbackType)(CallbackTypeMask & (uintptr_t)mValue.mFunc);
    }

    static EventHandler Make(
        /* [in] */ void* thisPtr,
        /* [in] */ void* funcPtr)
    {
        return EventHandler(thisPtr, funcPtr, CallbackType_PlainC, NULL);
    }

    template <class T>
    static EventHandler Make(
        /* [in] */ T* thisPtr,
        /* [in] */ void* funcPtr,
        /* [in] */ CallbackType type)
    {
        assert(((type == CallbackType_CAR)
                && Conversion<T, CCarObject>::exists) || (type != CallbackType_CAR));
        return EventHandler(thisPtr, funcPtr, type, (_IInterface*)thisPtr);
    }

private:
    EventHandler(
        /* [in] */ void* thisPtr,
        /* [in] */ void* funcPtr,
        /* [in] */ CallbackType type,
        /* [in] */ PInterface object)
    {
        mValue.mThis = thisPtr;
        mValue.mFunc = funcPtr;

        mValue.mFunc = (_ELASTOS PVoid)((uintptr_t)mValue.mFunc | (CallbackTypeMask & type));
        mCarObjClient = object;
    }

public:
    AutoPtr<IInterface> mCarObjClient;

private:
    _Delegate mValue;
};

_ELASTOS_NAMESPACE_END

#endif // __ELDELEGATE_H__
