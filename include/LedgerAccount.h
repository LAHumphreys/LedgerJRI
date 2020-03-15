#ifndef LEDGERJRI_LEDGERACCOUNT_H
#define LEDGERJRI_LEDGERACCOUNT_H

#include <string>

namespace ledger {
    class account_t;
}

namespace LedgerAccount {
    std::string name(const ledger::account_t& account);
    unsigned short depth(const ledger::account_t& account);
}

#endif //LEDGERJRI_LEDGERACCOUNT_H
