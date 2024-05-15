import datetime

from django.db import models

from .utils import dateDiff

# Create your models here.


class User(models.Model):
    name = models.CharField(max_length=25)
    email = models.EmailField()
    password = models.CharField(max_length=255)
    added = models.DateTimeField(auto_now_add=True)

    def __str__(self) -> str:
        return self.name

    def getDate(self):
        return self.added.strftime("%d.%m.%Y %H:%M:%S")

    def getDiff(self):
        return dateDiff(self.added, datetime.datetime.now(datetime.timezone.utc))


class Manufacturer(models.Model):
    name = models.CharField(max_length=50)
    country = models.CharField(max_length=50)

    def __str__(self) -> str:
        return self.name


class Figure(models.Model):
    name = models.CharField(max_length=50)
    anime = models.CharField(max_length=50)
    size = models.IntegerField()
    weight = models.FloatField()
    price = models.FloatField()
    year = models.IntegerField()
    manufacturer = models.ForeignKey(
        "Manufacturer", null=False, blank=False, on_delete=models.CASCADE
    )
    added = models.DateTimeField(auto_now_add=True)
    added_by = models.ForeignKey("User", on_delete=models.SET_NULL, null=True)

    def __str__(self) -> str:
        return self.name

    def getDate(self):
        return self.added.strftime("%d.%m.%Y %H:%M:%S")

    def getDiff(self):
        return dateDiff(self.added, datetime.datetime.now(datetime.timezone.utc))


class Image(models.Model):
    url = models.URLField()
    figure = models.ForeignKey("Figure", on_delete=models.CASCADE)

    def __str__(self) -> str:
        return f"Image of {self.figure.name}"


class UserLike(models.Model):
    user = models.ForeignKey("User", on_delete=models.CASCADE)
    figure = models.ForeignKey("Figure", on_delete=models.CASCADE)

    def __str__(self) -> str:
        return f"User {self.user.name} liked {self.figure.name}"


class FigureComment(models.Model):
    user = models.ForeignKey("User", on_delete=models.CASCADE)
    figure = models.ForeignKey("Figure", on_delete=models.CASCADE)
    comment = models.TextField()
    date = models.DateTimeField(auto_now_add=True)

    def __str__(self) -> str:
        return f"{self.pk} - {self.user.name} commented on {self.figure.name}"

    def getDate(self):
        return self.date.strftime("%d.%m.%Y %H:%M:%S")

    def getDiff(self):
        return dateDiff(self.date, datetime.datetime.now(datetime.timezone.utc))


class ProfileComment(models.Model):
    user = models.ForeignKey("User", related_name="user", on_delete=models.CASCADE)
    profile = models.ForeignKey(
        "User", related_name="profile", on_delete=models.CASCADE
    )
    comment = models.TextField()
    date = models.DateTimeField(auto_now_add=True)

    def __str__(self) -> str:
        return f"{self.pk} - {self.user.name} commented on {self.profile.name}"

    def getDate(self):
        return self.date.strftime("%d.%m.%Y %H:%M:%S")

    def getDiff(self):
        return dateDiff(self.date, datetime.datetime.now(datetime.timezone.utc))
