#include <LedgerCommands.h>

#include <LedgerSession.h>
#include <LedgerBalanceQuery.h>


LedgerCommands::LedgerCommands(Ledger &instance, LedgerSession& sess)
   : ledgerInstance(instance)
   , sess(sess)
{
}

std::string LedgerCommands::BalanceReport(const LedgerCommands::Args &args) {
    LedgerBalanceHandler query(sess);

    sess.DoReport(ledgerInstance, query, args);

    return query.GetResult();
}
