#include <LedgerQuery.h>

AccountQueryHandler::AccountQueryHandler(AccountQuery& processor)
    : queryProcessor(processor)
{

}

void AccountQueryHandler::DoQuery(
        AccountQuery& query,
        LedgerSession& sess,
        LedgerSessionData::InvocationInstance& qryVars)
{

    using Reporter = ledger::reporter<ledger::account_t,
            ledger::acct_handler_ptr,
            &ledger::report_t::accounts_report>;

    boost::shared_ptr<ledger::item_handler<ledger::account_t>> handler;
    std::stringstream out;
    handler.reset(new AccountQueryHandler(query));
    Reporter reporter(handler, sess.Data().report, std::string("#") + query.Verb());

    reporter(qryVars.reportArgsScope);
}

void AccountQueryHandler::flush() {
    queryProcessor.QueryCompleted();
}

void AccountQueryHandler::operator()(ledger::account_t& item) {
    queryProcessor.NextAccount(item);
}

