import pytest

from tasks import spocti_krychli, OvladaciPanel, nejcastejsi_glyfy

def test_spocti_krychli():
    assert spocti_krychli("souradnice.txt") == 255013440

def test_ovladaci_panel():
    panel = OvladaciPanel(["A", "B", "C"])
    panel.posun_doprava()
    panel.zadej_znak()
    panel.posun_doprava()
    panel.posun_doprava()
    panel.zadej_znak()
    panel.posun_doleva()
    panel.zadej_znak()
    assert panel.vytoc_adresu() == ["B", "A", "C"]

    panel = OvladaciPanel(["Taurus", "Virgo", "Crater", "Libra", "Centaurus"])
    panel.zadej_znak()
    for i in range(6):
        panel.posun_doleva()
    panel.zadej_znak()
    for i in range(7):
        panel.posun_doprava()
    panel.zadej_znak()
    assert panel.vytoc_adresu() == ["Taurus", "Centaurus", "Virgo"]



def test_nejcastejsi_glyfy():
    assert nejcastejsi_glyfy("planety.txt") == [
        ('Sagittarius', 51), ('Hydra', 39), ('Libra', 38),
        ('Perseus', 38), ('Aries', 37), ('Capricornus', 37),
        ('Virgo', 37), ('Auriga', 36), ('Crater', 36),
        ('Eridanus', 36), ('Lynx', 36), ('Scorpius', 36),
        ('Norma', 34), ('Orion', 32), ('Scutum', 31),
        ('Sextans', 31), ('Centaurus', 30), ('Gemini', 30),
        ('Bootes', 29), ('Taurus', 26)
    ]

