# Breakout

### Parametry programu
Udávají:
--[paramter]=hodnota

Příklad:
```JS
./main --help
./main --level=level1.txt
./main --level=level2.txt --includeDefaultLevels=true
``` 

* **help**:
Zobrazí parametry programu

* **level**:

    Načte jeden, nebo více levelů

    V každém levelu jdou použít tyto barevné cihly:
    * Žlutá (Yellow) ![Yellow Brick](https://upload.patrick115.eu/screenshot/brick_yellow.png)
    * Zelená (Lime) ![Lime Brick](https://upload.patrick115.eu/screenshot/brick_lime.png)
    * Šedá (Gray) ![Gray Brick](https://upload.patrick115.eu/screenshot/brick_gray.png)
    * Červená (Red) ![Red Brick](https://upload.patrick115.eu/screenshot/brick_red.png)
    * Fialová (Purple) ![Purple Brick](https://upload.patrick115.eu/screenshot/brick_purple.png)
    * Modrá (Blue) ![Blue Brick](https://upload.patrick115.eu/screenshot/brick_blue.png)


    Ve vestavěných levelech mají životy od Modré po Žlutou sestupně, takže:
    
    ![Blue Brick](https://upload.patrick115.eu/screenshot/brick_blue.png) 6 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)
    
    ![Purple Brick](https://upload.patrick115.eu/screenshot/brick_purple.png) 5 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)

    ![Red Brick](https://upload.patrick115.eu/screenshot/brick_red.png) 4 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)

    ![Gray Brick](https://upload.patrick115.eu/screenshot/brick_gray.png) 3 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)

    ![Lime Brick](https://upload.patrick115.eu/screenshot/brick_lime.png) 2 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)

    ![Yellow Brick](https://upload.patrick115.eu/screenshot/brick_yellow.png) 1 ![Heart](https://upload.patrick115.eu/screenshot/hearth.png)

    Po každém zásahu se cihla promnění na tu pod ní a Žlutá po zásahu zmizí.

    V každém levlu jsou potom definované životy.

    Nastavení levelu se vždy ukáže před hrou.

    Level může mít jakoukoliv příponu, jen musí obsahovat správně zadaný text.

    * **Ukázkové nastavení**:
        Nastavení vždy začíná slovíčkem "%Settings" a končí slovíčkem "%EndSettings".

        Mezi těmito značkami se poté definuje nastavení:
        * **Lives: číslo** definuje počet životů hráče **(Požadováné)**
        * **Score: číslo** definuje, kolik bodů dostane hráč za zničenou cihlu (počítá se i snížení životů) **(Požadované)**
        * **Description: text** Definuje krátký popisek levelu, který bude v seznamu levelů. (Max 20 znaků) **(Požadováné)**
        * **Barva Brick: číslo** definuje počet životů cihly
        * Nikdy nesmí existovat dvě cihly, které mají stejný počet životů.
        * Cihly, které nejsou definované v Settings nepůjdou použít

        ```YAML
        %Settings
        Lives: 5
        Score: 10
        Description: Level 2 (Těžší)
        Yellow Brick: 4
        Blue Brick: 5
        Gray Brick: 3
        Lime Brick: 2
        Yellow Brick: 1
        %EndSettings
        ```

    * **Ukázkový level**:
        Level vždy začíná slovíčkem "%Level" a končí slovíčkem "%EndLevel"

        Mezi těmito značkami se poté definuje level:
        * Level může mít maximální šířku <width>
        * Level může mít maximální výšku <height>
        * Pokud level bude v nějaké/obou velikostech menší, vycentruje se
        * Do levelu se vždy píšou první velké písmeny cihy, kterou jsme definovali v nastavení
        * Mezera = žádná cihla zde nebude

        ```YAML
        %Level
        
        %EndLevel
        ```

    * **Finální ukázkový soubor**:
        ```YAML
        %Settings
        Lives: 5
        Score: 10
        Description: Level 2 (Těžší)
        Yellow Brick: 4
        Blue Brick: 5
        Gray Brick: 3
        Lime Brick: 2
        Yellow Brick: 1
        %EndSettings
        %Level
        
        %EndLevel
        ```
* **includeDefaultLevels**:
    V základu je na false, takže pokud načtu nějaký svůj level, tak se načte pouze ten.

    Pokud ale chci načíst zákaldní levely + moje, tak tento argument dám na **true**.