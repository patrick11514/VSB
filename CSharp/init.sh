#!/bin/bash
if [ -z "$1" ] ; then
    echo "Usage: $0 <project name>"
    exit 1
fi

dotnet new console --name=$1
cp Program.cs $1/
