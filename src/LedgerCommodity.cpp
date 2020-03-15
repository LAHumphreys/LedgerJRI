#include "LedgerCommodity.h"

#include <ledger/system.hh>
#include <ledger/commodity.h>

LedgerCommodity::LedgerCommodity(const ledger::commodity_t &comm)
   : symbol(comm.symbol())
{
}

