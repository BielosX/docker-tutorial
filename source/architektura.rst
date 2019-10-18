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