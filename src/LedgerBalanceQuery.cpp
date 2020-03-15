#include "LedgerBalanceQuery.h"
#include <LedgerSession.h>
#include <LedgerAccount.h>
#include <LedgerValue.h>

LedgerBalanceHandler::LedgerBalanceHandler(LedgerSession &sess)
    : sess(sess)
{
    builder.StartAccountsArray("accounts");
    ++depth;
}

void LedgerBalanceHandler::NextAccount(ledger::account_t &item) {
    const auto itemDepth = LedgerAccount::depth(item);
    const std::string name = LedgerAccount::name(item);

    ++count;
    if (count == 1) {
        // This will be the first anonymous object, no need to do anything...
    } else if (depth == itemDepth) {
        // Last object left its array open. We're at the same level so:
        //    - close the array - its empty
        //    - close the object
        builder.EndAccountsArray();
        builder.EndAccount();
    } else if (depth > itemDepth) {
        while (depth > itemDepth) {
            // close object at this level
            builder.EndAccountsArray();
            builder.EndAccount();

            // close parent - we're about to start a new object parallel to it
            builder.EndAccountsArray();
            builder.EndAccount();
            --depth;
        }
    } else {
        if (depth < itemDepth) {
            // Child of the previous object we inspected
            //    Array is already open - nothing to do
            ++depth;
        }

        if (depth < itemDepth) {
            // We need a child to add a grandchild to...
            throw "generational skip!";
        }
    }

    builder.StartAccount(name);
    builder.AddValue(*sess.CalculateDisplayTotal(item));
    builder.StartAccountsArray();
}

void LedgerBalanceHandler::QueryCompleted() {
    while (depth >= 1) {
        builder.EndAccountsArray();
        builder.EndAccount();
        --depth;
    }
    builder.EndAccountsArray();

    result = builder.GetJSON();
}
