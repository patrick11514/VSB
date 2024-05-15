from django import forms

from .models import Figure, Image, User


class RegisterForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User
        fields = ["name", "email", "password"]


class LoginForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User
        fields = ["name", "password"]


class FigureForm(forms.ModelForm):
    class Meta:
        model = Figure
        exclude = ["added_by"]


class ImageForm(forms.ModelForm):
    class Meta:
        model = Image
        exclude = ["figure"]
