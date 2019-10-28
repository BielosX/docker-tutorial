*********************
Docker - architektura
*********************

Podstawowe pojęcia
--------------------

obraz
```````

kontener
``````````

Instancja procesu odseparowanego o systemu hosta wraz z przypisanym do niego systemem plików
``read-write``. Wszystkie warstwy składające się na obraz na którym opiera się kontener
są scalane i widoczne z poziomu kontenera. Działanie w systemie plików obrazu
opera się na metodzie **copy-on-write**, znaczy to, że dopóki kontener
używa plików obrazu w trybie tylko do odczytu to współdzieli ten plik
z innymi kontenerami opartymi na tym obrazie. W przypadku pierwszej modyfikacji
pliku udostępnianego przez obraz jest on kopiowany do systemu plików kontenera
i tam zapisywany jako zmodyfikowany. Od tego momentu kontener pracuje
na własnej kopii pliku.

volume
`````````

OverlayFS
---------

Jak sama nazwa wskazuje `OverlayFS` (overlay: eng. nakładać) jest systemem plików umożliwającym
składanie wielu systemów plików tak aby były widoczne jako jeden. `OverlayFS` definiuje pojęcia takie jak
`UpperDir`, `LowerDir` oraz `Merged`. `LowerDir` jest systemem plików, lub systemami plików
które łączone są w jeden system **tylko do odczytu**, jeśli jakiś plik istnieje na wielu zdefiniowanych
warstwach to kolejność ich definicji mówi, z której warstwy zostanie pobrany dany plik.
`UpperDir` reprezentuje system plików **read write**, każda zmiana plików udostępnianych przez dolne warstwy
będzie kopiowana do tej warstwy. Równierz tworzenie nowych plików będzie miało miejsce na tej
warstwie i nie spowoduje zmian w warstwach zdefiniowanych jako `LowerDir`.
`Merged` jest wynikiem połączenia `LowerDir` i `UpperDir` i przedstawia spójny system plików.

Doker wykorzystuje ``OverlayFS`` do tworzenia kontenerów. Przy uruchamianiu aplikacji w kontenerze
warstwy składające się na obraz bazowy montowane są jako `LowerDir` natomiast przestrzeń
**read write** zarezerwowana dla kontenera montowana jest jako `UpperDir`, następnie
kontener uruchamiany jest z zamontowanym katalogiem ``/`` wskazującym na katalog ``Merged``.

Aby zamontować system plików ``OverlayFS`` należy wydać polecenie:

.. code-block:: console
    :linenos:

    mount -t overlay overlay -o lowerdir=/lower1:/lower2:/lower3,upperdir=/upper,workdir=/work /merged

Efektem tego będzie system plików składający się z następujących warstw:

.. code-block:: console
    :linenos:

    /upper
    /lower1
    /lower2
    /lower3

Warstwy wyspecyfikowane w parametrze ``lowerdir`` będą występowały w kolejności od prawej do lewej.
Oznacza to, że jeśli na warstwie ``/lower3`` znajduje się plik ``test.txt`` który występuje również
na warstwie ``/lower2`` to wersja tego pliku widoczna w katalogu ``/merged`` będzie pochodziła z warstwy ``/lower2``.
Katalog zdefiniowany opcją ``workdir`` jest niezbędny i dokumentacja opisuje go następująco:

.. code-block:: console
    :linenos:

    The workdir option is required, and used to prepare files before they areswitched
    to the overlay destination in an atomic action (the workdir needs to be on the same filesystem as the upperdir).


Jak widać warstwy oraz obrazy oferowane przez docker mogą być w pełni realizowane przez sterownik OverlayFS.
Docker przy uruchamianiu kontenera wyszukuje w swojej bazie danych informacji o obrazie na bazie którego
ma powstać kontener, następnie wszystkie warstwy obrazu montowane są jako `LowerDir` a specjalnie
utworzony na potrzeby kontenera katalog jako `UpperDir`, zapewnia to separacje kontenera od innych
bazujących na tym samym obrazie, gdyż z definicji `LowerDir` jest *read-only*, nie ma więc możliwości, że
dwa kontenery bazujące na tym samym obrazie nadpiszą sobie jakiś plik. Zmiany w ich plikach będą widoczne
jedynie w ich `UpperDir` do których inne kontenery nie będą miały dostępu.

Separacja kontenerów
----------------------

Docker jest ściśle powiązany z systemem Linux i używa dostarczanych przez niego narzędzi takich jako
`namespaces` oraz `cgrups` do izolowania procesu działającego w kontenerze od innych procesów działających w systemie hosta.


namespaces
````````````

