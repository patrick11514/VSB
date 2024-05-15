"""
URL configuration for Project project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""

from django.contrib import admin
from django.urls import path

from AnimeFigures import views

urlpatterns = [
    path("admin/", admin.site.urls),
    path("", views.index, name="index"),
    path("register", views.register, name="register"),
    # auth
    path("login", views.login, name="login"),
    path("logout", views.logout, name="logout"),
    # account
    path("user/<int:user_id>", views.user, name="user"),
    path("user/<int:user_id>/comment", views.user_add_comment, name="user_add_comment"),
    path("settings", views.settings, name="settings"),
    path("settings/changePassword", views.change_password, name="change_password"),
    path("settings/changeName", views.change_name, name="change_name"),
    path("settings/changeEmail", views.change_email, name="change_email"),
    # path("settings/deleteAccount", views.delete_account, name="delete_account"),
    # figures
    path("figure/<int:figure_id>", views.figure, name="figure"),
    path("figure/<int:figure_id>/addImage", views.add_image, name="add_image"),
    path("figure/<int:figure_id>/like", views.like, name="like"),
    path(
        "figure/<int:figure_id>/deleteImage/<int:image_id>",
        views.delete_image,
        name="delete_image",
    ),
    path("figure/<int:figure_id>/delete", views.delete_figure, name="delete_figure"),
    path(
        "figure/<int:figure_id>/comment",
        views.figure_add_comment,
        name="figure_add_comment",
    ),
    path("figure/add", views.add_figure, name="add_figure"),
    # search
    path("search", views.search, name="search"),
    # manufacturers
    path("manufacturers", views.manufacturers, name="manufacturers"),
    path("manufacturer/add", views.add_manufacturer, name="add_manufacturer"),
    path("manufacturer/<int:manufacturer_id>", views.manufacturer, name="manufacturer"),
    path(
        "manufacturer/<int:manufacturer_id>/remove",
        views.remove_manufacturer,
        name="remove_manufacturer",
    ),
]
