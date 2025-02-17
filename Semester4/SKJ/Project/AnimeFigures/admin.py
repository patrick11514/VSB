from django.contrib import admin

from .models import (
    Figure,
    FigureComment,
    Image,
    Manufacturer,
    ProfileComment,
    User,
    UserLike,
)

# Register your models here.
admin.site.register(Figure)
admin.site.register(FigureComment)
admin.site.register(Image)
admin.site.register(Manufacturer)
admin.site.register(ProfileComment)
admin.site.register(User)
admin.site.register(UserLike)
