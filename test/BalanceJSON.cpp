#include "BalancesJSON.h"
#include <gtest/gtest.h>

namespace BalanceJSON {
    NewStringField(fullname);
    NewStringField(name);

    NewStringField(type);
    NewStringField(commodity_key);

    NewDoubleField(value_amount);

    typedef SimpleParsedJSON<commodity_key, value_amount> AmountJSON;
    NewEmbededObject(amount, AmountJSON);

    typedef SimpleParsedJSON<amount, type> ValueJSON;
    NewEmbededObject(value, ValueJSON);

    namespace child {
        typedef SimpleParsedJSON<fullname, name, value> ChildAccountsJSON;
        NewObjectArray(accounts, ChildAccountsJSON);
    }

    typedef SimpleParsedJSON<fullname, name, value, child::accounts> AccountsJSON;
    NewObjectArray(accounts, AccountsJSON);

    namespace root {
        typedef SimpleParsedJSON<BalanceJSON::accounts, fullname, name, value> AccountsJSON;
        NewObjectArray(accounts, AccountsJSON);
    }

    typedef SimpleParsedJSON<root::accounts> BalanceReport;

}

template<class JSONVal>
void AssertValEq(JSONVal& val, const BalAccount& bal) {
    using namespace BalanceJSON;
    ASSERT_EQ(val.template Get<type>(), bal.valType);
    if (bal.valType == "VAL_ZERO") {
        ASSERT_FALSE(val.template Supplied<amount>());
    } else if (bal.valType == "VAL_AMOUNT") {
        ASSERT_TRUE(val.template Supplied<amount>());
        auto& am = val.template Get<amount>();
        ASSERT_FLOAT_EQ(am.template Get<value_amount>(), bal.value);
        ASSERT_EQ(am.template Get<commodity_key>(), bal.commodityKey);
    } else {
        throw "WTF is this thing?";
    }
}


void AssertJSONMatches(const std::string& json, const std::vector<BalAccount>& balAccs) {
    using namespace BalanceJSON;
    BalanceReport parser;

    std::string empty = parser.GetPrettyJSONString();
    std::string err;
    ASSERT_TRUE(parser.Parse(json.c_str(), err)) << err << std::endl << json << std::endl << empty;

    std::stringstream logMsg;

    logMsg << "Expected:" << std::endl;
    for (size_t i = 0; i < balAccs.size(); ++i) {
        logMsg << "    [" << i << "] name:    " << balAccs[i].name << std::endl;
        logMsg << "    [" << i << "] value:   " << balAccs[i].value << std::endl;
        logMsg << "    [" << i << "] symbol:  " << balAccs[i].commodityKey << std::endl;
        logMsg << "    [" << i << "] valType: " << balAccs[i].valType << std::endl;
        logMsg << "    [" << i << "] children: " <<  std::endl;
        for (size_t j = 0; j < balAccs[i].children.size(); ++j) {
            logMsg << "        [" << j << "] name:    " << balAccs[i].children[j].name << std::endl;
            logMsg << "        [" << j << "] value:   " << balAccs[i].children[j].value << std::endl;
            logMsg << "        [" << j << "] symbol:  " << balAccs[i].children[j].commodityKey << std::endl;
            logMsg << "        [" << j << "] valType: " << balAccs[i].children[j].valType << std::endl;
        }
        logMsg << std::endl;
    }
    logMsg << "Received:" << std::endl;
    logMsg << parser.GetPrettyJSONString() << std::endl;
    logMsg << std::endl;

    ASSERT_EQ(parser.Get<root::accounts>().size(), balAccs.size()) << logMsg.str();
    for (size_t i = 0; i < balAccs.size(); ++i) {
        const auto& exp = balAccs[i];
        auto& got = *parser.Get<root::accounts>()[i];
        auto& gval = got.Get<value>();

        ASSERT_EQ(got.Get<name>(), exp.name) << logMsg.str();
        ASSERT_EQ(got.Get<accounts>().size(), exp.children.size()) << logMsg.str();
        ASSERT_NO_FATAL_FAILURE(AssertValEq(gval, exp)) << logMsg.str();


        for (size_t j = 0; j < balAccs[i].children.size(); ++j) {
            const auto& cexp = balAccs[i].children[j];
            auto& cgot = *got.Get<accounts>()[j];
            auto& cval = cgot.Get<value>();

            ASSERT_EQ(cgot.Get<name>(), cexp.name) << logMsg.str();
            ASSERT_NO_FATAL_FAILURE(AssertValEq(cval, cexp)) << logMsg.str();
        }
    }

}
