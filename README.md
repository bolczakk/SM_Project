**Microprocessor Temperature Control System**
Projekt realizowany w ramach przedmiotu Systemy Mikroprocesorowe na Politechnice Poznańskiej. System stanowi zamkniętą pętlę regulacji temperatury z wizualizacją danych w czasie rzeczywistym przez Ethernet.
**Architektura systemu**
-BlackPill: Odczyt temperatury (DS18B20), sterowanie wentylatorem (PWM), lokalny interfejs użytkownika (OLED SSD1306).
-Nucleo F767ZI: Bramka komunikacyjna UART-to-Ethernet wykorzystująca stos LwIP.
-Stacja nadrzędna (PC): Serwer Node.js z frontendem w JavaScript wyświetlającym dane procesowe, umożliwiającym zapisywanie pomiarów.
**Instrukcja uruchomienia**
1. Skonfiguruj adres IP.
2. Uruchom serwer JavaScript.
3. Podłącz Nucleo przez Ethernet do komputera (dane są pobierane co 5 sekund).
<img width="1920" height="1032" alt="Zrzut ekranu 2026-01-30 215703" src="https://github.com/user-attachments/assets/3a185073-bdb1-45e7-bdbb-a4f974d0ddc9" />
