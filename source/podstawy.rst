********
Podstawy
********

Uruchomienie polecenia w kontenerze
`````````````````````````````````````

.. code-block:: console
    :linenos:

    docker run busybox echo "Hello World!"

Wywołanie to może skutkować pojawieniem się następującej informacji:

.. code-block:: text
    :linenos:

    Unable to find image 'busybox:latest' locally
    latest: Pulling from library/busybox
    7c9d20b9b6cd: Pull complete 
    Digest: sha256:fe301db49df08c384001ed752dff6d52b4305a73a7f608f21528048e8a08b51e
    Status: Downloaded newer image for busybox:latest

Oznacza to, że obraz ``busybox`` który znajduje się w zdalnym repozytorium nie został jeszcze
nigdy ściągnięty. Obraz zostanie pobrany i będzie przechowywany na dysku na potrzeby następnych uruchomień.

Składnia tego polecenia jest następująca: wywołujemy docker cli z poleceniem ``run``,
jako obraz wybrany został tu ``busybox`` z powodu małych rozmiarów,
po nazwie obrazu znajduje się nazwa polecenia, które ma zostać wykonane w kontenerze oraz argumenty tego polecenia.
W tym przypadku w kontenerze zostanie wykonany program ``echo`` a jego argumentem jest napis "Hello World!".

Można więc zapisać składne jako:

**docker run IMAGE:[TAG] [COMMAND] [ARG...]**

Zastosowana tutaj notacja oznacza, że podanie nazwy obrazu jest obowiązkowe natomiast zarówno komenda jak i jej argumenty są opcjonalne.
Uruchomienie polecenia

.. code-block:: console
    :linenos:

    docker run busybox

bez podania programu do wykonania jako argumentu zakończy natychmiast działanie kontenera. W kolejnych przykładach takie wywołanie
będzie jednak bardziej przydatne.

Opcjonalny parametr ``tag`` pozwala na wersjonowanie obrazu za pomocą dodatkowej informacji nazywanej tagiem.
Tag nie jest w żaden sposób narzucony więc różne obrazy posiadają różną konwencję,
Na przykład obraz Apache Httpd zawiera tagi:

* 2.4.41
* 2.4
* 2
* 2.4.41-alpine
* 2.4-alpine
* 2-alpine
* alpine
* latest

Jak widać przyjęta w tym przypadku konwencja pozwala wybrać dokładną wersję httpd bądź zdecydować jedynie o major wersji.
Tagi bez `alpine` oznaczają obrazy oparte na systemie Debian. Te zawierające `alpine` oparte sią na systemie
Alpine Linux i cechują się małym rozmiarem.

Jeśli nie zostanie wybrany żaden tag docker domyślnie uruchomi obraz z tagiem ``latest``.

Uruchomienie kontenera w trybie interaktywnym
```````````````````````````````````````````````

Domyślne zachowanie jest takie, że ``STDOUT`` wykonywanego programu jest dostępny i można go
przekierować do konejnego programu za pomocą operatora ``|``.

Następujące polecenie:

.. code-block:: console
    :linenos:

    docker run busybox echo "Hello World" | cat

Spowoduje wypisanie na terminalu napisu ``Hello World``, natomiast próba wywołania następującego polecenia:

.. code-block:: console
    :linenos:

    echo "Hello World" | docker run busybox cat

Nie wyświetli na terminalu żadnego wyniku. Dzieje się tak, gdyż domyślnie ``STDIN`` kontenera nie jest otwarte.
Aby powyższe polecenie działało poprawnie konieczne jest dodanie flagi ``-i`` lub jej pełnego odpowiednika czyli ``--interactive``

Poniższe wywołania są równoważne:

.. code-block:: console
    :linenos:

    echo "Hello World" | docker run --interactive busybox cat
    echo "Hello World" | docker run -i busybox cat

Wynikiem ich działania jest wypisanie na terminalu ``Hello World``

