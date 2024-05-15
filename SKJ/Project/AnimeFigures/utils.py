import datetime


def dateDiff(date1: datetime.datetime, date2: datetime.datetime) -> int:
    """
    Calculate the difference between two dates and format it to years/months/days/hours/minuts/seconds, depends on the difference.
    """
    datesDiff = date2 - date1

    diff = round(abs(datesDiff.days / 365.25))

    if diff > 0:
        return f"{diff} let"

    diff = round(abs(datesDiff.days / 30))

    if diff > 0:
        return f"{diff} měsíců"

    diff = round(abs(datesDiff.days))

    if diff > 0:
        return f"{diff} dní"

    diff = round(abs(datesDiff.seconds / 3600))

    if diff > 0:
        return f"{diff} hodin"

    diff = round(abs(datesDiff.seconds / 60))

    if diff > 0:
        return f"{diff} minut"

    return f"{abs(datesDiff.seconds)} sekund"
