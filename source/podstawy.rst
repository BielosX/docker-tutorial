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

**docker run IMAGE [COMMAND] [ARG...]**

Zastosowana tutaj notacja oznacza, że podanie nazwy obrazu jest obowiązkowe natomiast zarówno komenda jak i jej argumenty są opcjonalne.
Uruchomienie polecenia

.. code-block:: console
    :linenos:

    docker run busybox

bez podania programu do wykonania jako argumentu zakończy natychmiast działanie kontenera. W kolejnych przykładach takie wywołanie
będzie jednak bardziej przydatne.


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