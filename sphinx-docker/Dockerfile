from alpine:3.10.2

run apk add --no-cache py3-setuptools make bash && \
    pip3 install --upgrade pip && \
    pip3 install -U sphinx==1.8.5 && \
    pip3 install -U sphinx_glpi_theme && \
    pip3 install -U sphinx-copybutton


run echo $'#!/bin/bash \n\
echo "SPHINX inside Docker!" \n\
program=$1 \n\
shift \n\
case $program in \n\
"build") \n\
    sphinx-build "$@" \n\
    ;; \n\
"quickstart") \n\
    sphinx-quickstart "$@" \n\
    ;; \n\
"apidoc") \n\
    sphinx-apidoc "$@" \n\
    ;; \n\
"autogen") \n\
    sphinx-autogen "$@" \n\
    ;; \n\
*) \n\
    echo $"Usage: $0 {build|quickstart|apidoc|autogen} args" \n\
    exit 1 \n\
esac' >> run_sphinx.sh && chmod +x run_sphinx.sh

workdir /sphinx-workdir
entrypoint ["/run_sphinx.sh"]
