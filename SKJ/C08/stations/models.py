from django.db import models

# Create your models here.


class Station(models.Model):
    name = models.CharField(max_length=30)
    address = models.CharField(max_length=150)
    active = models.BooleanField(default=False)
    created = models.DateTimeField("created date", auto_now_add=True)
    cars_capacity = models.IntegerField()
    company = models.ForeignKey(
        "Company", null=True, blank=True, on_delete=models.CASCADE
    )

    def __str__(self) -> str:
        return f"{self.name} - {self.address}, kapacita: {self.cars_capacity}"


class Company(models.Model):
    name = models.CharField(max_length=30)

    def __str__(self) -> str:
        return self.name
