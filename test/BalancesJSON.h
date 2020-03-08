#include <SimpleJSON.h>

struct BalAccount {
    std::string name;
    double value = 0.0;
    std::string valType = "VAL_ZERO";
    std::string commodityKey = "";

    std::vector<BalAccount> children;

    BalAccount() = default;
    BalAccount& withName(std::string n) {
        this->name = n;
        return *this;
    }
    BalAccount& withChildren(std::vector<BalAccount> c) {
        this->children = std::move(c);
        return *this;
    }
    BalAccount& withValue(const std::string& commodity, const double& val) {
        this->valType = "VAL_AMOUNT";
        this->commodityKey = commodity;
        this->value = val;
        return *this;
    }
};

void AssertJSONMatches(const std::string& json, const std::vector<BalAccount>& balAccs);

