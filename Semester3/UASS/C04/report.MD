# Komunity sítí
## Rozdíl mezi NetworkX a Gephi pomocí různých algoritmů:
1. **Louvain** 
    Funguje ve dvou fázích:
    1. Všechny vrcholy hodí do svojí separatní komunity
    2. Prochází všechny vrcholy a hledá maximální pozitivní přírust modularity tím, že je zkouší přesouvat do sousedních komunit. Pokud není žádný přírust modularity, tak zůstane ve svojí vlastní komunitě.
    ### Dolphins
    #### NetworkX
    ![NetworkX Graph](https://upload.patrick115.eu/screenshot/DolphinsColoredPy.png)

    #### Gephi
    ![Gephi Graph](https://upload.patrick115.eu/screenshot/DolphinsColoredGephi2.png)


    ### Rozdíl
    Když se podíváme na rozdíl mezi NetworkX obarvením komunit a Gephi, tak na první pohled vidíme, že rozložení komunit se velmi podobá, ale při bližším prozkoumání si všimneme, že například vrchol 
    ![Node Gephi VS NetworkX](https://upload.patrick115.eu/screenshot/1GephiVSPy.png) spadá v NetworkX do komunity tmavě modré (u NetworkX), ale v Gephi již do růžové dole. Druhý vrhol ![Node 2 Gephi VS NetworkX](https://upload.patrick115.eu/screenshot/2GephiVSPy.png) tady u NetworkX spadá ještě do komunity, u které se nachází (Cyan u NetworkX a oranžová u Gephi), ale u Gephi již do zapadá do Dark Cyan barvy.

    ### Nereálná síť
    #### NetworkX
    ![NetworkX Graph](https://upload.patrick115.eu/screenshot/UnrealColoredPy.png)

    #### Gephi
    ![Gephi Graph](https://upload.patrick115.eu/screenshot/UnrealColoredGephi.png)

    ### Rozdíl
    Když se podíváme na tyto sítě, tak se jedná jednoduše o jednu velkou kouli, která má v sobě celkem dost komunit. Žádná z komunit není moc uzavřená, protože mezi ní je vždy mezi jejími některými členy jiná komunita. Například v NetworkX červeno-oranžová a v Gephi oranžová jsou komunity, které jsou rozprostřené po celé síti.

    ### Reálná síť (B-A)
    #### NetworkX
    ![NetworkX Graph](https://upload.patrick115.eu/screenshot/RealColoredPy.png)

    #### Gephi
    ![Gephi Graph](https://upload.patrick115.eu/screenshot/RealColoredGephi.png)

    ### Porovnání
    Když se podíváme na obě sítě, tak tvarově jsou velmi podobné, ale obravení je velmi odlišné. Komunit na základě NetworkX je o dost více oproti Gephi komunitám. Když se podíváme na Gephi, tak střed sítě tvoří ve většině jedna komunity (šedá) a do toho jsou přimíchané jiné. U NetworkX můžeme vidět, že všechny sítě jsou promícháne do sebe a žádná ne tak moc, jako u Gephi nedominuje středu sítě. U Gephi také můžeme vidět, že šedá síť má i trčející části ze sítě, za to u NetworkX ze sítě trčí více různobarevné komunity.
2. **Fluid Communities algorithm**
    Algoritmus je z počátku náhodný, jako první si vytvoří k komunit a do nich přiřadí náhodný vrchol. Poté postupně 