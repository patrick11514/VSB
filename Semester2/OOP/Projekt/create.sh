#!/bin/bash

H=$(cat <<EOF
#pragma once

class $1 
{
public:
    $1();
    ~$1();
};


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