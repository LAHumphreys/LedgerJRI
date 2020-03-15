#include <LedgerAccount.h>
#include <ledger/system.hh>
#include <ledger/account.h>

std::string LedgerAccount::name(const ledger::account_t& account) {
    return std::string(account.name);
}

unsigned short LedgerAccount::depth(const ledger::account_t& account) {
    return account.depth;
}
