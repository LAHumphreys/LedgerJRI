#ifndef LEDGERJRI_LEDGERQUERY_H
#define LEDGERJRI_LEDGERQUERY_H

#include <string>
#include <LedgerSessionData.h>
#include <ledger/system.hh>
#include <ledger/report.h>
#include <LedgerSession.h>

class AccountQuery {
public:
    virtual const std::string& Verb() = 0;
    virtual void NextAccount(ledger::account_t& item) = 0;
    virtual void QueryCompleted() = 0;
};

class AccountQueryHandler: public ledger::item_handler<ledger::account_t> {
public:
    static void DoQuery(AccountQuery& query,
                        LedgerSession& sess,
                        LedgerSessionData::InvocationInstance& qryVars);
private:
    AccountQueryHandler(AccountQuery& processor);

    AccountQuery& queryProcessor;

    virtual void flush() override;
    virtual void operator()(ledger::account_t& item) override;

    // TODO: Do we actually needs these call-backs?
    virtual void title(const std::string& str) {}
    virtual void clear() override { }

};

#endif //LEDGERJRI_LEDGERQUERY_H