Uruchomienie kontenera w trybie terminala
```````````````````````````````````````````
Do tego przykładu należy wpierw uruchomić serwer `redis`:

.. code-block:: console
    :linenos:

    docker run redis

Możliwe jest również uruchomienie z parametrem ``-p`` aby był on dostępny jako ``localhost``,
jeśli parametr ten nie został użyty należy pobrać adres IP kontenera poleceniem ``docker inspect``,
w przedstawionym przykładzie adres to ``172.17.0.3`` ale na innych komputerach może się on różnić.
Po uruchomieniu serwera można już uruchomić narzędzie ``redis-cli`` będące częścią pakietu ``redis``
i pozwalające na wysyłanie poleceń do serwera:

.. code-block:: console
    :linenos:

    docker run -it redis redis-cli -h 172.17.0.3

Parametr ``-it`` jest tak naprawdę kombinacja parametrów ``-i`` oraz ``-t``, lub ich słownych wersji czyli
``--interactive`` ``--tty``. Powyższe wywołanie jest równoważne następującemu poleceniu:

.. code-block:: console
    :linenos:

    docker run --interactive --tty redis redis-cli -h 172.17.0.3

Po uruchomieniu tego polecenia powinnien pojawić się znak zachęty narzędzia ``redis-cli``:

.. code-block:: console
    :linenos:

    172.17.0.3:6379>

Można zweryfikować, że wykonanie komendy ``redis-cli`` w kontenerze bazującym na obrazie ``redis``
nie uruchomiło aplikacji serwerowej, a jedynie interaktywną konsolę. Zachowanie to zostanie
szerzej omówione w rozdziale poświęconym plikom ``Dockerfile``.


Aktywne kontenery
```````````````````

Aby wyświetlić aktualnie działające kontenery należy użyć polecenia:

.. code-block:: console
    :linenos:

    docker ps

Początkowo lista ta będzie pusta, próba uruchomienia kontenera poleceniem:

.. code-block:: console
    :linenos:

    docker run busybox echo "Hello World"

Nie spowoduje, że nowy kontener pojawi się na liście. Dzieje się tak dlatego, że kontener
niezwłocznie kończy swoje działanie gdy działający w nim proces zakończy się. Jako, że proces ``echo`` kończy się
od razu po wypisaniu napisu na ekranie kontener ten nie jest widoczny na liście aktywnych konteneryów.
Aplikacje serwerowe takie jak ``redis`` widoczne są na liście gdyż po ich uruchomieniu działają nieustannie
w trybie `foreground`. Uruchomienie kontenera ``redis`` oraz wylistowanie aktywnych kontenerów da rezultat:

.. code-block:: console
    :linenos:

    CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS               NAMES
    ea29ff3a20f7        redis               "docker-entrypoint.s…"   12 seconds ago      Up 11 seconds       6379/tcp            sleepy_kapitsa


Kontener w trybie detached
````````````````````````````

Jak można było zauważyć uruchomienie kontenera z serwerem ``redis`` spowodowało pojawienie się logów tej aplikacji
na standardowym wyjściu. Aby zlecić uruchomienie kontenera w tle, tak aby nie przejmował aktywnego terminala
należy użyć opcji ``-d`` lub ``--detach``.

.. code-block:: console
    :linenos:

    docker run -d redis

Wynikiem tego wywołania będzie wyświetlenie na terminalu identyfikatora uruchomionego kontenera. Jest to unikatowy identyfikator
za pomocą którego można zarządzać kontenerem.

Uruchamianie polecenia w działającym kontenerze
`````````````````````````````````````````````````

Identyfikacja kontenera
`````````````````````````

Każdy kontener posiada unikatowy identyfikator pozwalający na zarządzanie nim. Jest on widoczny w pierwszej kolumnie
po wpisaniu polecenia listowania kontenerów. Możliwe jest również identyfikowanie kontenera za pomocą nazwy.
Aby nadać kontenerowi nazwę należy użyć parametru ``--name`` przy uruchamianiu kontenera. Jeśli parametr
ten nie zostanie podany Docker wygeneruje losową nazwę.

