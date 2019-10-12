#!/bin/bash

program=$1
shift
case $program in
"build")
    sphinx-build "$@"
    ;;
"quickstart")
    sphinx-quickstart "$@"
    ;;
"apidoc")
    sphinx-apidoc "$@"
    ;;
"autogen")
    sphinx-autogen "$@"
    ;;
*)
    echo $"Usage: $0 {build|quickstart|apidoc|autogen} args"
    exit 1
esac
