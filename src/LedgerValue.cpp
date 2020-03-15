#include "LedgerValue.h"

#include <ledger/system.hh>
#include <ledger/report.h>

LedgerValue::LedgerValue(const ledger::value_t &val) {
    if (val.is_amount()) {
        amounts.emplace_back(val.as_amount());
        valType = Type::AMOUNT;
    } else {
        valType = Type::ZERO;
    }
}

