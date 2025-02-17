# Tondík Web Server

## Milestone do půlky semestru
- [x] Zpracování HTTP requestů
- [x] Odeslání HTTP response
- [x] Kompletně funkční "dev" server 
- [x] Argumenty příkazové řádky: --dev, --host, --port
- [x] Obsloužení více uživatelů najednou (multithreading)

## CMD Line

```BASH
> ./main --help
Tondík Web Server

Argument types:
 --name=value
 -name value

Argument list:
 --help - shows this help
Dev mode:
 --dev - start server in dev mode
 --host - start dev server on 0.0.0.0 interface
 --port=8080 - start dev server on specific port
 --path=/path/to/folder - server files in specified folder
Server mode:
 --path=/etc/tondik - path to config folder
```

## Modes
### Dev mode (Fully implemented)
Used to serve only one folder. By default started on 127.0.0.1 on port 8080 and it will be serving ./ folder. 

#### Parameters
- --dev - start dev mode
- --host - start on 0.0.0.0
- --port=1234 - change port
- --path=/path/to/folder - change folder

### Server mode (Fully implemented)
Used to serve multiple folders based on domains/subdomains/hostnames.

#### Start
- On default port 80:
```BASH
sudo ./main
```
*Port can by changed in config file, default ./main.ini*

```BASH
./main --path=/path/to/config/folder
```

#### Config file specifications (Based on INI)
- key=value - set value to key
- ;comment - single line comments
- [section] - section, keys under this section will be: section.key, section will be ended when another section starts
- array[]=value - add value to array
- if something is before comment, then it isn't threated as comment, so: 
mime_types[]=test,text/plain; charset=UTF-8
will be treated as new item to mime_type array with value "test,text/plain; charset=UTF-8"

#### Main config file
```INI
port=80
;directory, where domain configs will be located
config_directory=/etc/tondik/configs
;if local=false, server is started on 0.0.0.0, if local=true then server is started on 127.0.0.1 (optional) - default value false
local=false
;custom response headers (optional) -  array of strings
headers[]=Server: SuperWebServer
headers[]=Header: CustomHeaderValue
;custom mime types (optional) - array of strings
;format: "extension,mime_type"
mime_types[]=php,text/php
mime_types[]=html,text/html
mime_types[]=htm,text/html
access_log=/var/log/webserver/access.log
error_log=/var/log/webserver/error.log
; default root folder if no domain is found
default_root=/var/www/default
; default index file if no domain is found (optional), because it will use index.html by default
index=index.html
```

#### Config for domain/subdomain/hostname
- serving specific folder
    ```INI
    domain=example.com
    root=/var/www/example.com
    ; default index will be index.html, but can be changed by this option
    index=index.htm
    ; access_log and error_log is optional and will be inherited from main config
    ; but when specifying custom paths fro access and error log, both must be set
    ; otherwise single option will be ignored
    access_log=/var/log/webserver/example.com/access.log
    error_log=/var/log/webserver/example.com/error.log
    ```
- proxy pass (reverse proxy)
    ```INI
    domain=something.example.com
    reverse_proxy=http://localhost:8080
    access_log=/var/log/webserver/example.com/access.log
    error_log=/var/log/webserver/example.com/error.log
    ```

## Specification
- support only static files (html,css,js,videos,images...)
- support for HTTP 1.1
- doesn't support keep-alive connection
- support partial content:
    - when streaming video file, for example in vlc, you can skip through video file
    - supported only in VLC, because browsers are using same connection to do this and we don't support keep-alive connections

## Testing
### Dev mode
- Logs are written in /tmp/log.txt
    ```BASH
    ❯ wrk -t12 -c1000 -d30s http://localhost:8080
    Running 30s test @ http://localhost:8080
      12 threads and 1000 connections
      Thread Stats   Avg      Stdev     Max   +/- Stdev
        Latency    38.46ms    4.07ms 151.68ms   91.13%
        Req/Sec     2.15k   191.48     2.56k    82.77%
      773070 requests in 30.10s, 343.56MB read
      Socket errors: connect 0, read 773046, write 0, timeout 0
    Requests/sec:  25683.19
    Transfer/sec:     11.41MB
    ```
- Logs are written to stdout
    ```BASH
    ❯ wrk -t12 -c1000 -d30s http://localhost:8080
    Running 30s test @ http://localhost:8080
      12 threads and 1000 connections
      Thread Stats   Avg      Stdev     Max   +/- Stdev
        Latency    50.21ms    5.30ms 140.66ms   85.17%
        Req/Sec     1.65k   150.66     2.26k    79.20%
      592907 requests in 30.10s, 263.50MB read
      Socket errors: connect 0, read 592884, write 0, timeout 0
    Requests/sec:  19697.31
    Transfer/sec:      8.75MB
    ```
### Server mode

- Static files
❯ wrk -t12 -c1000 -d30s http://prvnistranka:8080
Running 30s test @ http://prvnistranka:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    38.23ms    6.38ms 132.58ms   78.05%
    Req/Sec     2.16k   313.94     3.09k    68.12%
  775178 requests in 30.10s, 1.00GB read
  Socket errors: connect 0, read 775161, write 0, timeout 0
Requests/sec:  25754.62
Transfer/sec:     33.92MB

- Reverse proxy
❯ wrk -t12 -c1000 -d30s http://druhastranka:8080
Running 30s test @ http://druhastranka:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   418.07ms   29.97ms 459.64ms   97.89%
    Req/Sec   210.67    109.07   595.00     65.80%
  71001 requests in 30.05s, 604.70MB read
  Socket errors: connect 0, read 71001, write 0, timeout 0
Requests/sec:   2362.55
Transfer/sec:     20.12MB