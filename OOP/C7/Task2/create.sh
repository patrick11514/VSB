#!/bin/bash

H=$(cat <<EOF
#ifndef __$1_H_
#define __$1_H_ 1

class $1 
{
public:
    $1();
    ~$1();
};

#endif
EOF
)

echo "$H" > $1.h

C=$(cat <<EOF
#include "$1.h"

$1::$1()
{
}

$1::~$1()
{
}
EOF
)

echo "$C" > $1.cpp