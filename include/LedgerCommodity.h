//
// Created by lhumphreys on 15/03/2020.
//

#ifndef LEDGERJRI_LEDGERCOMMODITY_H
#define LEDGERJRI_LEDGERCOMMODITY_H
#include <string>

namespace ledger {
    class commodity_t;
}

class LedgerCommodity {
public:
    LedgerCommodity(const ledger::commodity_t& comm);

    const std::string& Symbol() const { return symbol; }
private:
    const std::string symbol;
};

#endif //LEDGERJRI_LEDGERCOMMODITY_H
