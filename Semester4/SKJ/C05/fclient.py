from xmlrpc.client import ServerProxy


class ForecastException(Exception):
    pass


def print_forecast(fserver, date):
    print("Forecast for " + date)
    try:
        print(fserver.get_forecast(date))
    except ForecastException as e:
        print("Forecast error: " + e)


fserver = ServerProxy("http://localhost:10001")

print_forecast(fserver, "2012-11-05")
print_forecast(fserver, "2012-11-06")
print_forecast(fserver, "2012-11-07")
print_forecast(fserver, "2012-11-08")

print("Updating ... ", end="")
print(
    fserver.update_forecast(
        "master-of-weather", "2012-11-09", "rains dogs and cats", 10.3, 3.0
    )
)
print(
    fserver.update_forecast(
        "master-of-weatherRANDOMHESLO", "2012-11-09", "rains dogs and cats", 10.3, 3.0
    )
)

print_forecast(fserver, "2012-11-09")

# Updating existsing one
print(
    fserver.update_forecast("master-of-weather", "2012-11-07", "velbloudi", 10.3, 3.0)
)
print_forecast(fserver, "2012-11-07")
