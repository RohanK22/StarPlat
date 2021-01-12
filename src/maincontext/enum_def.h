/*enum for various graph characteristics*/

enum TYPE
{
 TYPE_LONG,
 TYPE_INT,
 TYPE_BOOL,
 TYPE_FLOAT,
 TYPE_DOUBLE,
 TYPE_GRAPH,
 TYPE_DIRGRAPH,
 TYPE_LIST,
 TYPE_SETN,
 TYPE_SETE

};

enum PROP
{
 PROP_NODE,
 PROP_EDGE

};

enum REDUCE
{
    REDUCE_SUM,
    REDUCE_COUNT,
    REDUCE_PRODUCT,
    REDUCE_MAX,
    REDUCE_MIN
};

enum OPERATOR
{
 OPERATOR_ADD,
 OPERATOR_SUB,
 OPERATOR_MUL,
 OPERATOR_DIV,
 OPERATOR_MOD,
 OPERATOR_OR,
 OPERATOR_AND,
 OPERATOR_LT,
 OPERATOR_GT,
 OPERATOR_LE,
 OPERATOR_GE,
 OPERATOR_EQ,
 OPERATOR_NE


};