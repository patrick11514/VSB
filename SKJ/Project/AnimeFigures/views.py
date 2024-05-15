import json
from multiprocessing.managers import BaseManager

from bcrypt import checkpw, gensalt, hashpw
from django.http import HttpRequest, HttpResponse, HttpResponseBadRequest
from django.shortcuts import get_list_or_404, get_object_or_404, redirect, render

from AnimeFigures.forms import (
    FigureCommentForm,
    FigureForm,
    ImageForm,
    LoginForm,
    ManufacturerForm,
    RegisterForm,
    UserCommentForm,
)
from AnimeFigures.models import (
    Figure,
    FigureComment,
    Image,
    Manufacturer,
    ProfileComment,
    User,
    UserLike,
)

session_fields = ["user_id", "username", "email"]


######################### FUNCTIONS
def get_session_data(request: HttpRequest):
    dictionary = {}

    for field in session_fields:
        dictionary[field] = request.session.get(field)

    return dictionary


def remove_session_data(request: HttpRequest):
    for field in session_fields:
        if request.session.get(field) is not None:
            del request.session[field]


########################## INDEX


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


########################## FIGURES


def add_figure(request: HttpRequest):
    if request.session.get("user_id") is None:
        return redirect("login")

    if request.method == "POST":
        figure_form = FigureForm(request.POST)

        if figure_form.is_valid():
            figure: Figure = figure_form.save(commit=False)
            figure.added_by = User.objects.get(pk=request.session.get("user_id"))
            figure.save()

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


def delete_figure(request: HttpRequest, figure_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    figure = get_object_or_404(Figure, pk=figure_id)

    figure.delete()

    return redirect("index")


def figure(request: HttpRequest, figure_id: int):
    sortType = request.GET.get("sort")

    figure = get_object_or_404(Figure, pk=figure_id)

    comments: BaseManager[FigureComment]
    if (sortType is None) or (sortType == "desc"):
        sortType = "desc"
        comments = FigureComment.objects.filter(figure=figure).order_by("-date")
    else:
        sortType = "asc"
        comments = FigureComment.objects.filter(figure=figure).order_by("date")

    pictures = Image.objects.filter(figure=figure)

    comment_form = FigureCommentForm()

    logged = request.session.get("user_id") is not None
    liked = None

    if logged:
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
            "logged": logged,
            "liked": liked,
            "comments": comments,
            "sortType": sortType,
            "comment_form": comment_form,
        },
    )


########################## IMAGES
def get_figure_image(figure: Figure):
    image = Image.objects.filter(figure=figure).first()
    if image is not None:
        return image.url
    return None


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


def delete_image(request: HttpRequest, figure_id: int, image_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    figure = get_object_or_404(Figure, pk=figure_id)
    image = get_object_or_404(Image, pk=image_id)

    image.delete()

    return redirect("figure", figure.pk)


def figure_add_comment(request: HttpRequest, figure_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    if request.method == "POST":
        comment_form = FigureCommentForm(request.POST)
        if comment_form.is_valid():
            comment = FigureComment()
            comment.user = User.objects.get(pk=request.session.get("user_id"))
            comment.figure = Figure.objects.get(pk=figure_id)
            comment.comment = comment_form.cleaned_data["comment"]
            comment.save()

            return redirect("figure", figure_id)


########################## AUTH


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


def logout(request: HttpRequest):
    if request.session.get("user_id") is not None:
        remove_session_data(request)

    return redirect("index")


########################## MANUFACTURER


def manufacturers(request: HttpRequest):
    manufacturers = Manufacturer.objects.all()

    return render(
        request,
        "AnimeFigures/manufacturers.html",
        get_session_data(request)
        | {
            "manufacturers": manufacturers,
        },
    )


def add_manufacturer(request: HttpRequest):
    if request.session.get("user_id") is None:
        return redirect("login")

    if request.method == "POST":
        manufacturer_form = ManufacturerForm(request.POST)

        if manufacturer_form.is_valid():
            manufacturer_form.save()
            return redirect("manufacturers")

    manufacturer_form = ManufacturerForm()

    return render(
        request,
        "AnimeFigures/add_manufacturer.html",
        get_session_data(request) | {"manufacturer_form": manufacturer_form},
    )


def remove_manufacturer(request: HttpRequest, manufacturer_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    manufacturer = get_object_or_404(Manufacturer, pk=manufacturer_id)

    manufacturer.delete()

    return redirect("manufacturers")


def manufacturer(request: HttpRequest, manufacturer_id: int):
    manufacturer = get_object_or_404(Manufacturer, pk=manufacturer_id)

    logged = request.session.get("user_id") is not None

    figures = Figure.objects.filter(manufacturer=manufacturer)
    images = get_figure_images(figures)

    return render(
        request,
        "AnimeFigures/manufacturer.html",
        get_session_data(request)
        | {
            "manufacturer": manufacturer,
            "figures": figures,
            "images": images,
            "logged": logged,
        },
    )


########################## USER


def user(request: HttpRequest, user_id: int):
    sortType = request.GET.get("sort")

    user = get_object_or_404(User, pk=user_id)

    comments: BaseManager[ProfileComment]
    if (sortType is None) or (sortType == "desc"):
        sortType = "desc"
        comments = ProfileComment.objects.filter(profile=user).order_by("-date")
    else:
        sortType = "asc"
        comments = ProfileComment.objects.filter(profile=user).order_by("date")

    liked_figures = UserLike.objects.filter(user=user)

    figures = list(map(lambda like: like.figure, liked_figures))
    images = get_figure_images(figures)
    logged = request.session.get("user_id") is not None

    comment_form = UserCommentForm()

    return render(
        request,
        "AnimeFigures/user.html",
        get_session_data(request)
        | {
            "user": user,
            "liked_figures": liked_figures,
            "images": images,
            "comments": comments,
            "comment_form": comment_form,
            "logged": logged,
            "sortType": sortType,
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


def user_add_comment(request: HttpRequest, user_id: int):
    if request.session.get("user_id") is None:
        return redirect("login")

    if request.method == "POST":
        comment_form = UserCommentForm(request.POST)
        if comment_form.is_valid():
            comment = ProfileComment()
            comment.user = User.objects.get(pk=request.session.get("user_id"))
            comment.profile = User.objects.get(pk=user_id)
            comment.comment = comment_form.cleaned_data["comment"]
            comment.save()

            return redirect("user", user_id)


########################## SEARCH


def search(request: HttpRequest):
    if request.method == "POST":
        searchText = request.POST.get("text")

        # search users
        users = User.objects.filter(name__icontains=searchText).all()

        # search figures
        figures = Figure.objects.filter(name__icontains=searchText).all()

        # return as json
        response = HttpResponse()
        response.headers["Content-Type"] = "application/json"

        usersData = map(
            lambda user: {
                "id": user.pk,
                "name": user.name,
                "date": user.getDate(),
                "likes": UserLike.objects.filter(user=user).count(),
            },
            list(users),
        )

        figuresData = map(
            lambda figure: {
                "id": figure.pk,
                "name": figure.name,
                "manufacturer": figure.manufacturer.name,
                "price": figure.price,
                "image": get_figure_image(figure),
            },
            list(figures),
        )

        response.content = json.dumps(
            {"users": list(usersData), "figures": list(figuresData)}
        )
        return response

    return render(request, "AnimeFigures/search.html", get_session_data(request))
