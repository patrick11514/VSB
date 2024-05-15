from django import forms

from .models import Figure, FigureComment, Image, Manufacturer, ProfileComment, User


class RegisterForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput(), label="Heslo")

    class Meta:
        model = User
        fields = ["name", "email"]
        labels = {"name": "Jméno", "email": "Email"}


class LoginForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput(), label="Heslo")

    class Meta:
        model = User
        fields = ["name"]
        labels = {"name": "Jméno"}


class FigureForm(forms.ModelForm):
    class Meta:
        model = Figure
        exclude = ["added_by"]
        labels = {
            "name": "Jméno",
            "anime": "Anime",
            "size": "Velikost (cm)",
            "weight": "Váha (g)",
            "price": "Cena (Kč)",
            "year": "Rok",
            "manufacturer": "Výrobce",
        }


class ImageForm(forms.ModelForm):
    class Meta:
        model = Image
        exclude = ["figure"]
        labels = {
            "url": "Odkaz na obrázek",
        }


class ManufacturerForm(forms.ModelForm):
    class Meta:
        model = Manufacturer
        exclude = []
        labels = {
            "name": "Jméno",
            "country": "Země",
        }


class UserCommentForm(forms.ModelForm):
    class Meta:
        model = ProfileComment
        fields = ["comment"]
        labels = {
            "comment": "Komentář",
        }


class FigureCommentForm(forms.ModelForm):
    class Meta:
        model = FigureComment
        fields = ["comment"]
        labels = {
            "comment": "Komentář",
        }


class PasswordChangeForm(forms.Form):
    old_password = forms.CharField(label="Staré heslo", widget=forms.PasswordInput())
    new_password = forms.CharField(label="Nové heslo", widget=forms.PasswordInput())


class NameChangeForm(forms.Form):
    name = forms.CharField(label="Nové jméno", widget=forms.TextInput())


class EmailChangeForm(forms.Form):
    email = forms.CharField(label="Nový email", widget=forms.EmailInput())
