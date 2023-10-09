using System;
using System.Globalization;

namespace DU1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("cs-CZ");
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            string file = args[0];

            string csv = File.ReadAllText(file);

            Country[] countrys = ParseCSV(csv);

            Console.WriteLine($"Průměrný plat: {averageWageInCountrys(countrys)}");
            Console.WriteLine($"Nejvyšší plat v roce 2023: {getCountryWithHighestWage(countrys, 2023)?.name}");

            Console.WriteLine("");

            foreach (var country in countrys)
            {
                Console.WriteLine($"{country.name}: {double.Round(country.priceInEuro, 2)} EUR");
            }
        }

        static Country[] ParseCSV(string csv)
        {
            string[] lines = csv.Split('\n');

            Country[] countrys = new Country[lines.Length - 1];

            for (int i = 1; i < lines.Length; i++)
            {
                string line = lines[i];
                string[] cols = line.Split(';');

                string countryName = cols[0];

                AverageWage averageWage = parseAverageWage(cols[1]);

                double exchageRate = double.Parse(cols[2]);

                string[] yearSplited = cols[3].Split('-');
                int year = -1;
                if (yearSplited[0].Length > 0) {
                    year = int.Parse(yearSplited[0]);
                }

                bool inEU = cols[4].Equals("ano");

                Country c = new Country(countryName, averageWage, exchageRate, year, inEU);
                countrys[i - 1] = c;
            }

            return countrys;
        }

        static AverageWage parseAverageWage(string str)
        {
            AverageWage averageWage;

            string[] wageSplited = str.Split(' ');

            if (double.TryParse(wageSplited[0], out double wage))
            {
                averageWage.value = wage;
                averageWage.currency = wageSplited[1];
            }
            else
            {
                averageWage.value = double.Parse(wageSplited[1]);
                averageWage.currency = wageSplited[0];
            }

            return averageWage;
        }

        static double averageWageInCountrys(Country[] countrys)
        {
            double averageWage = 0;
            int count = 0;

            foreach (Country country in countrys)
            {
                if (!country.isInEU) continue;

                double inEuro = country.priceInEuro;
                averageWage += inEuro;
                count++;
            }
            
            averageWage /= count;

            return double.Round(averageWage, 2);
        }

        static Country? getCountryWithHighestWage(Country[] countrys, int year)
        {
            Country? country = null;

            foreach (Country c in countrys)
            {
                if (c.dataObtainYear != year && c.dataObtainYear != -1) continue;

                if (country == null)
                {
                    country = c;
                } else
                {
                    if (c.priceInEuro > country.priceInEuro)
                    {
                        country = c;
                    }
                }
            }

            return country;
        }
    }
}