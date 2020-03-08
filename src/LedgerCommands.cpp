#include <LedgerCommands.h>

#include <ledger/system.hh>
#include <ledger/report.h>
#include <LedgerSession.h>
#include <LedgerSessionData.h>
#include <LedgerQuery.h>

#include <SimpleJSON.h>

namespace {

    void writeValue(SimpleJSONPrettyBuilder& build, ledger::value_t& val) {
        build.AddName("value");
        build.StartAnonymousObject();
        if (val.is_amount()) {
            const ledger::amount_t& amount = val.as_amount();
            const ledger::commodity_t& comm = amount.commodity();

            build.Add("type", std::string("VAL_AMOUNT"));
            build.AddName("amount");
            build.StartAnonymousObject();
                build.Add("value_amount", amount.to_double());
            build.Add("commodity_key", comm.symbol());
            build.EndObject();
        } else if (val.is_zero()) {
            build.Add("type", std::string("VAL_ZERO"));
        }
        build.EndObject();
    }

    class BalanceHandler: public AccountQuery {
    public:
        BalanceHandler(std::ostream& out, ledger::report_t& report)
            : out(out)
            , report(report)
        {
            json.StartArray("accounts");
            ++depth;
        }


        std::string result = "{ }";
        SimpleJSONPrettyBuilder json;
        size_t depth = 0;
        size_t count = 0;

        const std::string& GetResult() const {
            std::cout <<"RESULT: " << std::endl << result << std::endl;
            return result;
        };

        void NextAccount(ledger::account_t& item) override {
            ++count;
            if (count == 1) {
                // This will be the first anonymous object, no need to do anything...
            } else if (depth == item.depth) {
                // Last object left its array open. We're at the same level so:
                //    - close the array - its empty
                //    - close the object
                json.EndArray();
                json.EndObject();
            } else if (depth > item.depth) {
                while (depth > item.depth) {
                    // close object at this level
                    json.EndArray();
                    json.EndObject();

                    // close parent - we're about to start a new object parallel to it
                    json.EndArray();
                    json.EndObject();
                    --depth;
                }
            } else {
                if (depth < item.depth) {
                    // Child of the previous object we inspected
                    //    Array is already open - nothing to do
                    ++depth;
                }

                if (depth < item.depth) {
                    // We need a child to add a grandchild to...
                    throw "generational skip!";
                }
            }

            ledger::bind_scope_t boundScope(report, item);
            ledger::call_scope_t callScope(boundScope);
            ledger::value_t total = report.display_value(report.fn_display_total(callScope));

            json.StartAnonymousObject();
            json.Add("name", item.name);
            writeValue(json, total);
            json.StartArray("accounts");
        }

        void QueryCompleted() override {
            while (depth >= 1) {
                json.EndArray();
                json.EndObject();
                --depth;
            }
            json.EndArray();
            result = json.GetAndClear();
        }

        const std::string& Verb() override { return verb; }
    private:

        std::ostream& out;
        ledger::report_t& report;
        const std::string verb = "balance";
    };
}

LedgerCommands::LedgerCommands(Ledger &instance, LedgerSession& sess)
   : ledgerInstance(instance)
   , sess(sess)
{
}

std::string LedgerCommands::BalanceReport(const LedgerCommands::Args &args) {
    std::stringstream out;

    BalanceHandler query(out, sess.Data().report);

    sess.DoReport(ledgerInstance, query, args);

    return query.GetResult();
}