Narzędzie pozwalające na tworzenie wyizolowanych zasobów dla procesu. Przy tworzeniu nowego procesu za pomocą
wywołania systemowego ``clone`` (``man 2 clone``) istnieje możliwość określenia konkretnych przestrzeni nazw w których
będzie działał proces. Istnieje również możliwość włączenia procesu do istniejącej przestrzeni nazw za pomocą
funkcje ``setns`` (``man 2 setns``) lub przeniesienie działającego procesu do nowej przestrzeni nazw za pomocą
funkcji systemowej ``unshare`` (``man 2 unshare``). 

Linux udostępnia następujące przestrzenie nazw:

pid_namespaces
    Izolacja identyfikatorów procesów. Oznacza to, że procesy znajdujące się w różnych
    *pid_namespaces* mogą posiadać ten sam identyfikator procesu. Pierwszy proces utworzony
    w nowej przestrzeni nazw otrzymuje PID 1 i staje się rodzicem wszystkich osieroconych
    procesów działających w tej przestrzeni nazw. **Jeśli proces o PID 1 w namespace
    zostanie zatrzymany kernel zatrzymuje wszystkie procesy w tym namespace**.

mount_namespace
    Izoluje liste zamontowanych systemów plików widocznych przez procesy znajdujące się w tej przestrzeni nazw.
    Modyfikacje zamontowanych systemów plików za pomocą wywołań systemowych ``mount`` i ``umount``
    nie zmodyfikują zamontowanych systemów plików w innych przestrzeniach nazw.

network_namespace
    Izoluje zasoby systemowe związane z sieciami: urządzenia, tablice routingu, regóły firewalla, stos IPv4 i IPv6.

user_namespaces
    Izoluje identyfatory użytowników i grup, katalog ``root``.

Pełna lista dostępna za pomocą polecenia ``man 7 namespaces``.

cgroups
`````````

Control groups, pozwala zorganizować procesy w hierarchię umożliwiającą ograniczanie i monitorowanie
zasobów takich jak pamięć czy CPU.


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

Ukazane tutaj procesy, czyli ``redis-server`` oraz ``sh`` posiadają unikatowy PID i mogą zostać
zatrzymane z poziomu hosta za pomocą polecenia ``kill``.

baza danych
`````````````

.. hint::
    Wszelkie wpisy formatu ``JSON`` znajdujące się w bazie danych docker są przechowywane w wersji pozbawionej
    znaków odstępu. Aby wyświetlać je w czytelny sposób używaj polecenia ``python -m json.tool`` np.
    ``cat repositories.json | python -m json.tool``.

Jak opisano wocześniej Docker wykorzystuje system plików ``OverlayFS`` do współdzielenia obrazów
oraz zapewnienia separacji między systemami plików poszczególnych kontenerów. Stosuje on również
dostępne w systemie Linux narzędzia takie jak `namespaces` i `cgroups` do zapewnienia
separacji w trakcie wykonania procesu. Głównym zadaniem ``dockerd`` oraz pomocniczych usług jest
więc zarządzanie bazą danych obrazów i kontenerów. Baza danych programu docker znajduje się w katalogu
``/var/lib/docker``. Uruchomienie nowego kontenera przebiega następująco:

1. Znalezienie wpisu dotyczącego obrazu bazowego w pliku ``image/overlay2/repositories.json``.
2. Jeśli wybrany obraz znajduje się w tym pliku pobierana jest wartość skrótu ``sha256``, jeśli nie ma takiego obrazu następuje próba pobrania go.
3. Plik zawierający liste warstw składających się na obraz znajduje się w ``image/overlay2/imagedb/content/sha256``, jego nazwa odpowiada pobranemu wcześniej skrótowi, plik ten jest formatu ``JSON``.
4. Warstwy składające się na obraz znajdują się na liście ``rootfs.diff_ids``, należy pobrać całą liste.
5. Opisu warstw należy szukać w ``image/overlay2/layerdb/sha256``, zawarte tam katalogi zawierają plik ``diff`` którego wartość jest taka sama jak ``diff_ids`` pobrane z opisu obrazu.
6. Ustalenie relacji między warstwami jest możliwe dzięki plikowi ``parent`` znajdującym się w tym samym katalogu co plik ``diff``.
7. W każdym katalogu zawierającym pliki ``diff`` znajduje się plik ``cache-id``, zawartość tego pliku zawiera identyfikator systemu plików danej warstwy. Pliki składające się na daną warstwe przechowywane są w folderze ``/var/lib/docker/overlay2`` w katalogu którego nazwa odpowiada uzyskanemu wcześniej ``cache-id``

.. admonition:: Zadanie

    Zamontuj system plików ``OverlayFS`` składający się ze wszystkich warstw wybranego obrazu oraz nowej warstwy `UpperDir`,
    utwórz w nim nowy plik i zweryfikuj, czy zmiany te widoczne są w dolnych warstwach.

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