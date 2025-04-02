grammar Grammar;

/** The start rule; begin parsing here. */
prog: stat+;

stat: (expr ';')+ EOF
    ;

expr: expr op=('*'|'/') expr # Mul 
    | expr op=('+'|'-') expr # Add 
    | INT                    # Int
    | HEX                    # Hex
    | OCT                    # Oct
    | '(' expr ')'           # Par
    ;

number: INT
      | HEX
      | OCT
      ;

INT : [1-9][0-9]* ;            // match integers
HEX : '0x' [0-9a-fA-F]* ;      // hexa numbers
OCT : '0' [0-7]* ;             // octal numbers
WS : [ \t\n\r]+ -> skip ;      // toss out whitespace
