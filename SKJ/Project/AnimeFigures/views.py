from bcrypt import checkpw, gensalt, hashpw
from django.http import HttpRequest, HttpResponseBadRequest
from django.shortcuts import get_list_or_404, get_object_or_404, redirect, render

from AnimeFigures.forms import FigureForm, ImageForm, LoginForm, RegisterForm
from AnimeFigures.models import Figure, Image, User, UserLike

session_fields = ["user_id", "username", "email"]


# Create your views here.
def get_session_data(request: HttpRequest):
    dictionary = {}

    for field in session_fields:
        dictionary[field] = request.session.get(field)

    return dictionary


def remove_session_data(request: HttpRequest):
    for field in session_fields:
        if request.session.get(field) is not None:
            del request.session[field]


def get_figure_images(figures: list[Figure]):
    images: dict[int, str | None] = {}

    listDict = list(
        map(
            lambda figure: {
                "id": figure.pk,
                "url": Image.objects.filter(figure=figure).first(),
            },
            figures,
        )
    )

    for item in listDict:
        id = item["id"]
        item = item["url"]
        if item is not None:
            item = item.url
        images[id] = item

    return images


def index(request: HttpRequest):
    figures = Figure.objects.all()
    users = User.objects.all()

    # first image of each figure
    images = get_figure_images(figures)

    return render(
        request,
        "AnimeFigures/index.html",
        get_session_data(request)
        | {"users": users, "figures": figures, "images": images},
    )


def search(request: HttpRequest):
    return render(request, "AnimeFigures/search.html", get_session_data(request))


def register(request: HttpRequest):
    if request.session.get("user_id") is not None:
        return redirect("index")

    if request.method == "POST":
        register_form = RegisterForm(request.POST)
        if register_form.is_valid():
            existing_user = User.objects.filter(
                name=register_form.cleaned_data["name"]
            ).first()

            if existing_user is not None:
                register_form = RegisterForm()
                return render(
                    request,
                    "AnimeFigures/register.html",
                    {
                        "register_form": register_form,
                        "error": "Uživatelské jméno již existuje.",
                    },
                )

            user = User()
            user.name = register_form.cleaned_data["name"]
            user.email = register_form.cleaned_data["email"]
            user.password = hashpw(
                register_form.cleaned_data["password"].encode(), gensalt()
            ).decode()

            user.save()

            return redirect("login")

    register_form = RegisterForm()

    return render(
        request, "AnimeFigures/register.html", {"register_form": register_form}
    )


def login(request: HttpRequest):
    if request.session.get("user_id") is not None:
        return redirect("index")

    if request.method == "POST":
        # handle post login
        login_form = LoginForm(request.POST)

        if login_form.is_valid():
            user = User.objects.filter(name=login_form.cleaned_data["name"]).first()
            if user is None:
                login_form = LoginForm()
                return render(
                    request,
                    "AnimeFigures/login.html",
                    {"login_form": login_form, "error": "Špatné uživatelské jméno."},
                )

            if not checkpw(
                login_form.cleaned_data["password"].encode(), user.password.encode()
            ):
                return render(
                    request,
                    "AnimeFigures/login.html",
                    {"login_form": login_form, "error": "Špatné heslo."},
                )

            request.session["user_id"] = user.pk
            request.session["username"] = user.name
            request.session["email"] = user.email

            return redirect("index")

    login_form = LoginForm()

    return render(request, "AnimeFigures/login.html", {"login_form": login_form})


def manufacturers(request: HttpRequest):
    return render(
        request,
        "AnimeFigures/manufacturers.html",
        get_session_data(request),
    )


def user(request: HttpRequest, user_id: int):
    user = get_object_or_404(User, pk=user_id)

    liked_figures = UserLike.objects.filter(user=user)

    figures = list(map(lambda like: like.figure, liked_figures))
    images = get_figure_images(figures)

    return render(
        request,
        "AnimeFigures/user.html",
        get_session_data(request)
        | {
            "user": user,
            "liked_figures": liked_figures,
            "images": images,
        },
    )


def logout(request: HttpRequest):
    if request.session.get("user_id") is not None:
        remove_session_data(request)

    return redirect("index")


def figure(request: HttpRequest, figure_id: int):
    figure = get_object_or_404(Figure, pk=figure_id)

    pictures = Image.objects.filter(figure=figure)

    can_like = request.session.get("user_id") is not None
    liked = None

    if can_like:
        liked = UserLike.objects.filter(
            user=request.session.get("user_id"), figure=figure
        ).first()

    return render(
        request,
        "AnimeFigures/figure.html",
        get_session_data(request)
        | {
            "figure": figure,
            "pictures": pictures,
            "can_like": can_like,
            "liked": liked,
        },
    )


def add_figure(request: HttpRequest):
    if request.session.get("user_id") is None:
        return redirect("login")

    if request.method == "POST":
        figure_form = FigureForm(request.POST)

        if figure_form.is_valid():
            figure: Figure = figure_form.save()

            return redirect("figure", figure.pk)

    figure_form = FigureForm()

    return render(
        request,
        "AnimeFigures/add_figure.html",
        get_session_data(request)
        | {
            "figure_form": figure_form,
        },
    )


def add_image(request: HttpRequest, figure_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    figure = get_object_or_404(Figure, pk=figure_id)

    if request.method == "POST":
        image_form = ImageForm(request.POST)

        if image_form.is_valid():
            image = Image()
            image.url = image_form.cleaned_data["url"]
            image.figure = figure

            image.save()

            return redirect("figure", figure_id)

    image_form = ImageForm()

    return render(
        request,
        "AnimeFigures/add_image.html",
        get_session_data(request)
        | {
            "image_form": image_form,
            "figure": figure,
        },
    )


def like(request: HttpRequest, figure_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    figure = get_object_or_404(Figure, pk=figure_id)

    like = UserLike.objects.filter(
        figure=figure.pk, user=request.session.get("user_id")
    ).first()

    if like is None:
        like = UserLike()
        like.figure = figure
        like.user = User.objects.get(pk=request.session.get("user_id"))
        like.save()

        return redirect("figure", figure_id)

    like.delete()

    return redirect("figure", figure_id)
