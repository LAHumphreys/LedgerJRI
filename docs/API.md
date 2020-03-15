# Requests
## Load a Journal: REQ_LOAD_JOURNAL
Ask the query daemon to (re)load a Ledger file.

### Request:

```JSON
{
     "journalName": "/home/lah/personal/budget/2020.jnl"
}
```

Request Field    | Type       | Valid Values    | Description
-----------------|------------|-----------------|------------
**journalName**  |string      | File-system path| The path to the journal to load

### Response:

#### On success:

```JSON
{
     "status":  {
         "state":  "READY",
         "reason":  null
     },
     "sessionId":   1
}
```

#### On Failure

```JSON
{
     "status":  {
         "state":  "BAD_JOURNAL",
         "reason": "No such file: '/home/lah/personal/budget/2020.jnl'"
     },
     "sessionId":   null
}
```

#### Full definition:

Response Field   | Type                        | Description
-----------------|-----------------------------|-----------------
**status**       | [Status](#status-object)    | Indicates success or failure of the load.
**sessionId**    | unsigned-int                | If successful, the unique session id to be used when interrogating the loaded journal

## Run a Balance report: REQ_BALANCE_REPORT

Run a balance report against the ledger - see the Ledger CLI documentation for further details

### Request:

```JSON
{
     "sessionId":  1,
     "total"    : "O",
     "empty"    : true
}
```

Request Field    | Type       | Default         |Valid Values    | Description
-----------------|------------|-----------------|----------------|-------------
**sessionId**    |unsigned-int| MANDATORY       |                | A sessionId returned by REQ_LOAD_JOURNAL
**total**        |string      |                 | see total-expr | A total_expr, as defined by Ledger CLI
**empty**        |boolean     | false           |                | If true, include accounts whose total is zero.

### Response:

```JSON
{
    "accounts": [ {
         "name":     "Assets",
         "value":    {
             "type":    "VAL_AMOUNT",
             "amount":  {
                 "commodity_key": "£",
                 "value_amount":  173.92
             }
         },
         "accounts":    [{
             "name":     "CurrentAccount",
             "value":    {
                 "type":    "VAL_AMOUNT",
                 "amount":  {
                     "commodity_key": "£",
                     "value_amount":  173.92
                 }
             },
             "accounts": []
         }, {
             "name":     "CurrentAccount",
             "value":    {
                 "type":    "VAL_ZERO"
             },
             "accounts": []
         }]
    } , {
         "name":     "Equity",
         "value":    {
             "type":    "VAL_AMOUNT",
             "amount":  {
                 "commodity_key": "£",
                 "value_amount":  -173.92
             }
         },
         "accounts":    [{
             "name":     "OpeningBalances",
             "value":    {
                 "type":    "VAL_AMOUNT",
                 "amount":  {
                     "commodity_key": "£",
                     "value_amount":  -173.92
                 }
             },
             "accounts": []
         }]
    }]
}
```

#### Full definition:

Response Field   | Type                                    | Description
-----------------|-----------------------------------------|-----------------
**accounts**     | Array of [accounts](#account-object)    | The list of top level accounts in the report

# API Objects

## Account Object

The account object is returned be queries that report on a per-account basis. It details the state of the account at a
given point in time.

### Example

```JSON
{
     "name":     "Assets",
     "value": {
         "type":    "VAL_AMOUNT",
         "amount": {
             "commodity_key": "£",
             "value_amount":  173.92
         }
     },
     "accounts": [
         {
             "name":     "CurrentAccount",
             "value": {
                 "type":    "VAL_AMOUNT",
                 "amount": {
                     "commodity_key": "£",
                     "value_amount":  173.92
                 }
             },
             "accounts": []
         }, {
             "name":     "CurrentAccount",
             "value": {
                 "type":    "VAL_ZERO"
             },
             "accounts": []
         }
     ]
}
```

#### Full definition:

Response Field   | Type                                    | Description
-----------------|-----------------------------------------|-----------------
**name**         | String                                  | The name of this account
**value**        | [Value](#value-object)                  | The value calculated for this account
**accounts**     | Array of [accounts](#account-object)    | The list of accounts, which are children of this account, and are included in the report

## Amount Object

Represents an amount (e.g £2.50) of a single commodity.

### Example

```JSON
{
     "commodity_key": "£",
     "value_amount":  173.92
}
```

### Full Definition

Response Field   | Type      | Description
-----------------|-----------|----------
**commodity_key**| String    | The symbol associated with amount's commodity
**value_amount** | Number    | The numberical value associated with the amount


## Status Object

Every time the LedgerJRI daemon is asked to load, or reload, a journal - a new session id is created. The status
object reports the current state of that session.

### Example

```JSON
{
     "status":  {
         "state":  "BAD_JOURNAL",
         "reason": "No such file: '/home/lah/personal/budget/2020.jnl'"
     }
}
```

### Definition

Status Field   | Type       | Can Be Null?|   Valid Values     | Description
---------------|------------|-------------|--------------------|-------------
**state**      | String     | No          | READY, BAD_JOURNAL |
**reason**     | String     | Yes         |                    |Populated with a suitable error text, if the journal is not READY

## Value Object

### Example - single commodity amount

```JSON
{
     "type":    "VAL_AMOUNT",
     "amount": {
         "commodity_key": "£",
         "value_amount":  173.92
     }
}
```

### Example - Zero amount

```JSON
{
     "type":    "VAL_ZERO"
}
```

### Full definition:

Status Field   | Type                       | Can Be Null?|   Valid Values       | Description
---------------|----------------------------|-------------|----------------------|-------------
**type**       | String                     | No          | VAL_AMOUNT, VAL_ZERO | The type of value being returned
**amount**     | [Amount](#amount-object)   | Yes         |                      | If type is VAL_AMOUNT, contains the single commodity amount vaule
