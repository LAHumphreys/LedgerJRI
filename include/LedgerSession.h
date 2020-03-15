#ifndef LEDGERJRI_LEDGERSESSION_H
#define LEDGERJRI_LEDGERSESSION_H
#include <string>
#include <vector>
#include <memory>

class Ledger;
class LedgerSessionData;
class AccountQuery;
class LedgerValue;

namespace ledger {
    class scope_t;
    class account_t;
}

/**
 * This is an internal session object, which holds the state associated with
 * an active ledger cli session.
 *
 * In order to avoid boost polluting our namespace, an opaque LedgerSessionData
 * hold our actual state.
 *
 * For further details, see the notes in Ledger.h
 *
 */
class LedgerSession {
public:
//PUBLIC API
    /**
     * Check whether the session has correctly  initialised, and is still usable
     */
    bool IsOk() const;

    enum class StateModel {
        NOT_INITIALISED,
        READY,
        BAD_JOURNAL
    };
    struct State {
        StateModel  state = StateModel::NOT_INITIALISED;
        std::string reasonText;
    };

    const State& GetState() const { return state;}


//API Used by Ledger Instance

     // Only the Ledger singleton itself is able to create a new session.
     // See Ledger::LoadJournal
    LedgerSession(Ledger& instance);

    // Re-initialize the session, and run a specifc query
    void DoReport(Ledger& instance,
                  AccountQuery& qry,
                  const std::vector<std::string>& args);

    // Calculate display total
    std::unique_ptr<LedgerValue> CalculateDisplayTotalFromScope(
            ledger::scope_t& item);
    std::unique_ptr<LedgerValue> CalculateDisplayTotal(
            ledger::account_t& item);

    // Access to the opaque data object
    LedgerSessionData& Data() { return *data; }

    // Call back from the Ledger instance, once the session has been correctly initialised
    void SessionInitialised(Ledger&);

    // Call back from the Ledger instance, in the event that the journal could not be loaded
    void BadJournal(Ledger&, std::string errText);

    ~LedgerSession();
private:
    State state;
    LedgerSessionData* const data;
};

std::ostream& operator<<(std::ostream& os, const LedgerSession::State& state);


#endif //LEDGERJRI_LEDGERSESSION_H
