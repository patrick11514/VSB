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
    # figures
    path("figure/<int:figure_id>", views.figure, name="figure"),
    path("figure/<int:figure_id>/addImage", views.add_image, name="add_image"),
    path("figure/<int:figure_id>/like", views.like, name="like"),
    path("figure/add", views.add_figure, name="add_figure"),
    path("search", views.search, name="search"),
    path("manufacturers", views.manufacturers, name="manufacturers"),
]
