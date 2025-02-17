namespace C08.Models
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    public class Product
    {

        public int Id { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public double Price { get; set; }
        public string Photo { get; set; }



        public static List<Product> GetProducts()
        {
            return new List<Product>() {
            new Product() {
                Id = 1,
                Name = "77\" LG OLED77B2",
                Description = "Televize SMART OLED, 195cm, 4K Ultra HD, 100Hz, HDR10, HLG, Dolby Vision, DVB-T2/S2/C, H.265/HEVC, 4× HDMI, 2× USB, CI+, USB nahrávání, LAN, WiFi, Bluetooth, DLNA, Miracast, HbbTV 2.0, herní režim, hlasové ovládání, Netflix, Disney+, HBO Max, Apple TV, Voyo, O2 TV, Google Assistent, Přehrávání 360° videa, Amazon Alexa, Apple Airplay 2, párování s mobilním zařízením, WebOS, VESA 400×200, repro 20W, Dolby Atmos, Ultra Surround, en. třída F",
                Price = 49_990,
                Photo = "https://cdn.alza.cz/Foto/f11/WK/WK612g09b1.jpg"
            },
            new Product() {
                Id = 2,
                Name = "Garmin Fenix 7X Sapphire Solar Mineral Blue DLC Titanium/Whitestone",
                Description = "Chytré hodinky - pro muže i ženy, s ovládáním v češtině, transflektivní MIP displej, GPS, NFC platby skrze Garmin Pay, měření tepu, monitoring spánku, krokoměr, oxymetr, barometr, hledání telefonu, přehrávač hudby v hodinkách, ovládání přehrávače hudby v mobilu, předpověď počasí, režim spánku/nerušit, obchod s aplikacemi, navigace, always On Display a integrované mapy, vhodné na běh, cyklistiku, jógu, golf, plavání, vodotěsnost 100 m (10 ATM), max. výdrž baterie 888 h, detekce pádu, materiál pouzdra plast",
                Price = 21_490,
                Photo = "https://cdn.alza.cz/Foto/f11/PP/PPfenix7s15.jpg"
            },
            new Product() {
                Id = 3,
                Name = "ETA Falco Smart 2515 90000",
                Description = "Robotický vysavač 2v1 se smart aplikací na vytírání podlah, na koberce, na sbírání nečistot, na všechny druhy podlah, tvrdé podlahy, hladké podlahy i měkké povrchy, doba provozu 120 min, doba nabíjení 5 h, automatické dobíjení, předvolba místností k úklidu, senzor proti pádu ze schodů, senzor proti nárazu, vysátí více místností a přizpůsobení povrchu podlahy, objem nádoby 0,6 l, počet bočních štětců 2, překoná překážku do 1,5 cm, způsob navigace gyro, DC motor, typ ovládání dálkový ovladač, způsob čištění: kartáče,programy a mopování, typ baterie Li-ion, HEPA filtr, hlučnost 72 dB(A), mobilní aplikace, boční kartáč, dálkové ovládání, mopovací modul s mopovací tkaninou, mopovací textílie (2ks), nabíjecí adaptér, nabíjecí základna a virtuální zeď, barva stříbrná",
                Price = 7_999,
                Photo = "https://cdn.alza.cz/Foto/f11/ET/ETARV410.jpg"
            },
            new Product() {
                Id = 4,
                Name = "Sava 29 Carbon 6.2 vel 19\"/L",
                Description = "Horské kolo 29\" - karbonový rám, 12 rychlostí, vzduchová vidlice ROCKSHOX JUDY silver TK 29 tapered (zdvih 100 mm), brzdy SHIMNAO SLX, BL-M7100, hmotnost 11,9 kg",
                Price = 44_790,
                Photo = "https://cdn.alza.cz/Foto/f11/SP/SPTsav133nad.jpg"
            },

        };
        }
    }



}
