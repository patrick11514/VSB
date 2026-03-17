## SQL Server

4.4:
- **IO Cost:** 1605
- **CPU Time:** 78 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/75800180-7f19-4df2-81d4-36c7ff9aa25a.png)
- **Proč:** Protože nemá žádný index a tak musel prokenovat celou haldu a pro každý záznam ručně provést porovnání atributů

4.6:
- **IO Cost:** 24
- **CPU Time:** 0 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/244a4f18-48b3-42e2-bb84-2132079e01bd.png)
- **Proč:** Nyní má DBS vytvořený složený klíč pro všechny atributy a je pro něho výhodné si v B-Stromu najít dané záznamy a poté přečíst pouze daný záznam z haldy.

4.7:
- **IO Cost:** 93
- **CPU Time:** 0 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/508eb1f6-689c-4437-b0c4-cc47cdf5a620.png)
- **Proč:** Selektivita dotazu je 0.0297% tedy stále nízká a DBS se stále vyplatí využít index, místo seq read.

4.8:
- **IO Cost:** 1069
- **CPU Time:** 62 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/3e01217e-47ec-4a6b-85b6-5ef3e06c509b.png)
- **Proč:** Tady se DBS rozhodl použít sekvenční čtení haldy, důvodem může být nižší odhadnutý CPU time. 

4.9:
- **IO Cost:** 7
- **CPU Time:** 0 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/2f2d1b71-cb30-4be7-9cab-876368ecd2ac.png)
- **Proč:** Zde DBS využil pouze operaci seek a přeskočil RID lookup, protože my chceme pouze lname a fname, které již v b-stromu jsou uloženy

4.10:
- **IO Cost:** 1512
- **CPU Time:** 78 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/63d39c53-96ec-4ec4-98dc-76fb112fb839.png)
- **Proč:** Zde musí DBS použít sequential read přes haldu, protože index je vytvořen lexikograficky na fname, lname a residence a zde v selekci vynecháváme lname a tím index využít nemůže.

4.11:
- **Halda:** 1606 pages / 1609 total pages
- **PK__Customer__DC501A0C5BE68E27:** 673 pages / 697 total pages
- **idx_customer_ln_fn_rs:** 1515 pages / 1609 total pages
- **Proč:** Tady můžeme vidět, že dohromady náš vytvořený index má skoro stejně stránek jako samotná halda, dohromady s indexem primárního klíče zabírají indexy dohromady více stránek, než samotná data na haldě, proto je v tomto případě dobré se rozhodnout, zda je opravdu vhodné mít náš vlastní index (např. rychlejší dotazy s nízkou selektivitou atd...).

## Oracle

4.4:
- **IO Cost:** 1865
- **CPU Time:** 28 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/c2a85f64-7fd0-4bbb-a43b-b185df6d77d3.png)
- **Proč:** Protože nemá žádný index a tak musel prokenovat celou haldu a pro každý záznam ručně provést porovnání atributů

4.6:
- **IO Cost:** 24
- **CPU Time:** 300 us
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/d7f2762d-dab4-475a-adc1-13e1cf1255bc.png)
- **Proč:** Nyní má DBS vytvořený složený klíč pro všechny atributy a je pro něho výhodné si v B-Stromu najít dané záznamy a poté přečíst pouze daný záznam z haldy.

4.7:
- **IO Cost:** 199
- **CPU Time:** 15 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/f87234ed-2f1b-4ef1-9b83-2047c7a31777.png)
- **Proč:** Selektivita dotazu je 0.0297% tedy stále nízká a DBS se stále vyplatí využít index, místo seq read.

4.8:
- **IO Cost:** 1248
- **CPU Time:** 2 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/8374ccf1-6629-42cd-9b4b-d91b7e55905c.png)
- **Proč:** Tady se DBS rozhodl použít sekvenční čtení haldy, důvodem může být nižší odhadnutý CPU time.

4.9:
- **IO Cost:** 14
- **CPU Time:** 2 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/293a80df-6653-4d51-bf59-3793165273af.png)
- **Proč:** Zde DBS využil pouze operaci seek a přeskočil RID lookup, protože my chceme pouze lname a fname, které již v b-stromu jsou uloženy

4.10:
- **IO Cost:** 34
- **CPU Time:** 13 ms
- **Plan:** ![Plan](https://upload.patrick115.eu/raw/images/863f741e-63c1-4e85-9577-7e0cd29ed772.png)
- **Proč:** 

4.11:
- **Halda:** 1606 pages / 1609 total pages
- **PK__Customer__DC501A0C5BE68E27:** 673 pages / 697 total pages
- **idx_customer_ln_fn_rs:** 1515 pages / 1609 total pages
- **Proč:** Tady můžeme vidět, že dohromady náš vytvořený index má skoro stejně stránek jako samotná halda, dohromady s indexem primárního klíče zabírají indexy dohromady více stránek, než samotná data na haldě, proto je v tomto případě dobré se rozhodnout, zda je opravdu vhodné mít náš vlastní index (např. rychlejší dotazy s nízkou selektivitou atd...).
