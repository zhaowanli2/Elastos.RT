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

#include <elapi.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#ifdef _win32
#include <e_time.h>
#endif

using namespace Elastos;

EXTERN_C const InterfaceID EIID_IProxy;

// ElAPI functions
//

#ifdef _android
// #include <marshal_ipc.h>
#endif

#ifndef _apple
#include <marshal.h>
#endif

pthread_key_t g_TlSystemSlots[10];

EXTERN void InitTLS()
{
    for (Int32 i = 0; i < 10; i++) {
        pthread_key_create(&g_TlSystemSlots[i], NULL);
    }
}

EXTERN void UninitTLS()
{
    for (Int32 i = 0; i < 10; i++) {
        pthread_key_delete(g_TlSystemSlots[i]);
    }
}

ELAPI _CObject_MarshalInterface(
    /* [in] */ IInterface* object,
    /* [in] */ MarshalType type,
    /* [out] */ void** package,
    /* [out] */ Int32* size)
{
    if (package == NULL) {
        if (size) *size = 0;
        return E_INVALID_ARGUMENT;
    }
    *package = NULL;

    if (size == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *size = 0;

    if (object == NULL || size == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (type == MarshalType_IPC) {
#ifdef _android
        assert(0 && "TODO: Need Binder");
        // Elastos::IPC::InterfacePack* itfPack = (Elastos::IPC::InterfacePack*)calloc(sizeof(Elastos::IPC::InterfacePack), 1);
        // if (itfPack == NULL) {
        //     return E_OUT_OF_MEMORY;
        // }

        // ECode ec = StdMarshalInterface(object, itfPack);
        // if (FAILED(ec)) {
        //     free(itfPack);
        //     return ec;
        // }

        // *size = sizeof(Elastos::IPC::InterfacePack);
        // *package = itfPack;
        return NOERROR;
#else
        return E_NOT_IMPLEMENTED;
#endif
    }
    else if (type == MarshalType_RPC) {
#ifdef _apple
        assert(0 && "RPC is not supported on ios.");
#else
#ifdef _ELASTOS64
        assert(0 && "64-bit cpu architecture does not support RPC.");
#else
        InterfacePack* itfPack = (InterfacePack*)calloc(sizeof(InterfacePack), 1);
        if (itfPack == NULL) {
            return E_OUT_OF_MEMORY;
        }

        ECode ec = StdMarshalInterface(object, itfPack);
        if (FAILED(ec)) {
            free(itfPack);
            return ec;
        }
        *size = sizeof(InterfacePack);
        *package = itfPack;
#endif
#endif
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ELAPI _CObject_UnmarshalInterface(
    /* [in] */ void* package,
    /* [in] */ MarshalType type,
    /* [in] */ UnmarshalFlag flag,
    /* [out] */ IInterface** object,
    /* [out] */ Int32* size)
{
    if (object == NULL) {
        if (size) *size = 0;
        return E_INVALID_ARGUMENT;
    }
    *object = NULL;

    if (size == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *size = 0;

    if (package == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (type == MarshalType_IPC) {
#ifdef _android
        assert(0 && "TODO: Need Binder");
        // ECode ec = StdUnmarshalInterface(flag, (Elastos::IPC::InterfacePack*)package, object);
        // if (FAILED(ec)) {
        //     return ec;
        // }

        // *size = sizeof(Elastos::IPC::InterfacePack);
        return NOERROR;
#else
        return E_NOT_IMPLEMENTED;
#endif
    }
    else if (type == MarshalType_RPC) {
#ifdef _apple
        assert(0 && "RPC is not supported on ios.");
#else
#ifdef _ELASTOS64
        assert(0 && "64-bit cpu architecture does not support RPC.");
#else
#ifdef _android
        ECode ec = StdUnmarshalInterface(flag, NULL, (InterfacePack*)package, object);
#else
        ECode ec = StdUnmarshalInterface(flag, (InterfacePack*)package, object);
#endif
        if (FAILED(ec)) {
            return ec;
        }

        *size = sizeof(InterfacePack);
#endif
#endif
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

