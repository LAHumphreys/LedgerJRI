#include <gtest/gtest.h>
#include "TestUtils.h"
#include "BalancesJSON.h"
#include <util_time.h>
#include <Ledger.h>

class BalanceTest: public ::testing::Test {
public:
    BalanceTest() {}
protected:
    std::unique_ptr<LedgerSession> LoadJournalWithContents(const std::string& jnl) {
        nstimestamp::Time stamp;
        stamp.SetNow();
        const std::string path = "TempJournal." + stamp.Timestamp() + ".jnl";
        TestFile tmp(path, jnl);
        return Ledger::LoadJournal(path);
    }
};

TEST_F(BalanceTest, NoJournal) {
    auto session = Ledger::LoadJournal("");

    ASSERT_FALSE(session->IsOk());
}

TEST_F(BalanceTest, NoSuchJournal) {
    auto session = Ledger::LoadJournal("NotAJnl");

    ASSERT_FALSE(session->IsOk());
}

TEST_F(BalanceTest, BadJournal) {
    std::string jnl = R"JNL(
        2020-01-03 * Openning Balance ; Bad leading whitespace...
          Assets:CurrentAccount       £173.92
          Assets:TicketFund           £0
          Equity:OpeningBalances      ;
    )JNL";
    auto session = LoadJournalWithContents(jnl);
    ASSERT_FALSE(session->IsOk());
}

TEST_F(BalanceTest, SingleValidJournal) {
    std::string jnl = R"JNL(
2020-01-03 Opening Bal
    Assets:CurrentAccount       £173.92
    Assets:TicketFund           £0
    Equity:OpeningBalances      ;
)JNL";
    auto session = LoadJournalWithContents(jnl);
    ASSERT_TRUE(session->IsOk()) << session->GetState();


    std::vector<BalAccount> balAccs = {
        BalAccount{}.withName("Assets").withValue("£", 173.92).withChildren({
                BalAccount{}.withName("CurrentAccount").withValue("£", 173.92),
                BalAccount{}.withName("TicketFund"),
            }),
        BalAccount{}.withName("Equity").withValue("£", -173.92).withChildren({
                BalAccount{}.withName("OpeningBalances").withValue("£", -173.92),
            })
    };
Ledger::WithSession(*session, [&] (LedgerCommands& cmds) {
        const std::string result = cmds.BalanceReport({"--empty", "-T", "O"});
        ASSERT_NO_FATAL_FAILURE(AssertJSONMatches(result, balAccs));
    });
}