Nazwa kontenera nie słóży jedynie do zarządzania nim. Jest ona używana również przez usługę DNS udostępnianą przez Docker.

.. admonition:: Zadanie

    Sprawdź, czy po nadaniu nazwy kontenerowi możliwe jest komunikowanie się między kontenerami za pomocą tej nazwy.
    Sprawdź, czy z poziomu hosta możliwy jest dostęp do kontenera za pomocą nazwy.


Cykl życia kontenera
``````````````````````

Kontener po uruchomieniu za pomocą polecenia ``docker run`` ma status ``running`` i jest widoczny na liście
aktywnych kontenerów po wydaniu polecenia ``docker ps``.
Wydanie polecenia:

.. code-block:: console
    :linenos:

    docker kill nazwa-lub-id

Powoduje wysłanie sygnału `kill` do procesu działającego w kontenerze co skutkuje zamknięciem kontenera.
Zmienia on wtedy status na ``exited``. Aby wyświetlić wszystkie kontenery, nawet te zatrzymane
należy wywłać polecenie:

.. code-block:: console
    :linenos:

    docker ps --all

lub

.. code-block:: console
    :linenos:

    docker container ls --all

Kontener który ma status ``exited`` może zostać ponownie uruchomiony za pomocą polecenia:

.. code-block:: console
    :linenos:

    docker container start nazwa-lub-id

Jeśli aplikacja działająca w zatrzymanym kontenerze zapisywała dane na dysku to
**powinny być one widoczne** po ponownym uruchomieniu kontenera.

Domyślnym zachowaniem kontenera po zatrzymaniu jest oczekiwanie na ponowne uruchomienie,
kontener nie jest usuwany po zakończeniu działania aplikacji.
Aby kontener usunął się automatycznie po zakończeniu działania aplikacji należy go 
uruchomić z parametrem ``--rm``:

.. code-block:: console
    :linenos:

    docker run -d --rm redis

System prune
``````````````

Docker przechowuje wszystkie obrazy, kontenery, wolumeny oraz sieci. Nagromadzenie danych może powodować, że na dysku zacznie brakować miejsca.
Aby usunąć wszystkie niepotrzebne rzeczy należy wywołać polecenie:

.. code-block:: console
    :linenos:

    docker system prune

Docker wyświetli informacje o konsekwencjach tego działania i zarząda powierdzenia:

.. code-block:: console
    :linenos:

    WARNING! This will remove:
    - all stopped containers
    - all networks not used by at least one container
    - all dangling images
    - all dangling build cache

Dodanie flagi ``--all`` wyszyści wszystkie składowane dane, zanim to się jednak stanie wyświetlana
jest informacja o plikach które zostaną usunięte. Konieczne jest potwierdzenie chęci wykonania tej operacji:

.. code-block:: console
    :linenos:

    WARNING! This will remove:
    - all stopped containers
    - all networks not used by at least one container
    - all images without at least one container associated to them
    - all build cache

Polecenie ``prune`` może zostać wywołane nie tylko na całym systemie, komendy
``container``, ``image``, ``network``, ``volume`` oferują komendę ``prune`` pozwalającą
na wyczyszczenie tylko wskazanego zasopu. Na przykład aby wyczyścić wszystkie nieużywane
obrazy należy wywołać polecenie:

.. code-block:: console
    :linenos:

    docker image prune

Dla części z tych poleceń zdefiniowana jest również flaga ``prune --all``.
Istnieje też możliwość filtrowania obrazów, kontenerów lubi sieci przy usuwaniu
za pomocą komendy ``prune``. Filtr `until` pozwala usunąć wszystkie przechowywane
dane według kryterium czasu stworzenia.

.. code-block:: console
    :linenos:

    docker image prune --filter "until=2019-10-21"

Powyższe polecenie spowoduje usunięcie obrazów utworzonych przed dniem 21.10.2019
Istnieje też możliwość użycia filtra ``until`` wraz z formatem dostępnym w języku Go oznaczającym czas trwania:

.. code-block:: console
    :linenos:

    docker image prune --filter "until=1h"

