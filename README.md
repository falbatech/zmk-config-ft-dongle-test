# zmk-config-ft-dongle-test

**Test fizycznego połączenia GC9A01 ↔ nice!nano v2** dla projektu FT Dongle.

Ten firmware **NIE komunikuje się z GC9A01 przez SPI** - tylko pulsuje pin BL (backlight) przez PWM. To pierwszy krok testu hardware przed pełnym firmware.

## Pinout testowany

| GC9A01 | nice!nano | Funkcja |
|---|---|---|
| BL (pin 8) | D5 / P0.24 | PWM backlight |
| VCC (pin 1) | 3.3V | zasilanie |
| GND (pin 2) | GND | masa |

Pozostałe piny GC9A01 (CS, DC, RST, DIN, CLK) **nie są testowane** w tym kroku.

## Co powinieneś zobaczyć

Po sflashowaniu i podłączeniu USB powinien być pulsujący backlight w cyklu 4-sekundowym:

| Sekunda | Jasność BL |
|---|---|
| 0-1s | 50% (przyciemniony) |
| 1-2s | 100% (pełna jasność) |
| 2-3s | 0% (wyłączony) |
| 3-4s | 100% (pełna jasność) |

Sam ekran GC9A01 pokaże losowe szumy (bo SPI nie jest inicjalizowane) - to normalne. Patrzymy tylko na **jasność świecenia podświetlenia**.

## Diagnostyka

| Co widzisz | Co to znaczy |
|---|---|
| BL pulsuje 4-fazowo | ✅ Wszystko działa, idziemy dalej |
| BL zawsze świeci 100% | Sprawdź D5 lutowanie, możliwy short do VCC |
| BL zawsze wyłączony | Sprawdź lutowanie BL, sprawdź VCC i GND |
| Ekran szumi ale BL ciemny | BL nie podłączony - sprawdź pin 8 GC9A01 |
| Nic się nie dzieje | Sprawdź czy nice!nano dostał firmware (LED nice!nano powinien migać) |

## Build

GitHub Actions buduje 1 firmware: `ft_dongle_test-nice_nano_v2-zmk.uf2`

## Flashowanie

1. Podłącz nice!nano (z wlutowanym GC9A01) przez USB
2. Naciśnij szybko 2× reset
3. Pojawi się pendrive `NICENANO`
4. Przeciągnij `ft_dongle_test-nice_nano_v2-zmk.uf2`
5. Po 5 sekundach BL zacznie pulsować

## Jak działa kod

Plik `src/blink_test.c` używa Zephyr LED API + PWM driver:
- PWM0 channel 0 jest skonfigurowany na P0.24 (D5) w devicetree
- Pętla nieskończona ustawia jasność 50/100/0/100 z 1-sekundowymi przerwami

Plik się kompiluje w nasz custom module (CMakeLists.txt w roocie).

---

**Po pomyślnym teście** wracamy do głównego `zmk-config-ft-dongle` z pełnym firmware (Prospector + GC9A01 driver + LVGL widgets).
