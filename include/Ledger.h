#ifndef LEDGERJRI_LEDGER_H
#define LEDGERJRI_LEDGER_H

#include <memory>
#include <string>
#include <functional>
#include <mutex>

class LedgerCommands;
class LedgerSession;

/**
 * Access point to the Ledger CLI library.
 *
 * The related LedgerCommands object defines the API used to interogate
 *
 * A note on sessions and concurrency
 * -----------------------------------
 * libLedger is explicitly *not* thread-safe - the library has a collection of
 * statics which means it is only valid to process one sesssion (and hence one
 * journal) at a time. *However*, as per the note on set_session_context:
 *
 *  //
 *  // Set the current session context, transferring all static globals to point
 *  // at the data structures related to this session.  Although Ledger itself is
 *  // not thread-safe, by locking, switching session context, then unlocking
 *  // after an operation is done, multiple threads can sequentially make use of
 *  // the library.  Thus, a session_t maintains all of the information relating
 *  // to a single usage of the Ledger library.
 *  //
 *  void set_session_context(session_t * session);
 *
 *  In order to enforce this model within our API:
 *    1. It is only possible to interact with libLedger via the Ledger singleton,
 *       access to which is policed by a global mutex.
 *    2. State for individual "sessions" are held in LedgerSession objects. In
 *       practical terms, there'll be one session per parsed Journal File.
 *
 * Example Usage
 * -------------
 * auto session = Ledger::LoadJournal("test.jnl");
 *
 * if (session->IsOk()) {
 *     Ledger::WithSession(*session, [] (LedgerCommands& commands) {
 *         std::cout << commands.BalanceReport({"--empty"});
 *     });
 * } else {
 *     // error handling
 * }
 */
class Ledger {
public:
    static std::unique_ptr<LedgerSession> LoadJournal(const std::string& fname);

    static void WithSession(LedgerSession& sess, const std::function<void (LedgerCommands&)>&);

private:
    static void WithInstance(const std::function<void (Ledger& instance)>& cb);

    void SwitchToSession(LedgerSession& sess);

    Ledger();
    Ledger(const Ledger&) = delete;
    Ledger& operator=(const Ledger&) = delete;

    std::mutex globalLock;
};

#endif //LEDGERJRI_LEDGER_H
