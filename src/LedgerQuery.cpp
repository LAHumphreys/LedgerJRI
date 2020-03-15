#include <LedgerQuery.h>
#include <LedgerSession.h>
#include <LedgerSessionData.h>

namespace {
    class AccountQueryHandler: public ledger::item_handler<ledger::account_t> {
    public:
        AccountQueryHandler(AccountQuery& processor)
            : queryProcessor(processor) {}

        AccountQuery& queryProcessor;

        virtual void flush() override {
            queryProcessor.QueryCompleted();
        }
        virtual void operator()(ledger::account_t& item) override {
            queryProcessor.NextAccount(item);
        }

        // TODO: Do we actually needs these call-backs?
        virtual void title(const std::string& str) {}
        virtual void clear() override { }

    };
}

void LedgerQueryHandlers::DoQuery(
        AccountQuery& query,
        LedgerSession& sess,
        LedgerQueryInstanceData& qryVars)
{

    using Reporter = ledger::reporter<ledger::account_t,
            ledger::acct_handler_ptr,
            &ledger::report_t::accounts_report>;

    boost::shared_ptr<ledger::item_handler<ledger::account_t>> handler;
    handler.reset(new AccountQueryHandler(query));

    Reporter reporter(handler, sess.Data().report, std::string("#") + query.Verb());

    reporter(qryVars.reportArgsScope);
}

