**************
Mount i Volume
**************

Docker pozwala na montowanie systemów plików widocznych tak aby były widoczne w kontenerze.
Dostępne są trzy typy montowania:

* bind
* volume
* tmpfs

bind
``````

    tryb montowania `bind` pozwala na zamontowanie wskazanego katalogu znajdującego się w systemie
    hosta tak aby było on widoczny w kontenerze pod wskazaną ścieżką.

    .. code-block:: console
        :linenos:

        docker run -d --mount type=bind,src=/tmp/dir,dst=/data busybox

    wywołanie to spowoduje, że zawartość katalogu ``/tmp/dir`` znajdującego się w systemie hosta
    będzie widoczna w kontenerze pod ścieżką ``/data``.

volume
````````

    pozwala zdefiniować wolumen zarządzany przez docker. Urzytkownik wolumenu nie zna jego miejsca składowania
    (chyba, że wywoła polecenie ``docker inspect``). Wolumen tworzony jest poprzez wywołanie:

    .. code-block:: console
        :linenos:

        docker volume create volume-name

    Wywołanie to utworzy wolumen o nazwie ``volume-name``. Aby podejrzeć własności stworzonego wolumenu
    należy wywołać polecenie:

    .. code-block:: console
        :linenos:

        docker volume inspect volume-name

    Informacja o miejscu składowania danych znajduje się w polu ``Mountpoint``.
    Możliwe jest listowanie wszystkich stworzonych wolumenów za pomocą polecenia:

    .. code-block:: console
        :linenos:

        docker volume ls

    Aby usunąć istniejący wolumen należy wywołać polecenie:

    .. code-block:: console
        :linenos:

        docker volume rm volume-name

    Możliwe jest usunięcie wszystkich nieużywanych wolumenów za pomocą polecenia:

    .. code-block:: console
        :linenos:

        docker volume prune

    Aby uruchomić kontener wykorzystując utworzony wcześniej wolumen należy użyć polecenia:

    .. code-block:: console
        :linenos:

        docker run -d --mount type=volume,source=psgres,target=/var/lib/postgresql/data postgres

    Wywołanie tego polecenia spowoduje, że utworzony wcześniej wolumen o nazwie ``psgres`` zostanie
    zamontowany pod ścieżką ``/var/lib/postgresql/data``. Efektem tego będzie zachowanie
    danych składowanych w bazie nawet po zatrzymaniu kontenera.

tmpfs
```````

    System ``tmpfs`` w odróżnieniu od pozostałych trybów montowania nie pozwala na współdzielenie
    danych między kontenerami oraz przechowywanie ich po zatrzymaniu kontenera.
    W przypadku ``tmpfs`` system plików jest tymczasowy a miejscem jego przechowywania
    jest pamięć RAM. W przypadku tego systemu wymagany jest jedynie parametr ``destination``
    wskazujący miejsce montowania w systemie plików kontenera.
    Aby uruchomić kontener wraz z zamontowanych ``tmpfs`` należy użyć polecenia:

    .. code-block:: console
        :linenos:

        docker run -d --mount type=tmpfs,target=/var/lib/postgresql/data postgres

.. caution::

    flaga ``--mount`` definiuje parametry o różnych nazwach lecz tym samym znaczeniu. I tak
    ``src`` może przyjść forme ``source`` natomiast ``dst`` może zostać zamieniony na
    ``target`` lub ``destintation``.


.. caution::

    Obecnie nie jest możliwe IPC za pomocą Unix Domain Sockets, PIPE etc które zostały utworzeone
    na dockerowym wolumenie.

.. admonition:: Zadanie

    Sprawdź różnice między systemem plików kontenera a montowaniem ``tmpfs``,
    użyj do tego obrazu ``postgres`` oraz benchmarka ``pgbench``.
    Czy widoczna jest różnica w liczbie transakcji oraz w opóźnieniu?
