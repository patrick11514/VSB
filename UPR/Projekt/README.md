# Breakout

### Parametry programu
Udávají:
--[paramter]=hodnota

Příklad:
```BASH
./main --help
./main --level=level1.txt
``` 

* **help**:
Zobrazí parametry programu

* **level**:
    Načte jeden, nebo více levelů
    V každém levelu jdou použít tyto barevné cihly:
    * Žlutá (Yellow) ![Yellow Brick](https://upload.patrick115.eu/screenshot/brick_yellow.png)
    * Zelená (Green) ![Green Brick](https://upload.patrick115.eu/screenshot/brick_green.png)
    * Šedá (Gray) ![Gray Brick](https://upload.patrick115.eu/screenshot/brick_gray.png)
    * Červená (Red) ![Red Brick](https://upload.patrick115.eu/screenshot/brick_red.png)
    * Fialová (Purple) ![Purple Brick](https://upload.patrick115.eu/screenshot/brick_purple.png)
    * Modrá (Blue) ![Blue Brick](https://upload.patrick115.eu/screenshot/brick_blue.png)

    Ve vestavěných levelech mají životy od Modré po Žlutou sestupně, takže:
    ![Blue Brick](https://upload.patrick115.eu/screenshot/brick_blue.png) 6 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)
    ![Purple Brick](https://upload.patrick115.eu/screenshot/brick_purple.png) 5 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)
    ![Red Brick](https://upload.patrick115.eu/screenshot/brick_red.png) 4 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)
    ![Gray Brick](https://upload.patrick115.eu/screenshot/brick_gray.png) 3 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)
    ![Green Brick](https://upload.patrick115.eu/screenshot/brick_green.png) 2 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)
    ![Yellow Brick](https://upload.patrick115.eu/screenshot/brick_yellow.png) 1 ![Hearth](https://upload.patrick115.eu/screenshot/hearth.png)

    Po každém zásahu se cihla promnění na tu pod ní a žlutá poté zmizí.

    

    ```TXT


    ```