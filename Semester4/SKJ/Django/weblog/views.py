from django.http import HttpResponse
from django.shortcuts import get_object_or_404, redirect, render

from .forms import CommentForm
from .models import Comment, Post

# Create your views here.


def index(request):
    posts = Post.objects.all()

    return render(request, "weblog/index.html", {"posts": posts})


def post(request, post_id):
    post = get_object_or_404(Post, pk=post_id)
    comments = Comment.objects.filter(post=post)

    comment_form = CommentForm()

    return render(
        request,
        "weblog/post.html",
        {"post": post, "comments": comments, "form": comment_form},
    )


def addcomment(request, post_id):
    post = get_object_or_404(Post, pk=post_id)

    if request.method == "POST":
        comment = CommentForm(request.POST)
        if comment.is_valid():
            comment_body = comment.cleaned_data["body"]
            comment = Comment(post=post, body=comment_body)
            comment.save()

            return redirect("post", post_id=post.pk)
