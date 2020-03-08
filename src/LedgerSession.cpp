#include <LedgerSession.h>
#include <LedgerQuery.h>
#include <iostream>

LedgerSession::LedgerSession(Ledger &instance)
    : data (new LedgerSessionData)
{
}

LedgerSession::~LedgerSession() {
    delete data;
}

bool LedgerSession::IsOk() const {
    return (GetState().state == StateModel::READY);
}


void LedgerSession::BadJournal(Ledger &, std::string errText) {
    state.state = StateModel::BAD_JOURNAL;
    state.reasonText = errText;
    std::cerr << "Failed to parse journal: " << errText << std::endl;
}

void LedgerSession::SessionInitialised(Ledger &) {
    state.state = StateModel::READY;
    state.reasonText.clear();
}

void LedgerSession::DoReport(Ledger &, // we must have the instance lock
                             AccountQuery &qry,
                             const std::vector<std::string> &args)
{
    auto qryVars = data->NewReportInvocation();
    ledger::strings_list ledgerArgs;
    for (const std::string& arg: args) {
        ledgerArgs.push_back(arg);
    }
    ledgerArgs = ledger::process_arguments(ledgerArgs, data->report);

    for (const auto& arg: ledgerArgs) {
        qryVars->reportArgsScope.push_back(ledger::string_value(arg));
    }

    data->report.normalize_options(qry.Verb());

    AccountQueryHandler::DoQuery(qry, *this, *qryVars);
}

std::ostream& operator<<(std::ostream& os, const LedgerSession::State& state) {
    using SM = LedgerSession::StateModel;
    switch (state.state) {
        case SM::NOT_INITIALISED:
            os << "NOT_INITIALISED";
            break;
        case SM::READY:
            os << "READY";
            break;
        case SM::BAD_JOURNAL:
            os << "BAD_JOURNAL";
            break;
    }
    if (!state.reasonText.empty()) {
        os << ", reason text: '" << state.reasonText << "'";
    }
    return os;
}
