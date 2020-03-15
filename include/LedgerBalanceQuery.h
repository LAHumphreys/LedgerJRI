//
// Created by lhumphreys on 15/03/2020.
//

#ifndef LEDGERJRI_LEDGERBALANCEQUERY_H
#define LEDGERJRI_LEDGERBALANCEQUERY_H
#include <LedgerQueryInterface.h>
#include <LedgerJSON.h>
#include <string>


class LedgerSession;


class LedgerBalanceHandler: public AccountQuery {
public:
    LedgerBalanceHandler(LedgerSession& sess);

    const std::string& GetResult() const { return result; };
    const std::string& Verb() override { return verb; }

    void NextAccount(ledger::account_t& item) override;

    void QueryCompleted() override;

private:
    LedgerSession& sess;
    LedgerJSON builder;

    size_t depth = 0;
    size_t count = 0;

    std::string result = "{ }";
    const std::string verb = "balance";
};

#endif //LEDGERJRI_LEDGERBALANCEQUERY_H
