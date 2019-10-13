**********************
Docker Engine REST API
**********************

Docker daemon udostępnia API oparte o HTTP dzięki któremu aplikacje klienckie
takie jak ``docker-cli`` mogą zarządzać kontenerami, obrazami etc.
Istnieje również biblioteka dla języka python udostępniająca API do komunikacji z docker daemon,
dostępna jest pod adresem `Docker SDK <https://docker-py.readthedocs.io/en/stable/>`_.
Pełna dokumentacja REST API docker znajduje się pod adresem `Docker API <https://docs.docker.com/engine/api/v1.40/>`_.

Domyślnie docker engine nasłuchuje na ``Unix domain socket`` (``man 7 unix`` w konsoli wyświetli dokumentacje) ``/var/run/docker.sock``.
Program ``curl`` pozwala na komunikacje przez tego typu gniazda.

Przykładowo aby pobrać listę kontenerów należy wywołać polecenie:

.. code-block:: console
    :linenos:

    curl -X GET --unix-socket /var/run/docker.sock http:/v1.40/containers/json