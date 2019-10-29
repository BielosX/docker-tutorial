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


Uruchamianie polecenia
----------------------

.. warning::
    Niektóre managery pakietów (np. ``apt-get``) domyślnie wymagają potwierdzenia chęci zainstalowania pakietu
    oczekując na naciśnięcie odpowiedniego klawisza. W przypadku uruchamiania takiego managera pakietów
    za pomocą polecenia ``RUN`` należy się upewnić, że został on uruchomiony z odpowiednim przełącznikiem
    informującym manager, że jesteśmy pewni swojej decyzji. Wyświetlenie pytania może przerwać proces budowania.
    W przypadku ``apt-get`` są to przełączniki ``-y``, ``--yes`` oraz ``--assume-yes``.


Multistage build
----------------

Każda warstwa utworzona za pomocą komend w Dockerfile może znacząco zwiększyć rozmiar obrazu wynikowego.
Często jest tak, że narzędzia potrzebne do zbudowania aplikacji nie są potrzebne do jej uruchomienia,
pozostawienie niepotrzebnych aplikacji niezbędnych przy procesie budowania w obrazie wynikowym
spowodowałoby znaczny wzrost jego rozmiaru. Rozwiązań tego problemu jest kilka:

1. Stworzenie skryptu np. w ``bash`` uruchamiającego kontener z kompilatorem oraz zamontowanym folderem zawierającym źródła oraz folderem wynikowym. Następnie skrypt ten uruchamiałby budowanie Dockerfile kopiującego program wykonywalny z folderu wynikowego do nowego obrazu.
2. Stowrzenie skryptu tworzącego kontener i kopiującego za pomocą ``docker container cp`` źródła, następnie uruchamiającego kompilator i po etapie kompilacji kopiującego z kontenera program wynikowy który następnie zostanie użyty przy budowaniu obrazu za pomocą Dockerfile.
3. Zastosowanie ``Multistage build`` i etapów pośrednich których wygenerowane warstwy nie będą składowymi ostatecznego obrazu.

Multistage build pozwala na definiowanie wielu poleceń ``FROM`` a następnie kopiowanie wyników działania komend
do kolejnego etapu budowania, wygląda to następująco:


.. code-block:: dockerfile
    :linenos:

    FROM alpine:3.10.3
    RUN echo "test" > test.txt

    FROM busybox
    COPY --from=0 test.txt /tmp/test.txt

Istnieje możliwość nadawania nazw kolejnym etapom budowania i odwoływania się do nich za pomocą flagi ``--from``:

.. code-block:: dockerfile
    :linenos:

    FROM alpine:3.10.3 AS awesome
    RUN echo "test" > test.txt

    FROM busybox
    COPY --from=awesome test.txt /tmp/test.txt

.. admonition:: Zadanie

    Utwórz plik Dockerfile budujący następujący kod java za pomocą JDK:

    .. code-block:: java
        :linenos:

        public class Hello {
            public static void main(String[] args) {
                System.out.println("Hello World");
            }
        }

    Niech obraz wynikowy uruchamiający aplikacje nie zawiera narzędzi do budowania.
    (Dodatkowe punkty za Gradle z application plugin, ``gradle init --type java-application``).


CMD i ENTRYPOINT
----------------

Polecenie ``CMD`` pozwala określić **domyślne** polecenie wykonywane w kontenerze.
Jego działanie można zaobserwować na przykładzie obrazu ``redis``:

.. code-block:: console
    :linenos:

    docker run redis

Wywołanie tego polecenie spowoduje uruchomienie kontenera z działającym serwerem `redis`. W jego Dockerfile
program serwera został zdefiniowany w poleceniu ``CMD``, nic nie stoi jednak na przeszkodzie uruchomić obraz
``redis`` w następujący sposób:

.. code-block:: console
    :linenos:

    docker run -it redis /bin/bash

Spowoduje to uruchomienie aplikacji ``/bin/bash`` w kontenerze, lecz serwer ``redis`` **nie zostanie uruchomiony**.
``CMD`` wskazuje tu na domyślną aplikacje, lecz nie zabrania uruchomienia innej, dostępnej w obrazie.


Polecenie ``ENTRYPOINT`` mimo, że również pozwala określić proces, który zostanie uruchomiony w kontenerze
nie daje możliwości uruchomienia innego programu. Zamiast tego wszystkie parametry następującej
po nazwie obrazu przy uruchamianiu zostają przekazane do tej aplikacji.

Przykładowo mając plik Dockerfile na podstawie którego powstał obraz ``my-java``:

.. code-block:: dockerfile
    :linenos:

    FROM openjdk:8-jre-alpine
    ENTRYPOIN ["java"]

Uruchomienie aplikacji za pomocą:

.. code-block:: console
    :linenos:

    docker run my-java --help

Spowoduje przekazanie parametru ``--help`` do programu ``java``.
``ENTRYPOINT`` jest więc przydatny do budowania obrazów z programami narzędziowymi takimi jak kompilatory,
gdyż pozwala to na zdefiniowanie niezmiennej aplikacji która zostanie uruchomiona w kontenerze
wraz z niezbędnymi flagami która następnie zostanie uruchomiona z odpowiednimi parametrami.

Przykładowo można zdefiniować Dockerfile dla kompilatora GCC generujący obraz ``my-gcc`` w następujący sposób:

.. code-block:: dockerfile
    :linenos:

    FROM gcc:9
    ENTRYPOINT ["gcc", "-g", "-O3"]

Każde uruchomienie kontenera z tym kompilatorem wywoła go z opcjami ``-g`` (symbole debugowania) oraz ``-O3`` (optymalizacja).
Wywołanie:

.. code-block:: console
    :linenos:

    docker run my-gcc -o /out/main main.c

Co spowoduje w rzeczywistości wywołanie polecenia:

.. code-block:: console
    :linenos:

    gcc -g -O3 -o /out/main main.c


Argumenty budowania
-------------------

Istnieje możliwość zdefiniowania stałych możliwych do podania przy wywołaniu ``docker build`` jak również nadanie im domyślych wartości,
słóży do tego polecenie ``ARG``.

``ARG`` może zostać następnie użyte za pomocą odwołania z symbolem dolara.