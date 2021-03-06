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

#include "elastos/security/cert/CCertPathBuilderHelper.h"
#include "elastos/security/cert/CertPathBuilder.h"

namespace Elastos {
namespace Security {
namespace Cert {

CAR_INTERFACE_IMPL(CCertPathBuilderHelper, Singleton, ICertPathBuilderHelper)
CAR_SINGLETON_IMPL(CCertPathBuilderHelper)

ECode CCertPathBuilderHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ ICertPathBuilder **builder)
{
    return CertPathBuilder::GetInstance(algorithm, builder);
}

ECode CCertPathBuilderHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [in] */ const String& provider,
    /* [out] */ ICertPathBuilder **builder)
{
    return CertPathBuilder::GetInstance(algorithm, provider, builder);
}

ECode CCertPathBuilderHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider *provider,
    /* [out] */ ICertPathBuilder **builder)
{
    return CertPathBuilder::GetInstance(algorithm, provider, builder);
}

ECode CCertPathBuilderHelper::GetDefaultType(
    /* [out] */ String *type)
{
    return CertPathBuilder::GetDefaultType(type);
}

}
}
}
