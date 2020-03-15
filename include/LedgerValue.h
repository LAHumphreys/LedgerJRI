//
// Created by lhumphreys on 15/03/2020.
//

#ifndef LEDGERJRI_LEDGERVALUE_H
#define LEDGERJRI_LEDGERVALUE_H

#include "LedgerAmount.h"
#include <vector>

namespace ledger {
    class value_t;
    class report_t;
}


class LedgerValue {
public:
    LedgerValue(const ledger::value_t& val);

    enum class Type {
        AMOUNT,
        ZERO
    };

    const Type& ValueType() const {
        return valType;
    }

    const std::vector<LedgerAmount>& Amounts() const {
        return amounts;
    }

    LedgerValue (LedgerValue&& rhs ) = default;
    LedgerValue& operator=(LedgerValue&& rhs ) = default;

private:
    LedgerValue (const LedgerValue& rhs ) = delete;
    LedgerValue& operator=(const LedgerValue& rhs ) = delete;

    std::vector<LedgerAmount> amounts;
    Type valType;
};
#endif //LEDGERJRI_LEDGERVALUE_H
