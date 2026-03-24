from unittest import TestCase
from random import randint
from signature import Signature

from s256point import S256Point

A = 0
B = 7
P = 2**256 - 2**32 - 977
N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

G = S256Point(
    0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
    0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8,
)

class PrivateKey:
    def __init__(self, secret):
        self.secret = secret
        self.point = secret * G

    def hex(self):
        return f"{self.secret:x}".zfill(64)

    def sign(self, z):
        # TODO 1: Sign
        k = randint(1, N - 1)
        r = (k * G).x.num
        k_inv = pow(k, -1, N)
        s = (z + r * self.secret) * k_inv % N
        return Signature(r, s)


class PrivateKeyTest(TestCase):
    def test_sign(self):
        pk = PrivateKey(randint(0, 2**256))
        z = randint(0, 2**256)
        sig = pk.sign(z)
        self.assertTrue(pk.point.verify(z, sig))