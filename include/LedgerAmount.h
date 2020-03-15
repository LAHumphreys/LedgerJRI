#ifndef LEDGERJRI_LEDGERAMOUNT_H
#define LEDGERJRI_LEDGERAMOUNT_H

#include "LedgerCommodity.h"

namespace ledger {
    class amount_t;
}

class LedgerAmount {
public:
    LedgerAmount(const ledger::amount_t& amount);

    const double& DoubleVal() const { return val; }

    const LedgerCommodity& Commodity() const { return commodity; }

private:
    const LedgerCommodity commodity;
    const double          val;
};


#endif //LEDGERJRI_LEDGERAMOUNT_H
