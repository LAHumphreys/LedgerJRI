#include "LedgerAmount.h"

#include <ledger/system.hh>
#include <ledger/amount.h>

LedgerAmount::LedgerAmount(const ledger::amount_t &amount)
    : commodity(amount.commodity())
    , val(amount.to_double())
{
}
