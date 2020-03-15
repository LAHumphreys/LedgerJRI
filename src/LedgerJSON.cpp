#include <LedgerJSON.h>
#include <SimpleJSON.h>

#include <forward_list>
#include <LedgerValue.h>

struct ParserError: std::exception {
};
struct StateModelError: ParserError {
    const char*what() const noexcept override { return "Internal parser state error";}
};
struct NoOpenAccount: ParserError {
    const char*what() const noexcept override { return "There is no open account";}
};
struct NoOpenAccountArray: ParserError {
    const char*what() const noexcept override { return "There is no open array to add the account to";}
};


struct LedgerJSONOpaqueData {
    SimpleJSONPrettyBuilder builder;
    class WriterState {
    public:
        enum class State {
            EMPTY,
            READY,
            OPEN_ACCOUNT,
            OPEN_ACCOUNTS_ARRAY
        };
        const State& CurrentState() const { return state;}
        const State& ParentState() const { return *stateStack.begin();}

        void PushStateOntoStack(const State& s) {
            stateStack.push_front(state);
            state = s;
        }
        void PopState(const State& s) {
            if (state != s) {
                throw StateModelError{};
            } else {
                state = *stateStack.begin();
                stateStack.pop_front();
            }
        }
    private:
        State state = State::READY;
        std::forward_list<State> stateStack { State::EMPTY};
    };
    WriterState state;
};

using WS = LedgerJSONOpaqueData::WriterState::State ;

LedgerJSON::LedgerJSON() {
    opaqueData = new LedgerJSONOpaqueData;
}

LedgerJSON::~LedgerJSON() {
    delete opaqueData;
}

void LedgerJSON::StartAccountsArray(const std::string &name) {
    switch (opaqueData->state.CurrentState()) {
        case WS::READY:
        case WS::OPEN_ACCOUNT:
            opaqueData->builder.StartArray(name);
            opaqueData->state.PushStateOntoStack(WS::OPEN_ACCOUNTS_ARRAY);
            break;
        case WS::OPEN_ACCOUNTS_ARRAY:
            [[fallthrough]];
        case WS::EMPTY:
            throw NoOpenAccount{};
    }
}

void LedgerJSON::StartAccount(const std::string &name) {
    switch (opaqueData->state.CurrentState()) {
        case WS::OPEN_ACCOUNTS_ARRAY:
            opaqueData->builder.StartAnonymousObject();
            opaqueData->builder.Add("name", name);
            opaqueData->state.PushStateOntoStack(WS::OPEN_ACCOUNT);
            break;

        case WS::EMPTY:
            [[fallthrough]];
        case WS::OPEN_ACCOUNT:
            [[fallthrough]];
        case WS::READY:
            throw NoOpenAccountArray{};
    }
}

void LedgerJSON::EndAccount() {
    switch (opaqueData->state.CurrentState()) {
        case WS::OPEN_ACCOUNT:
            opaqueData->builder.EndObject();
            opaqueData->state.PopState(WS::OPEN_ACCOUNT);
            break;

        case WS::EMPTY:
        case WS::OPEN_ACCOUNTS_ARRAY:
            [[fallthrough]];
        case WS::READY:
            throw NoOpenAccount{};
    }
}

void LedgerJSON::EndAccountsArray() {
    switch (opaqueData->state.CurrentState()) {
        case WS::OPEN_ACCOUNTS_ARRAY:
            opaqueData->builder.EndArray();
            opaqueData->state.PopState(WS::OPEN_ACCOUNTS_ARRAY);
            break;

        case WS::EMPTY:
        case WS::OPEN_ACCOUNT:
            [[fallthrough]];
        case WS::READY:
            throw NoOpenAccountArray{};
    }

}

void LedgerJSON::AddValue(const LedgerValue &val, const std::string &name) {
    auto& build = opaqueData->builder;
    build.AddName(name);
    build.StartAnonymousObject();
    switch (val.ValueType()) {
        case LedgerValue::Type::AMOUNT:
            build.Add("type", std::string("VAL_AMOUNT"));
            build.AddName("amount");
            build.StartAnonymousObject();
            build.Add("value_amount", val.Amounts()[0].DoubleVal());
            build.Add("commodity_key", val.Amounts()[0].Commodity().Symbol());
            build.EndObject();
            break;
        case LedgerValue::Type::ZERO:
            build.Add("type", std::string("VAL_ZERO"));
            break;
    }
    build.EndObject();

}

std::string LedgerJSON::GetJSON() {
    switch (opaqueData->state.CurrentState()) {
        case WS::READY:
            return opaqueData->builder.GetAndClear();

        case WS::OPEN_ACCOUNTS_ARRAY:
            [[fallthrough]];
        case WS::OPEN_ACCOUNT:
            [[fallthrough]];
        case WS::EMPTY:
            throw StateModelError{};
    }
    throw StateModelError{};
}
