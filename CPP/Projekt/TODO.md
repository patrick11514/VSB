# TODO k HTTP Serveru

- [X] Při requestu vypsat log o tom, že přišel request z IP metodou X na URL Y
- [ ] Musím si udělat nějaký thread pool, protože když se accepte 1000 requestů na 1000 threadů, takže musím po maximálním zaplnění threadů nějakým způsobem requesty nechat čekat a když se uvolní, tak pak acceptnout connection
