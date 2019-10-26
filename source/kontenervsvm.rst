
*****************************
Kontener vs Maszyna Wirtualna
*****************************

.. admonition:: Zadanie

    1. Sprawdź wersje jądra ``linux`` w kontenerze opartym na obrazie ``debian:jessie``.
    2. Sprawdź wersje jądra ``linux`` w repozytorium odpowiadającym wersji ``jessie`` dystrybucji ``debian``.


Wirtualizacja
```````````````

Pod tym pojęciem kryje się wiele rozwiązań mających na celu zapewnienie izolacji programów lub całych systemów
operacyjnych od systemu operacyjnego na którym te programy działają.
Rozwiązania te różnią się poziomek wykorzystania zasobów maszyny na której ów program jest uruchamiany.
Istniają systemy zapewniające pełną emulacje urządzenia, każdej wykonywanej instrukcji oraz podzespołów.
Przykładem takiego rozwiązania jest emulator platformy *x86* `Bochs <http://bochs.sourceforge.net/>`_, emuluje
on procesor *x86* dekodując programowo każdą jego instrukcje i tłumacząc na ciąg poleceń
modyfikujących znajdujący się w pamięci model procesora. Wykonanie pojedynczych instrukcji
procesora za pomocą wielu instrukcji procesora wykonującego powoduje znaczne spowolnienie wykonania programu.
Tego typu emulacja jest na przykład stosowana w przypadku uruchamiania gier z platformy *GameBoy* na komputerze
PC. Różnice architektury sprzętowej w tym przypadku wymagają symulowania instrukcji procesora platformy *GameBoy*.

Rozwiązania takie jak `VirtualBox <https://www.virtualbox.org/>`_ czy `VMware Workstation <https://www.vmware.com/products/workstation-pro.html>`_
pozwalają na uruchomienie oprogramowania wraz z systemem operacyjnym w taki sposób, aby instrukcje wykonywanych programów
mogły wykonywać się bezpośrednio na procesorze hosta. Nadal jednak pozostaje problem wywołań systemowych,
czyli funkcji dostarczanych przez system operacyjny. W środowisku *x86* oraz *amd64* dostępnych jest kilka
poziomów bezpieczeństwa oraz istnieje szereg instrukcji procesora, które mogą zostać wywołane jedynie
przez program działający w trybie uprzywilejowanym, takim programem jest jądro systemu operacyjnego.
W systemach wirtualizowanych nie jest możliwe wykonanie wywołania systemowego w normalny sposób gdyż
skutkowałoby to wyłowaniem funkcji systemowej oferowanej przez system hosta, który może być różny
od systemu wirtualizowanego. Konieczne jest więc rozwiązanie tego problemu w inny sposób, który
może powodować widoczne spowolnienie wirtualizowanych programów w stosunku do natywnego systemu.

Kontenery
```````````

Naprzeciw powyrzszym problemom wychodzą kontenery. Jest to rozwiązanie oferujące separacje procesu
od systemu hosta w taki sposób, że system ten nie jest świadomy innych procesów, nie współdzieli z nimi
systemu plików, zasobów sieciowych etc. Różnica między wirtualizacją polega na tym,
że uruchamiany proces nadal jest procesem systemu hosta i jest przez niego zarządzany.
W rozwiązaniu tym w kontenerze nie działa inny system operacyjny oraz nie jest w nim
odwzorowywane całkowicie środowisko konteneryzowanego systemu.
Zaletą tego rozwiązania jest krótki czas oczekiwania na gotowość kontenera oraz mały rozmiar
jego obrazu. Rozwiązanie to ma również swoje konsekwencje, co pokazuje zadanie otwierające ten rozdział.
Kontenery silnie wykorzystują własności systemu operacyjnego hosta.