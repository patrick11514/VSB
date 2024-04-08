from django.http import HttpResponse
from django.shortcuts import render

from .models import Comment, Post

# Create your views here.


def index(request):
    posts = Post.objects.all()

    return render(request, "weblog/index.html", {"posts": posts})
