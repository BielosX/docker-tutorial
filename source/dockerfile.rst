**********
Dockerfile
**********

Przydatne obrazy dostępne na `DockerHub <https://hub.docker.com>`_:

busybox
`````````
    `BusyBox <https://busybox.net/about.html>`_ czyli zbiór typowych narzędzi UNIXowych
    takich jak ``cp``, ``ls`` czy ``ping`` zawartych w jednym programie wykonywalnym
    (każda komenda jest dowiązaniem symbolicznym do programu implementującego wszystkie funkcje).
    Dzięki takiemu rozwiązaniu rootfs oparty o busybox zajmuje bardzo mało miejsca.

alpine
````````
    Dystrybucja linuxa oparta o `BusyBox` która cechuje się małym rozmiarem.
    Udostępnia manager pakietów ``apk``. Lista dostępnych pakietów dostępna
    jest pod adresem `Packages <https://pkgs.alpinelinux.org/packages>`_.

scratch
`````````
    Pusty kontener. Przydatny gdy chcemy utworzyć naprawdę mały obraz zawierający jedynie
    plik wykonywalny i potrzebne zależności.


.. admonition:: Zadanie

    Stwórz Dockerfile który wygeneruje obraz z Apache Httpd serwującym plik html o następującej treści:

    .. code-block:: html
        :linenos:

        <!DOCTYPE html>
        <html>
            <head>
                <meta charset="utf-8">
                <title>docker-tutorial</title>
            </head>
            <body>
                <strong>docker-tutorial</strong>
            </body>
        </html>