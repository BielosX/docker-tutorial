**************
Docker Network
**************

Doker pozwala definiować i zarządzać sieciami, kontenery znajdujące się w tej samej sieciami
mogą się ze sobą komunikować. Dostępna jest również usługa DNS która pozwala na komunikacje
z konterenem za pomocą nazwy nadanej przy uruchomieniu opcją ``--name``.
Aby wyświetlić dostępne sieci należy wydać polecenie:

.. code-block:: console
    :linenos:

    docker network ls

Jeśli wcześniej nie zdefiniowano żadnych sieci wynik tego polecenia powinien być następujący:

.. code-block:: console
    :linenos:

    NETWORK ID          NAME                DRIVER              SCOPE
    6d618acda4a0        bridge              bridge              local
    01bb0eee2409        host                host                local
    bdef2b995dfc        none                null                local

Widoczne sieci są domyślnymi zdefiniowanymi przez docker i oznaczają:

bridge
````````

Sieć w której każdy kontener otrzymuje nowy adres IP oraz gateway 172.17.0.1, komunikacja przebiega za pomocą interfejsu sieciowego ``docker0``

host
```````

Kontener otrzymuje adres IP hosta, z poziomu kontenera widoczny jest również most ``docker0``, możliwa jest komunikacja między
kontenerami. Jedyna możliwa metoda adresacji to porty. W przypadku konfliktu portów aplikacja nie uruchomi się.


none
``````

Z poziomu kontenera widoczny jest tylko interfejs o adresie ``127.0.0.1``. Nie jest możliwa żadna komunikacja

.. caution:: Domyślna sieć ``bridge`` nie udostępnia usługi DNS

   Docker pozwala nadawać kontenerom nazwy za pomocą parametru ``--name`` i używać tych nazw do komunikacji
   między kontenerami znajdującymi się w tej samej sieci. Wyjątkiem jest tu domyślna sieć ``bridge`` w której
   usługa DNS nie jest aktywna. Kominikacja między kontenerami musi więc odbywać się w oparciu o adresy IP.


Tworzenie nowej sieci
-----------------------

Aby uzyskać separacje między różnymi grupami kontenerów oraz skorzstać z dostępnej usługi DNS
konieczne jest utworzenie własnej sieci. Aby utworzyć sieć typu bridge należy użyć polecenia:

.. code-block:: console
    :linenos:

    docker network create --driver=bridge my-network

Gdzie ``my-network`` jest nazwą nowo tworzonej sieci. Driver typu ``bridge`` jest domyślny i jeśli chcemy
utworzyć sieć z użyciem tego drivera możemy pominąć opcję ``--driver``.

Uruchomienie kontenera w nowo utworzonej sieci:

.. code-block:: console
    :linenos:

    docker run --network my-network redis

Możliwe jest pobranie informacji o kontenerach należących do danej sieci:

.. code-block:: console
    :linenos:

    docker network inspect my-network

.. code-block:: console
    :linenos:

    [
        {
            "Name": "my-network",
            "Id": "836381a45afe02ee269d4aeaba838032e57d3df10f5fa0a170d95dd94dd5fd1e",
            "Created": "2019-10-15T23:40:29.291379758+02:00",
            "Scope": "local",
            "Driver": "bridge",
            "EnableIPv6": false,
            "IPAM": {
                "Driver": "default",
                "Options": {},
                "Config": [
                    {
                        "Subnet": "172.19.0.0/16",
                        "Gateway": "172.19.0.1"
                    }
                ]
            },
            "Internal": false,
            "Attachable": false,
            "Ingress": false,
            "ConfigFrom": {
                "Network": ""
            },
            "ConfigOnly": false,
            "Containers": {
                "a54712d64daa722acce9d412bd8c2a6fd7b7b777a4ec38a5a16b028a89980c49": {
                    "Name": "my-redis1",
                    "EndpointID": "78cf1ac91a7c3890e27760d833a96313de977f1b4e57b197b8e7a0e6945a5646",
                    "MacAddress": "02:42:ac:13:00:02",
                    "IPv4Address": "172.19.0.2/16",
                    "IPv6Address": ""
                }
            },
            "Options": {},
            "Labels": {}
        }
    ]


Wszystkie aktualnie działające kontenery znajdujące się w tej sieci znajdą się w słowniku `Containers`,
kluczami tego słownika są identyfikatory kontenerów.


Nadawanie nazwy
-----------------

Możliwe jest nadanie kontenerowi nazwy która będzie następnie używana przez usługę DNS, aby to zrobić
należy uruchomić kontener z opcją ``--network``

.. code-block:: console
    :linenos:

    docker run --network my-network --name my-redis redis


.. admonition:: Zadanie

    Zweryfikuj, czy nowo utworzony kontener jest dostępny pod nazwą ``my-redis``
