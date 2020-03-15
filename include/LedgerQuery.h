#ifndef LEDGERJRI_LEDGERQUERY_H
#define LEDGERJRI_LEDGERQUERY_H

#include <LedgerQueryInterface.h>

class LedgerSession;
class LedgerQueryInstanceData;

namespace LedgerQueryHandlers {
    void DoQuery(
            AccountQuery& query,
            LedgerSession& sess,
            LedgerQueryInstanceData& qryVars);
}


#endif //LEDGERJRI_LEDGERQUERY_H
