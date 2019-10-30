from alpine:3.10.2 as builder

run apk add --no-cache py3-setuptools make && \
    pip3 install --upgrade pip && \
    pip3 install -U sphinx==1.8.5 && \
    pip3 install -U sphinx_glpi_theme && \
    pip3 install -U sphinx-copybutton
copy . /sphinx-proj
workdir /sphinx-proj
run make html

from httpd:2.4-alpine

copy --from=builder /sphinx-proj/build/html /usr/local/apache2/htdocs/

