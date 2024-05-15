from typing import TypeVar

from django import template

register = template.Library()

T = TypeVar("T")
I = TypeVar("I")


@register.filter
def hash(h: dict[T, I], key: T) -> I:
    return h.get(key)


@register.filter
def hash_count(h: dict[T, I], key: T) -> int:
    return len(hash(h, key))
