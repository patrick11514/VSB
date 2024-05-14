import datetime


def dateDiff(date1: datetime.date, date2: datetime.date) -> int:
    """
    Calculate the difference between two dates and format it to years/months/days/hours/minuts/seconds, depends on the difference.
    """
    datesDiff = date1 - date2

    diff = abs(datesDiff.days / 365.25)

    if diff > 0:
        return f"{diff} let"

    diff = abs(datesDiff.days / 30)

    if diff > 0:
        return f"{diff} měsíců"

    diff = abs(datesDiff.days)

    return f"{diff} dní"
