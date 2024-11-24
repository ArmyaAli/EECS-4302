// ERROR MESSAGE CONSTANTS
const char* ERRORMSG_SCANNER_UNEXPECTED_CHAR = "scan error: Unexpected character '%s' at line %d\n";
const char* USAGEMSG_SCANNER = "Usage: %s -scan sourcefile.bminor\n";
const char* USAGEMSG_PARSER = "Usage: %s -parse sourcefile.bminor \n";
const char* USAGEMSG_NAMERESOLVER = "Usage: %s -resolve sourcefile.bminor\n";
const char* USAGEMSG_TYPECHECKER = "Usage: %s -typecheck sourcefile.bminor\n";
const char* ERRORMSG_PARAM_LIST_SIZE = "TYPE_ERROR: Function paramter list size not equal\n";

const char* ERRORMSG_TYPE_ASSIGNMENT_ERROR="<< TYPE ERROR >>: Can not assign type of (%s) %s to (%s) %s\n";
const char* ERRORMSG_DECL_ASSIGNMENT_ERROR="<< TYPE ERROR >>: decl_type does not equal value type. %s -> %s\n";

const char* ERRORMSG_TYPE_EXPR_ADD_ERROR = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s + (%s) %s] \n";
const char* ERRORMSG_TYPE_EXPR_SUB_ERROR = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s - (%s) %s] \n";
const char* ERRORMSG_TYPE_EXPR_MUL_ERROR = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s * (%s) %s] \n";
const char* ERRORMSG_TYPE_EXPR_DIV_ERROR = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s / (%s) %s] \n";

const char* ERRORMSG_TYPE_EXPR_LOGICAL_AND_ERROR = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s && (%s) %s] \n";
const char* ERRORMSG_TYPE_EXPR_LOGICAL_OR_ERROR  = "<< TYPE_ERROR >>: <%s> can not be performed on [(%s) %s || (%s) %s] \n";

const char* ERRORMSG_TYPE_EXPR_LOGICAL_NOT_ERROR = "<< TYPE ERROR >>: << TYPE_MISTMATCH >> can only be applied on a boolean operator. ((%s) %s)\n";
const char* ERRORMSG_TYPE_EXPR_LOGICAL_EXP_ERROR =" << TYPE_ERROR >>: `%s` can not be performed on [(%s) %s ^ (%s) %s] \n";



