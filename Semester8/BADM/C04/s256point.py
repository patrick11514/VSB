from s256field import S256Field
from point import Point

from helper import encode_base58_checksum, hash160

A = 0
B = 7
P = 2**256 - 2**32 - 977
N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

class S256Point(Point):
    def __init__(self, x, y, a=None, b=None):
        a, b = S256Field(A), S256Field(B)
        if type(x) == int:
            super().__init__(x=S256Field(x), y=S256Field(y), a=a, b=b)
        else:
            super().__init__(x=x, y=y, a=a, b=b)

    def __repr__(self):
        if self.x is None:
            return "S256Point(infinity)"
        else:
            return f"S256Point({self.x.num:x},{self.y.num:x})"

    def __rmul__(self, coefficient):
        coef = coefficient % N
        return super().__rmul__(coef)

    def sec(self, compressed=True):
    #TODO 5: SEC Uncompressed and Compressed SEC Format
        if compressed:
            if self.y.num % 2 == 0:
                return b'\x02' + self.x.num.to_bytes(32, 'big')
            else:
                return b'\x03' + self.x.num.to_bytes(32, 'big')
        else:
            return b'\x04' + self.x.num.to_bytes(32, 'big') + self.y.num.to_bytes(32, 'big')

    def address(self, compressed=True, network="mainnet"):
    # TODO 4: Address
        h160 = hash160(self.sec(compressed))
        if network in ("testnet", "signet"):
            prefix = b'\x6f'
        else:
            prefix = b'\x00'
        return encode_base58_checksum(prefix + h160)

    def verify(self, z, sig):
        s_inv = pow(sig.s, -1, N)
        u = z * s_inv % N
        v = sig.r * s_inv % N
        total = u * G + v * self

        return total.x.num == sig.r

    @classmethod
    def parse(self, sec_bin):
        """returns a Point object from a compressed sec binary (not hex)"""
        if sec_bin[0] == 4:
            x = int(sec_bin[1:33].hex(), 16)
            y = int(sec_bin[33:65].hex(), 16)
            return S256Point(x=x, y=y)
        
        is_even = sec_bin[0] == 2
        x = S256Field(int(sec_bin[1:].hex(), 16))
        alpha = x**3 + S256Field(B)
        beta = alpha.sqrt()
        
        if beta.num % 2 == 0:
            even_beta = beta
            odd_beta = S256Field(P - beta.num)
        else:
            even_beta = S256Field(P - beta.num)
            odd_beta = beta
        if is_even:
            return S256Point(x, even_beta)
        else:
            return S256Point(x, odd_beta)
        
G = S256Point(
    0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
    0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8,
)