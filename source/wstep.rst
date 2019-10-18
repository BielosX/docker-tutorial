
********************************************
Zastosowanie kontenerów i maszyn wirtualnych
********************************************

Bezpieczeństwo
`````````````````

       aplikacja działająca na maszynie wirtualnej bądź w kontenerze nie ma bezpośredniego
       dostępu do systemu plików hosta. Niemożliwe jest również komunikowanie się z procesami
       działającymi w systemie hosta. Możliwość uruchomienia wirtualizowanego procesu w odseparowanej sieci.

Ujednolicenie narzędzi
`````````````````````````

       użycie tego samego obrazu zawierającego aplikacje niezbędną w procesie wytwarzania
       oprogramowania rozwiązuje problem różnic w wersjach tego oprogramowania na różnych systemach operacyjnych.
       Zestaw narzędzi nie musi być zainstalowany na komputerze hosta.

Rozwiązywanie konfliktów zależności
``````````````````````````````````````

       uruchomienie wielu aplikacji posiadających te same zależności lecz w różnych
       wersjach na tym samym komputerze może być niemożliwe. Rozwiązaniem tego problemu jest separacja tych
       aplikacji i utworzenie dla nich środowiska zapewniającego, że nie będzie działała tam żadna inna aplikacja
       mogąca wywołać potencjalne konflikty.

Łatwość wdrażania
```````````````````

       uruchomienie tej samej aplikacji na wielu serwerach wymaga jedynie skopiowania maszyny wirtualnej
       lub obrazu dockera na te urządzenia. Nie ma konieczności konfigurowania każdego z osobna.

Zarządzanie zasobami
``````````````````````

       zarówno kontenery jak i maszyny wirtualne pozwalają na deklarowanie ograniczeń związanych z zasobami
       komputera hosta. Można więc zdefiować rozmiar wirtualnego dysku, dostępną pamięć RAM oraz wykorzystanie procesora.