//=========================================================================
// Copyright (C) 2018 The Elastos Open Source Project
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

#include "Elastos.Wallet.h"
#include "elastos/core/StringUtils.h"
#include <stdio.h>
#include <ctype.h>
#include <stdexcept>

using Elastos::Core::StringUtils;

String masterWalletId("masterWalletId");
String phrasePassword("phrasePassword");
String payPassword("payPassword");
String language("english");
const Int64 SELA = 100000000;

void print(const char* function, String value)
{
    printf("[%s]==================[%s]\n", function, value.string());
}

static void do_cmd(
    /* [in] */ char *line,
    /* [in] */ ISubWallet* subWallet)
{
    assert(subWallet != NULL);
    char *args[16];
    int count = 0;
    char *p = NULL;
    int word = 0;

    for (p = line; *p != 0; p++) {
        if (isspace(*p)) {
            *p = 0;
            word = 0;
        } else {
            if (word == 0) {
                args[count] = p;
                count++;
            }

            word = 1;
        }
    }

    if (!strcmp(args[0], "address")) {
        String address;
        subWallet->CreateAddress(&address);
        print("address", address);
    }
    else if (!strcmp(args[0], "import")) {
        for (int i = 0; i < count; i++) {
            printf("args[%d]==================[%s]\n", i, args[i]);
        }
        // walletMgr->ImportWalletWithMnemonic(masterWalletId, );
    }
    else if (!strcmp(args[0], "balance")) {
        Int64 balance = 0;
        subWallet->GetBalance(&balance);
        printf("[balance]==================[%lld]\n", balance);
    }
    else if (!strcmp(args[0], "send")) {
        String fromAddress("");
        String toAddress(args[1]);
        print("toAddress", toAddress);
        Int64 amount = 0;
        StringUtils::Parse(String(args[2]), &amount);
        printf("[amount]==================[%lld]\n", amount);
        Int64 fee = 0;
        String memo("");
        String remark("");
        String txidJson;
        //1. CreateTransaction
        try {
            subWallet->CreateTransaction(fromAddress, toAddress, amount * SELA, fee, memo, remark, &txidJson);
        }
        catch (std::invalid_argument& e) {
            print("CreateTransaction invalid_argument", String(e.what()));
        }
        catch (std::logic_error& e) {
            print("CreateTransaction logic_error", String(e.what()));
        }
        catch (std::runtime_error& e) {
            print("CreateTransaction runtime_error", String(e.what()));
        }
        catch (std::exception& e) {
            print("CreateTransaction exception", String(e.what()));
        }

        //2. CalculateTransactionFee
        Int64 feePerKb = 10000;
        Int64 calculateFee = 0;
        try {
            subWallet->CalculateTransactionFee(txidJson, feePerKb, &calculateFee);
        }
        catch (std::invalid_argument& e) {
            print("CalculateTransactionFee invalid_argument", String(e.what()));
        }
        catch (std::logic_error& e) {
            print("CalculateTransactionFee logic_error", String(e.what()));
        }
        catch (std::runtime_error& e) {
            print("CalculateTransactionFee runtime_error", String(e.what()));
        }
        catch (std::exception& e) {
            print("CalculateTransactionFee exception", String(e.what()));
        }

        //3. SendRawTransaction
        String json;
        try {
            subWallet->SendRawTransaction(txidJson, calculateFee, payPassword, &json);
        }
        catch (std::invalid_argument& e) {
            print("SendRawTransaction invalid_argument", String(e.what()));
        }
        catch (std::logic_error& e) {
            print("SendRawTransaction logic_error", String(e.what()));
        }
        catch (std::runtime_error& e) {
            print("SendRawTransaction runtime_error", String(e.what()));
        }
        catch (std::exception& e) {
            print("SendRawTransaction exception", String(e.what()));
        }
    }
}

int main(int argc, char *argv[])
{
    printf("======================= Wallet Test ========================\n");
    printf("=========Simple command: [address]=[balance]=[send]=========\n");

    String rootPath("Data");
    AutoPtr<IMasterWalletManager> walletMgr;
    CMasterWalletManager::New(rootPath, (IMasterWalletManager**)&walletMgr);
    assert(walletMgr != NULL);

    String mnemonic;
    printf("Func=[%s]=========================LINE=[%d]\n", __FUNCTION__, __LINE__);
    walletMgr->GenerateMnemonic(language, &mnemonic);
    printf("Func=[%s]=========================LINE=[%d]\n", __FUNCTION__, __LINE__);
    print("GenerateMnemonic", mnemonic);



    // AutoPtr<IMasterWallet> masterWallet;
    // AutoPtr<ISubWallet> subWallet;
    // AutoPtr<ArrayOf<IMasterWallet*> > masterWallets;
    // walletMgr->GetAllMasterWallets((ArrayOf<IMasterWallet*>**)&masterWallets);
    // //0. Has MasterWallet?
    // if (masterWallets != NULL) {
    //     masterWallet = (*masterWallets)[0];
    //     assert(masterWallet != NULL);
    //     AutoPtr<ArrayOf<ISubWallet*> > subWallets;
    //     masterWallet->GetAllSubWallets((ArrayOf<ISubWallet*>**)&subWallets);
    //     if (subWallets != NULL) {
    //         subWallet = (*subWallets)[0];
    //     }
    // }
    // else {
    //     String mnemonic;
    //     printf("======================= Wallet Test =========================LINE=[%d]\n", __LINE__);

    //     try {
    //         //1. Get mnemonic
    //         walletMgr->GenerateMnemonic(language, &mnemonic);
    //         print("GenerateMnemonic", mnemonic);
    //         printf("======================= Wallet Test =========================LINE=[%d]\n", __LINE__);
    //         //2. CreateMasterWallet
    //         walletMgr->CreateMasterWallet(masterWalletId, mnemonic, phrasePassword, payPassword, language, (IMasterWallet**)&masterWallet);
    //         assert(masterWallet != NULL);
    //         printf("======================= Wallet Test =========================LINE=[%d]\n", __LINE__);
    //         //3. CreateSubWallet
    //         String chainID("ELA");
    //         Boolean singleAddress = FALSE;
    //         Int64 feePerKb = 0;
    //         masterWallet->CreateSubWallet(chainID, payPassword, singleAddress, feePerKb, (ISubWallet**)&subWallet);
    //         assert(subWallet != NULL);
    //     }
    //     catch (std::invalid_argument& e) {
    //         print("SPV invalid_argument", String(e.what()));
    //     }
    //     catch (std::logic_error& e) {
    //         print("SPV logic_error", String(e.what()));
    //     }
    //     catch (std::runtime_error& e) {
    //         print("SPV runtime_error", String(e.what()));
    //     }
    //     catch (std::exception& e) {
    //         print("SPV exception", String(e.what()));
    //     }
    // }

    // if (subWallet != NULL) {
    //     char input[512];
    //     while(1){
    //         gets(input);
    //         // String str(input);
    //         // printf("str=%s\n", str.string());
    //         do_cmd(input, subWallet);
    //     }
    // }

    printf("===================== Exit Wallet Test ======================\n");
    return 0;
}
