from django.contrib import admin

from .models import (
    Figure,
    FigureComments,
    Image,
    Manufacturer,
    ProfileComments,
    User,
    UserLikes,
)

# Register your models here.
admin.site.register(Figure)
admin.site.register(FigureComments)
admin.site.register(Image)
admin.site.register(Manufacturer)
admin.site.register(ProfileComments)
admin.site.register(User)
admin.site.register(UserLikes)
