from fieldelement import FieldElement

A = 0
B = 7
P = 2**256 - 2**32 - 977
N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

class S256Field(FieldElement):
    def __init__(self, num, prime=None):
        super().__init__(num=num, prime=P)

    def hex(self):
        return f"{self.num:x}".zfill(64)

    def __repr__(self):
        return self.hex()

    def sqrt(self):
        return self ** ((P + 1) // 4)