Uruchomienie powyższego polecenie spowoduje usunięcie obrazów starszych niż godzina.

Za pomocą polecenia:

.. code-block:: console
    :linenos:

    docker system df

Możliwe jest wyświetlenie podsumowania ukazującego ilość zajmowanego miejsca przez kolejno: obrazy,
kontenery, wolumeny oraz build cache.

Przykładowy wynik wywołania tego polecenia:

.. code-block:: console
    :linenos:

    TYPE                TOTAL               ACTIVE              SIZE                RECLAIMABLE
    Images              4                   2                   145.8MB             144.2MB (98%)
    Containers          2                   0                   481.8kB             481.8kB (100%)
    Local Volumes       0                   0                   0B                  0B
    Build Cache         0                   0                   0B                  0B


Uruchamienia procesu w działającym kontenerze
```````````````````````````````````````````````

Istnieje możliwość uruchomienia polecenia w już działającym kontenerze za pomocą polecenia ``exec``:

.. code-block:: console
    :linenos:

    docker run -d postgres

.. code-block:: console
    :linenos:

    docker exec <container-id> cat /var/lib/postgresql/data/postgresql.conf

Polecenie ``exec`` pozwala również uruchomić program w trybie interaktywnym. Jest to przydatne
np. gdy chcemy połączyć się do bazy danych działającej w kontenerze za pomocą narzędzi
dostępnych w jej pakiecie.

.. code-block:: console
    :linenos:

    docker exec -it <container-id> psql -U postgres

Możliwe jest również wywołanie ``/bin/bash`` i przeglądanie oraz modyfikacja zawartości systemu plików kontenera.

.. admonition:: Zadanie

    Uruchom kontener z aplikacją ``redis`` następnie zaloguj się do serwera za pomocą polecenia ``exec`` i ``redis-cli``.
    Jaki adres IP jest wyświetlany w konsoli oraz po wydaniu polecenia ``client list``?


Logs
``````

Domyślne działanie polecenia ``logs`` polega na wyświetleniu ``STDOUT`` oraz ``STDERR`` działającego kontenera:


.. code-block:: console
    :linenos:

    docker logs <container-id>

Jeśli aplikacja działająca w kontenerze nie wyświetla logów na standardowym wyjściu nadal możliwe jest ich
przeglądanie za pomocą polecenia ``exec``, konieczne wtedy jest uruchomienie ``/bin/bash`` lub
programów takich jak ``cat`` i odnalezienie pliku do którego aplikacja ta zapisuje logi.


Create i Start
````````````````

Istnieje możliwość utworzenia kontenera lecz nie uruchamiania go, słóży do tego polecenie ``docker create``.
Następnie można uruchomić kontener za pomocą polecenia ``docker start``.
Jest to przydatne np. gdy chcemy używać programu narzędziowego (np. GCC) znajdującego się w kontenerze.

.. admonition:: Zadanie

    Utwórz katalog a w nim prosty program w języku C:

    .. code-block:: C
        :linenos:

        #include <stdio.h>

        int main(void) {
            printf("Hello World!\n");
            return 0;
        }

    Następnie będąc w tym katalogu wywołaj polecenie (parametr ``--mount`` zostanie omówiony później):

    .. code-block:: console
        :linenos:

        docker create --name my-gcc --mount type=bind,src=$PWD,dst=/workdir gcc:9 gcc -o /workdir/main /workdir/main.c
        docker start my-gcc

    Zmodyfikuj kod programu tak, aby wymagał ponownej kompilacji następnie skompiluj i uruchom.

    Wprowadź błąd do programu, następnie uruchom proces kompilacji. Czy błąd jest widoczny na ekranie?


attach i interactive
----------------------

Możliwe jest dołączenie ``STDIN`` do uruchamianego kontenera za pomocą parametru ``--interactive`` bądź ``-i``
polecenia ``docker start``, jak również dołączenie ``STDOUT`` oraz ``STDERR`` za pomocą parametru ``--attach`` lub ``-a``.