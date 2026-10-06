# KFactorio (Kernel Factorio)
*Jedná se o portnutou část projektu z IvOS (Implementace v Operačních Systémech) kde jsem nahradil zápis do RAW VGA paměti zápisem do příslušných proc souborů :)*

Jedná se o jednoduchou ASCII factory hru, kde člověk staví malou továrnu, těží suroviny a snaží se vyrobit vědecké balíčky a po dopravení do laboratoře získá bod. Po získání 100 bodů "vyhrává".

Proc složka /proc/factorio obsahuje dva soubory:
- board -> readonly a vypisuje stav herní plochy
- cmd -> příjmá znaky pro ovládání jako WASD pro pohyb atd...

Tikání hry:
Hra se hýbe po "tikách" neboli "krocích", kdy každý krok se vykoná při úkonu (pohyb, položení/zničení něčeho), případně lze zmáčknout 'T' pro jeden krok hry.

Terén:
- # - železná ruda
- ~ - měděná ruda
- X - hráč

Je důležité řešit orientaci budov a pásů.

# TUI
Jednoduchý prográmek, který přemtne terminál do raw módu a posílá stisknuté klávesy do /proc/factorio/cmd a zároveň vždy vypíše aktualizovanou herní plochu. (Zavřít lze pak přes q nebo ^C - CTRL + C)

Jednoduchý gameplay video: https://www.youtube.com/watch?v=jZ4U5iRCLMc
A tady celý speedrun do 100 bodů: https://www.youtube.com/watch?v=_H1irA4BdGg 