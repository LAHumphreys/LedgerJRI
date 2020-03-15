#include <sstream>
#include <LedgerSession.h>
#include <gtest/gtest.h>

TEST(LedgerSessionStateName, NOT_INITIALISED) {
    std::stringstream buf;
    LedgerSession::State state{
        LedgerSession::StateModel::NOT_INITIALISED,
        ""
    };
    buf << state;

    ASSERT_EQ(buf.str(), "NOT_INITIALISED");
}

TEST(LedgerSessionStateName, READY) {
    std::stringstream buf;
    LedgerSession::State state{
            LedgerSession::StateModel::READY,
            ""
    };
    buf << state;

    ASSERT_EQ(buf.str(), "READY");
}

TEST(LedgerSessionStateName, BAD_JOURNAL) {
    std::stringstream buf;
    LedgerSession::State state{
            LedgerSession::StateModel::BAD_JOURNAL,
            ""
    };
    buf << state;

    ASSERT_EQ(buf.str(), "BAD_JOURNAL");
}

TEST(LedgerSessionStateName, REASON_TEXT) {
    std::stringstream buf;
    const std::string reason = "Hey! There's a reason for this nonsense you know!";
    LedgerSession::State state{
            LedgerSession::StateModel::NOT_INITIALISED,
            reason
    };
    buf << state;

    const std::string expected = "NOT_INITIALISED, reason text: 'Hey! There's a reason for this nonsense you know!'";

    ASSERT_EQ(buf.str(), expected);
}
