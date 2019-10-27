**********
Dockerfile
**********

`Dockerfile <https://docs.docker.com/engine/reference/builder/>`_ jest opisem kroków koniecznych do przygotowania obrazu dockerowego.
Możliwe jest kopiowanie plików znajdujących się w systemie hosta do obrazu,
uruchamianie programów udostępnianych przez poprzednie warstwy oraz
zarządzania parametrami obrazu.

Obraz bazowy
------------

Każdy Dockerfile musi zawierać odniesienie do obrazu bazowego na podstawie którego nowy obraz
będzie budowany, kolejne polecenia wykonają się tylko wtedy, gdy określony jest obraz bazowy.
Nawet jeśli nowy obraz ma być budowany od zera konieczne jest użycie obrazu bazowego ``scratch``
oznaczającego pusty obraz.

Przykładowy Dockerfile definiuje, że jako obraz bazowy ma zostać użyty ``alpine`` w wersji 3.10.3:

.. code-block:: dockerfile
    :linenos:

    FROM alpine:3.10.3

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
    Pusty obraz. Przydatny gdy chcemy utworzyć naprawdę mały obraz zawierający jedynie
    plik wykonywalny i potrzebne zależności.


Budowania obrazu
----------------

Polecenie ``docker build`` pozwala na stworzenie obrazu dockerowego za pomocą Dockerfile.
Składnia tego polecenia wygląda następująco:


**docker build [OPTIONS] PATH | URL | -**

Parametry ``PATH`` oraz ``URL`` określają tak zwany ``build context``. Jest to zbiór plików znajdujących
się pod wskazaną ścieżką. Proces budowania opisany w Dockerfile może odnosić się do każdego
pliku znajdującego się w build context. **Nie jest to ścierzka do Dockerfile a wyszukiwanie
plików w procesie budowania nie odbywa się względem połorzenia Dockerfile**, Dockerfile
może znajdować się w innym katalogu (ale będącym w hierarchii której początkiem jest ``PATH``).
Przykładowo możliwe jest wskazanie ``build context`` w ``/tmp/new-image`` a Dockerfile
może znajdować się w ``/tmp/new-image/dockerfile``:

.. code-block:: console
    :linenos:

    docker build /tmp/new-image -f /tmp/new-image/Dockerfile

Możliwe jest nadawanie nazw oraz opcjonalnych tagów tworzonym obrazom:

.. code-block:: console
    :linenos:

    docker build . --tag my-image:0.1

W powyrzszym przypadku polecenie ``build`` zostało wywołane w katalogu, w którym znajduje się Dockerfile
i który jednocześnie ma być ``build context``, stąd zamiast ścierzki podana jest kropka wskazująca
na aktualny katalog.

Kopiowanie plików do obrazu
---------------------------

Polecenie ``COPY`` pozwala na kopiowanie plików i folderów znajdujących się w systemie hosta do obrazu.
Wskazane pliki źródłowe są wyszukiwane względem `PATH` podanego jako argument polecenia ``docker build``.
Dockerfile kopiujący plik `test.txt` do nowego obrazu opartego na ``alpine`` wygląda następująco:

.. code-block:: dockerfile
    :linenos:

    FROM alpine:3.10.3
    COPY test.txt /test.txt


.. admonition:: Zadanie
    Zbuduj nowy obraz za pomocą powyrzszego Dockerfile a następnie zweryfikuj, czy plik tekstowy znalazł się w oczekiwanym miejscu.

Możliwe jest kopiowanie całych hierarchii plików:

.. code-block:: dockerfile
    :linenos:

    FROM alpine:3.10.3
    COPY my-dir /some-dir

W powyrzszym przykładzie jeśli hierarchia katalowów wygląda następująco:

.. code-block:: console
    :linenos:

    my-dir
    └── test
        └── test.txt

W nowo utworzonym obrazie będzie to widoczne jako:

.. code-block:: console
    :linenos:

    some-dir
    └── test
        └── test.txt


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

Wydanie polecenia:

.. code-block:: console
    :linenos:

    docker image history IMAGE_ID:IMAGE_TAG

pozwala na wyświetlenie warstw składających się na obraz. W polu *CREATED_BY* wyświetlone
jest polecenie które wygenerowało konkretną warstwe. Dodanie flagi ``--no-trunc`` spowoduje,
że polecenia znajdujące się w polu *CREATED_BY* nie będą skracane.