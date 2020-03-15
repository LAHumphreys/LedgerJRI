//
// Created by lhumphreys on 15/03/2020.
//

#ifndef LEDGERJRI_LEDGERQUERYINTERFACE_H
#define LEDGERJRI_LEDGERQUERYINTERFACE_H

#include <string>

namespace ledger {
    class account_t;
}

class AccountQuery {
public:
    virtual const std::string& Verb() = 0;
    virtual void NextAccount(ledger::account_t& item) = 0;
    virtual void QueryCompleted() = 0;
};

#endif //LEDGERJRI_LEDGERQUERYINTERFACE_H
