## WebServer

### Funkce
- Web server bude fungovat pouze na HTTP protokolu.
- Web server půjde spustit několika způsoby:
 1. V "dev" módu, kdy se server spustí v základu na localhostu, případně přes argument --host na všech interfacech (0.0.0.0) a vybraném portu (defaultně 8080) a bude sloužit pro servování jen jedé složky (defaultně ./, přépadně lze změnit pomocí argumentu --path).
 2. V "server" módu, kdy se spustí na všech interfacech (0.0.0.0) na daném portu (defaultně 80) a bude pomocí konfiguračních souborů servovat více složek na základě domén/subdomén... (podobně jako například Nginx). Do parametru se poté přidá root složka webserveru, kde bude základní config...
- Web server bude umět servovat statické soubory (HTML, CSS, JS, obrázky, fonty, ...).
- Web server bude umět obsloužit více uživatelů najednou.

### Milestone do půlky semestru
- [x] Zpracování HTTP requestů
- [x] Odeslání HTTP response
- [x] Kompletně funkční "dev" server 
- [x] Argumenty příkazové řádky: --dev, --host, --port
- [x] Obsloužení více uživatelů najednou (multithreading)

### Config soubor
#### Hlavní soubor
```INI
port=80
configs_folder=/etc/webserver
;custom response headers (optional) -  array of strings
headers[] = "Server: SuperWebServer"
headers[] = "Header: CustomHeaderValue"
;custom mime types (optional) - array of strings
;format: "extension,mime_type"
mime_types[] = "php,text/php"
mime_types[] = "html,text/html"
mime_types[] = "htm,text/html"
access_log=/var/log/webserver/access.log
error_log=/var/log/webserver/error.log
; default root folder if no domain is found
default_root=/var/www/default
; default index file if no domain is found (optional), because it will use index.html by default
index=index.html
```

#### Konfigurační soubory
```INI
domain=example.com
root=/var/www/example.com
; v základu bude brát jako index soubor index.html
; lze změnit přes index=index.htm například
index=index.htm
access_log=/var/log/webserver/example.com/access.log
error_log=/var/log/webserver/example.com/error.log
```

nebo

```INI
domain=something.example.com
reverse_proxy=http://localhost:8080
access_log=/var/log/webserver/example.com/access.log
error_log=/var/log/webserver/example.com/error.log
```

