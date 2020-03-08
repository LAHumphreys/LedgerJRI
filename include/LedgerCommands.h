//
// Created by lhumphreys on 10/03/2020.
//

#ifndef LEDGERJRI_LEDGERCOMMANDS_H
#define LEDGERJRI_LEDGERCOMMANDS_H
#include <memory>
#include <vector>

class Ledger;
class LedgerSession;

/**
 * Ledger CLI Interface
 */
class LedgerCommands {
public:
    LedgerCommands(Ledger&, LedgerSession& data);
    ~LedgerCommands() = default;

    using Args = std::vector<std::string>;
    std::string BalanceReport(const Args& args);
private:
    Ledger&        ledgerInstance;
    LedgerSession& sess;
};

#endif //LEDGERJRI_LEDGERCOMMANDS_H
