# TODO k HTTP Serveru

- [X] Při requestu vypsat log o tom, že přišel request z IP metodou X na URL Y
- [x] Musím si udělat nějaký thread pool, protože když se accepte 1000 requestů na 1000 threadů, takže musím po maximálním zaplnění threadů nějakým způsobem requesty nechat čekat a když se uvolní, tak pak acceptnout connection
- [X] Fixnout to, že když sobor neexistuje, tak se nevypíše nic do konzole - přidat logger i pod ify

- [X] /home/patrick115/Projects/VSB/CPP/Projekt/server/modes/mainMode.cpp: In member function ‘virtual void MainMode::handleRequest(const ReceivedData&, const HTTPPayload&)’:
/home/patrick115/Projects/VSB/CPP/Projekt/server/modes/mainMode.cpp:7:77: warning: unused parameter ‘data’ [-Wunused-parameter]
    7 | void MainMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
      |     
 - [X] If configs directory doesn't exists, create it
 - [X] Load all .ini files from configs directory
 - [ ] fixnout parsování, tedy udělat to takto: Najít první \r\n -> první řádek, pak najít \r\n\r\n =>konec headerů a začátek contentu a headery separátnout zvlášť


### Milestone do půlky semestru
- [x] Zpracování HTTP requestů
- [x] Odeslání HTTP response
- [x] Kompletně funkční "dev" server 
- [x] Argumenty příkazové řádky: --dev, --host, --port
- [x] Obsloužení více uživatelů najednou (multithreading)


## Nginx Raspberry Pi 4B 4GB RAM
❯ wrk -t12 -c1000 -d2m http://10.10.10.223
Running 2m test @ http://10.10.10.223
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    38.11ms   69.26ms 975.37ms   96.79%
    Req/Sec     2.95k   548.76     9.04k    73.33%
  4158508 requests in 2.00m, 1.62GB read
Requests/sec:  34629.25
Transfer/sec:     13.84MB

## Můj server před úpravou
Running 30s test @ http://localhost:8080
  12 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    11.49ms   81.27ms   1.90s    98.18%
    Req/Sec   671.72    575.41     3.93k    73.67%
  107578 requests in 30.08s, 18.98MB read
  Socket errors: connect 0, read 107575, write 0, timeout 69
Requests/sec:   3576.47
Transfer/sec:    646.14KB

## Můj server po úpravě
❯ wrk -t12 -c1000 -d2m http://localhost:8080
Running 2m test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    43.99ms    8.46ms 256.49ms   81.00%
    Req/Sec     1.83k   281.15     4.44k    74.27%
  2624298 requests in 2.00m, 463.00MB read
  Socket errors: connect 0, read 2624287, write 0, timeout 0
Requests/sec:  21850.57
Transfer/sec:      3.86MB

## Last test
❯ wrk -t12 -c1000 -d2m http://localhost:8080
Running 2m test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    41.37ms    4.18ms 194.60ms   85.54%
    Req/Sec     2.00k   165.09     2.47k    74.16%
  2866387 requests in 2.00m, 505.72MB read
  Socket errors: connect 0, read 2866381, write 0, timeout 0
Requests/sec:  23867.37
Transfer/sec:      4.21MB

## Test with std::cout and std::cerr 
❯ wrk -t12 -c1000 -d30s http://localhost:8080
Running 30s test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    56.20ms    8.05ms 165.72ms   87.03%
    Req/Sec     1.48k   186.25     1.79k    80.88%
  529864 requests in 30.10s, 93.48MB read
  Socket errors: connect 0, read 529863, write 0, timeout 0
Requests/sec:  17603.35
Transfer/sec:      3.11MB

## Test with writing to files
❯ wrk -t12 -c1000 -d30s http://localhost:8080
Running 30s test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    46.76ms    6.80ms 140.49ms   80.83%
    Req/Sec     1.77k   219.55     2.35k    74.75%
  635170 requests in 30.05s, 112.06MB read
  Socket errors: connect 0, read 635133, write 0, timeout 0
Requests/sec:  21134.67
Transfer/sec:      3.73MB
