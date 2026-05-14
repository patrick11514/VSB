## Kontrakt v sobě ukládá:

- adresu prodávajícího
- název + popis aukce
- originální sha256 checksum souboru (pro pozdější ověření)
- IPFS handle (hash) pro načtení souboru
- sha256 checksum enryptovaného souboru nahraného na IPFS
- sha256 hash passphrase pro odemčení souboru
- odhaledná passphrase, aby se měl jak dostat k výhercovi (zadává se po skončení aukce)
- čas konce aukce
- aktuální nejvyšší nabídka a adresu nejvyššího nabízející
- stav aukce (bidding, waiting passphrase, completed)

## Stavy:

- Bidding: probíhá aukce, lze bidovat (now < endAt)
- WaitingPassphrase: aukce skončila a čeká se až prodávající zadá passphrase (now >= endAt) (pouze "virtuální stav", reálně není nikdy nastaven)
- Completed: prodávající zadal passphrase, obdržel výdělek a výherce si může zobrazit passphrase k odemčení souboru

## Modifikátory:

- `onlyOwner` (basically je adresa od ownera? - tvůrce aukce)
- `onlyWinner` (jedná se o nejvyššího sázejícího?)
- `auctionEnded` (now >= endAt)
- `inState` (quick makro)

## Utility funkce:

- `isOwner`, `isWinner`, `canRefund` a `canEndAuction` pro lepší stav UI, jsou to pouze view, nemodifikují stav kontraktu
- `checkStatus` opět view a poskytuje všechna data, která jsou veřejně dostupná na web

## Funkce:

### bid

- Payable funkce
- Lze pouze provádět ve stavu bidding
- Kontolujeme, ještě jestli aukce již neskončila
- Dále nemůžeme bidnout zápornou částku
- Pokuď již daná adresa bidovala, přičteme ji k jejímu předchozímu bidu, jinak vytvoříme nový záznam v mappingu `bidders`

- Kontrolujeme, zda je newBid > currentHighestbid, pokud ano, provedeme aktualizaci nového biddera + nabídky (korektní stav)

### endAuction

- Testovací funkce, pro okamžitou modifikaci endAt na aktuální čas: TLDR okamžitě ukončená aukce
- Může to dělat pouze owner
- A musí být ve stavu bidding

### revealPassphrase

- Jako vstup bere string
- Může pouze owner, aukce musí být ukončena a ve stavu bidding (protože stav WaitingPassphrase je pouze "virtuální")
- Kontrolujeme, že owner zadává frázi shodnou s uloženým hashem fráze
- Po kontrole, aktualizace stavu + uložení fráze
- Pokuď na akci nikdo nevloží žádné peníze a je ukončena, owner nedostane žádnou odměnu (tedy mu jsou peníze pouze vyplaceny pokud někdo bidnul)

### givePassphrase

- Pouze pro výherce, a pouze pokud je stav Completed
- Vrací passphrase a IPFS handle, aby se výherce mohl dostat k souboru

### refund

- Všichni, kdo nejsou nejvyšší biddeři
- Mohou v jakémkoliv stavu, ale musí mít kladný zůstatek (tedy něco museli bidnout)
- Pokuď nic nebidnuli, nic nedostanou
- Po refundaci se jejich zůstatek v mappingu `bidders` nastaví na 0, aby nemohli refundovat vícekrát

## Flow stavů

1. Bidding: kontrakt byl vytvořen, aukce probíhá, lidé mohou bidovat
2. Aukce skončila (now >= endAt), ale prodávající ještě nezadal passphrase, takže stav je "WaitingPassphrase" (virtuální, není reálně nastaven, ale kontrakt se chová, jako by byl)
3. Prodávající zadá passphrase, kontrakt zkontroluje jeho hash, pokud je správný, nastaví stav na Completed, a umožní výherci získat passphrase a IPFS handle pro odemčení souboru a zároveň jako odměnu získane výdělek od nejvyššího biddera. (Tady probíhá priorita prodávajícího...)
