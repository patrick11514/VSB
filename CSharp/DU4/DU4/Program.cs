using DU4;
using static DU4.CsvReader;
using System.Formats.Asn1;
using System.Globalization;

namespace DU4v
{
    internal class Program
    {
        static void Main(string[] args)
        {
            CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
            CultureInfo.CurrentUICulture = CultureInfo.InvariantCulture;

            // Nejdříve jen vyzkoušíme jestli nám funguje CsvReader

            OnInvalidDataHandler errorHandler = (string row, int index) =>
            {
                Console.WriteLine("Chybný řádek:" + row);
            };

            OnDataCallback dataHandler = (string[] row, int index) =>
            {
                if (index == 0) { return; }
                Console.WriteLine(string.Join(" | ", row));
            };

            CsvReader treamInfoReader = new CsvReader("team_info.csv");
            treamInfoReader.SetDataCallback(dataHandler);
            treamInfoReader.OnInvalidData += errorHandler;
            treamInfoReader.Read();


            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine();


            // Sestavíme ligu, vypočteme statistiky a vypíšeme data...

            NationalHockeyLeague nhl = new NationalHockeyLeague();
            nhl.LoadData("team_info.csv", "match.csv");
            nhl.ComputeStatistics();

            nhl.SortTeams();


            Console.WriteLine("Žebříček týmů:");
            foreach (var team in nhl.Teams)
            {
                Console.WriteLine($"{team.ShortName} - {team.TeamName} | {team.WinCount} | {team.LossCount} | {team.TieCount}");
            }

            
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("Prvních 10 zápasů nejlepšího týmu:");
            foreach (var match in nhl.GetMatchesByTeam(nhl.Teams[0]).Take(10))
            {
                Console.WriteLine($"{match.HomeTeam.TeamName} vs {match.AwayTeam.TeamName} => {match.HomeGoals} : {match.AwayGoals}");
            }

            nhl.SaveTeamsToJson("data.json");
            nhl.SaveTeamsToXml("data.xml");


            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("Data z JSON:");

            foreach (var team in nhl.LoadTeamsFromJson("data.json"))
            {
                Console.WriteLine($"{team.ShortName} - {team.TeamName} | {team.WinCount} | {team.LossCount} | {team.TieCount}");
            }
            
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("Data z XML:");

            foreach (var team in nhl.LoadTeamsFromXml("data.xml"))
            {
                Console.WriteLine($"{team.ShortName} - {team.TeamName} | {team.WinCount} | {team.LossCount} | {team.TieCount}");
            }
        }
    }
}