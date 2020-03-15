#include <LedgerSession.h>
#include <LedgerSessionData.h>
#include <LedgerQuery.h>
#include <LedgerValue.h>
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
    ledger::strings_list ledgerArgs;
    for (const std::string& arg: args) {
        ledgerArgs.push_back(arg);
    }
    ledgerArgs = ledger::process_arguments(ledgerArgs, data->report);

    LedgerQueryInstanceData qryVars(Data());
    for (const auto& arg: ledgerArgs) {
        qryVars.reportArgsScope.push_back(ledger::string_value(arg));
    }

    data->report.normalize_options(qry.Verb());

    LedgerQueryHandlers::DoQuery(qry, *this, qryVars);
}

std::unique_ptr<LedgerValue> LedgerSession::CalculateDisplayTotalFromScope(ledger::scope_t &item) {
    ledger::bind_scope_t boundScope(data->report, item);
    ledger::call_scope_t callScope(boundScope);
    ledger::value_t total = data->report.display_value(data->report.fn_display_total(callScope));

    return std::make_unique<LedgerValue>(total);
}

std::unique_ptr<LedgerValue> LedgerSession::CalculateDisplayTotal(ledger::account_t &item) {
    return CalculateDisplayTotalFromScope(item);
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
