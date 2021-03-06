#ifndef LEDGERJRI_LEDGERSESSIONDATA_H
#define LEDGERJRI_LEDGERSESSIONDATA_H

#include <ledger/system.hh>
#include <ledger/report.h>

/**
 * This is the only header file permitted to pull in header
 * files from the ledger library. Since libLedger makes use
 * of boost its subject to intense compilation and slow
 * build times.
 *
 * In order to avoid polluting the application layer, only
 * Ledger*.cpp files will #include the SessionData.
 *
 */
struct LedgerQueryInstanceData;
struct LedgerSessionData {
// Global data - persistent across the lifetime of the session
    ledger::session_t sess;
    ledger::report_t report;

    ledger::empty_scope_t emptyScope;

    LedgerSessionData() : report(sess) {}
};

// Data that must be reset each time a new query is started.
//    (Whilst the report is persisted, and simply reset  for the new mode,
//     command line arguments / report configuration is specific to an
//     individual query)
struct LedgerQueryInstanceData {
    ledger::bind_scope_t reportScope;
    ledger::call_scope_t reportArgsScope;

    LedgerQueryInstanceData(LedgerSessionData& sess)
            : reportScope(sess.sess, sess.report)
            , reportArgsScope(reportScope) {}
};

#endif //LEDGERJRI_LEDGERSESSIONDATA_H
