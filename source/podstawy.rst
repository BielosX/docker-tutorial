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