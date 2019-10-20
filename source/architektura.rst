*********************
Docker - architektura
*********************

Podstawowe pojęcia
--------------------

obraz
```````

kontener
``````````

volume
`````````

klient-server
----------------

Głównym procesem odpowiedzialnym za działanie dockera jest ``dockerd``. Jest to daemon zarządzający
bazą danych obrazów, kontenerów, sieci, volumenów etc. Jest także rodzicem wszystkich procesów
działających w kontenerach. Wywołanie ``ps -aef --forest`` pokazuje relacje między procesami:

.. code-block:: console
    :linenos:

    root         454       1  0 paź19 ?       00:02:03 /usr/bin/dockerd -H fd:// -H tcp://0.0.0.0:2375
    root         500     454  0 paź19 ?       00:00:50  \_ containerd --config /var/run/docker/containerd/containerd.toml --log-level info
    root       36920     500  0 10:03 ?        00:00:00      \_ containerd-shim -namespace moby -workdir /var/lib/docker/containerd/daemon/io.containerd.runtime.v1.linux/moby/7b83eb2f97e1a1a4eed569bbc4097982e023b0954
    999        36939   36920  0 10:03 ?        00:00:03      |   \_ redis-server *:6379
    root       37359     500  0 10:04 ?        00:00:00      \_ containerd-shim -namespace moby -workdir /var/lib/docker/containerd/daemon/io.containerd.runtime.v1.linux/moby/8ca1ea2f2359f70c551d50bc4b887eae95f631829
    root       37376   37359  0 10:04 pts/0    00:00:00          \_ sh

W tym przypadku zostały wydane polecenia:

.. code-block:: console
    :linenos:

    docker run -it busybox sh
    docker run -d redis

Co widać na wyniku listowania procesów, zarówno ``redis-server`` jaki i ``sh`` są podprocesami ``dockerd``.
Widoczny w wyniku ``containerd`` jest odpowiedzialny za proces uruchomienia aplikacji w kontenerze.
Jego rolą jest uruchamianie aplikacji w odseparowanym środowisku tak, aby proces działających
w kontenerze nie był świadomy istnienia systemu hosta.
Można więc rozumieć odpowiedzialność ``dockerd`` jako tę związaną z bazą danych obrazów, kontenerów etc.
natomiast ``containerd`` jako warstwa uruchomieniowa kożystająca z funkcjonalności jądra ``Linux``
do zapewnienia separacji.

baza danych
`````````````

Wszystkie dane zarządzane przez ``dockerd`` w tym obrazy i kontenery znajdują się w ``/var/lib/docker``.
Systemy plików przechowywane są w katalogu o nazwie odpowiadającej użytemu systemowi plików,
domyślnie używany obecnie system plików to ``overlay2``. Foldery ``containers`` obraz
``image`` zawierają baze danych obrazów oraz kontenerów. Przechowywane tam inforamcje
to np. konfiguracja, pliki ``hostname``, ``resolv.conf`` oraz ``hosts``.

.. admonition:: Zadanie

    Uwtórz w systemie plików nowego kontenera plik tekstowy a nastepnie zlokalizuj
    go w ``/var/lib/docker`` (np. za pomocą narzędzi ``find`` lub ``grep``)

    Na potrzeby tego zadania uruchom interaktywny terminal poleceniem:
    ``docker run -it busybox sh``

komunikacja z daemonem
````````````````````````

Komunikacja między narzędziem konsolowym ``docker`` a ``dockerd`` odbywa się przez unix domain
socket ``/var/run/docker.sock``. ``Dockerd`` można więc uznać za aplikacje serwerową z publicznym API,
możliwa jest taka konfiguracja ``dockerd`` aby nasłuchiwał również pod konkretnym adresem IP i portem
co pozwala np. na konfigurację zdalną.

W systemach używających `Systemd <https://www.freedesktop.org/wiki/Software/systemd/>`_ opis serwisu docker
znajduje się w ``/etc/systemd/system/multi-user.target.wants/docker.service``, domyślnie proces ten uruchamiany jest
tylko z jednym parametrem, ``-H fd://`` który jest częścią sekwencji inicjalizacji oferowanej przez ``systemd``
(szczegóły dostępne `Tutaj <https://stackoverflow.com/a/43408869>`_). Możliwe jest wprowadzenie zmian w tym pliku,
edytowanie ``/etc/docker/daemon.json``, bądź użycie `Drop-in files <https://wiki.archlinux.org/index.php/Systemd#Drop-in_files>`_.

.. warning::
    W przypadku uruchamiania dockerd przez systemd
    Chcąc dodać do ``/etc/docker/daemon.json`` konfiguracje adresów na których ma nasłuchiwać docker
    konieczne jest wykonanie kroków opisanych w `Doc <https://docs.docker.com/config/daemon/#troubleshoot-conflicts-between-the-daemonjson-and-startup-scripts>`_


.. admonition:: Zadanie

    1. Skonfiguruj dockerd tak aby nasłuchiwał na localhost:2375 po protokole TCP

    Na potrzeby tego zadania ściągnij obraz ``hello-world`` poleceniem:
    ``docker pull hello-world``

    Po wprowadzeniu zmian w konfiguracji konieczny jest restart usługi:
    ``systemctl restart docker.service``


.. admonition:: Zadanie

    1. Wyświetl wszystkie procesy uruchomione w kontenerze
    2. Wyświetl wszystkie procesy uruchomione namaszynie hosta

    Na potrzeby tego zadania uruchom interaktywny terminal poleceniem:
    ``docker run -it busybox sh``