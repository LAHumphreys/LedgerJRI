#include <Ledger.h>
#include <LedgerSessionData.h>
#include <LedgerSession.h>
#include <LedgerCommands.h>

Ledger::Ledger() {
    // libLedger one-time setup?
}

std::unique_ptr<LedgerSession> Ledger::LoadJournal(const std::string& fname) {
    std::unique_ptr<LedgerSession> sess;
    WithInstance([&] (Ledger& instance) {
        sess = std::make_unique<LedgerSession>(instance);

        instance.SwitchToSession(*sess);

        const char ** env = const_cast<const char**>(environ);
        sess->Data().sess.set_flush_on_next_data_file(true);
        ledger::process_environment(env, "LEDGER_", sess->Data().report);
        sess->Data().sess.set_flush_on_next_data_file(true);

        bool initialised = false;
        try {
            sess->Data().sess.read_journal(fname);
            initialised = true;
        } catch (ledger::error_count& ec) {
            sess->BadJournal(instance, ec.what());
        } catch (const std::runtime_error& err) {
            sess->BadJournal(instance, err.what());
        }

        if (initialised) {
            sess->SessionInitialised(instance);
        }
    });
    return sess;
}

void Ledger::WithInstance(const std::function<void(Ledger &instance)> &cb) {
    static Ledger instance;
    std::unique_lock<std::mutex> lock(instance.globalLock);
    cb(instance);
}

void Ledger::SwitchToSession(LedgerSession&sess) {
    ledger::set_session_context(&sess.Data().sess);

    ledger::scope_t::default_scope = &sess.Data().report;
    ledger::scope_t::empty_scope = &sess.Data().emptyScope;
}

void Ledger::WithSession(LedgerSession &sess,
                         const std::function<void(LedgerCommands &)>& task)
{
    WithInstance([&] (Ledger& instance) {
        instance.SwitchToSession(sess);
        LedgerCommands cmdHandler(instance, sess);
        task(cmdHandler);
    });
}


