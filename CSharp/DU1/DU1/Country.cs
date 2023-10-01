using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU1
{
    public struct AverageWage
    {
        public double value;
        public string currency;
    }

    internal class Country
    {
        public string name { get; }
        public AverageWage averageWage { get; }
        public double exchageRate { get; }
        public int dataObtainYear { get; }
        public bool isInEU { get; }
        public double priceInEuro { get {
                return averageWage.value * exchageRate;
        } }

        public Country(string name, AverageWage averageWage, double exchageRate, int dataObtainYear, bool isInEU)
        {
            this.name = name;
            this.averageWage = averageWage;
            this.exchageRate = exchageRate;
            this.dataObtainYear = dataObtainYear;
            this.isInEU = isInEU;
        }
    }
}
