grammar Project;
start  : statement+ EOF ;  

statement : command? ';'                                      # CMD
          | '{' statement+ '}'                                # BLOCK
          | 'if' '(' condition ')' statement                   # IF
          | 'if' '(' condition ')' statement 'else' statement # IFFELSE
          | 'while' '(' condition ')' statement                # WHILE
          ;
 

command : vartype VARID (',' VARID)*                     # CMDVAR
        | expr                                           # CMDEXPR
        | 'read' VARID (',' VARID)*                      # CMDREAD
        | 'write' expr (',' expr)*                       # CMDWRITE
        ; 

condition : expr ;

expr : '-' (INT | FLOAT)           # MINUS
     | '!' expr                    # NOT
     | expr ('*' | '/' | '%') expr # MUL
     | expr ('+' | '-' | '.') expr # ADD
     | expr ('<' |'>') expr        # REL
     | expr ('==' | '!=') expr     # EQUAL
     | expr '&&' expr              # AND
     | expr '||' expr              # OR
     | expr '=' expr               # ASSIGN
     | INT                         # INT
     | FLOAT                       # FLOAT
     | BOOL                        # BOOL   
     | STR                         # STR
     | VARID                       # VARID
     | '(' expr ')'                # PAREN
     ;

//LITERALS
INT : [0-9]+ ;
FLOAT : [1-9][0-9]* '.' [0-9]+ ;
BOOL : 'true' | 'false' ;
STR : '"' .*? '"' ;

//VARABLES
VARID : [a-zA-Z][0-9a-zA-Z]* ;
vartype : 'int'
        | 'float'
        | 'bool'
        | 'string'
        ;

WS  :   [ \t\r\n]+ -> skip ;
COMMENT : '//' ~[\r\n]* -> channel(HIDDEN) ;
