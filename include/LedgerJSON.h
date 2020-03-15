//
// Created by lhumphreys on 13/03/2020.
//

#ifndef LEDGERJRI_LEDGERJSON_H
#define LEDGERJRI_LEDGERJSON_H

#include <string>
struct LedgerJSONOpaqueData;
class LedgerValue;

namespace ledger {
    class value_t;
}

class LedgerJSON {
public:
    LedgerJSON();
    ~LedgerJSON();

    void StartAccountsArray(const std::string& name = "accounts");
    void EndAccountsArray();

    void StartAccount(const std::string& name);
    void EndAccount();

    void AddValue(const LedgerValue& val, const std::string& name = "value");

    std::string GetJSON();
private:
    LedgerJSONOpaqueData* opaqueData;

};

#endif //LEDGERJRI_LEDGERJSON_H
