using System.Globalization;
using System.Text.Json;

namespace Test2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
            CultureInfo.CurrentUICulture = CultureInfo.InvariantCulture;

            ElectionCollection collection = new ElectionCollection();
            collection.Load("election.bin");

            Console.WriteLine("Počty hlasů");
            Console.WriteLine("-------------");

            collection.PrintPartyVotes();

            Console.WriteLine();

            collection.Sort();

            Console.WriteLine("Kandidáti");
            Console.WriteLine("-------------");

            foreach (Candidate candidate in collection.Filter(true /* jen výherní strany */, 45 /* minimální věk */, 3000 /* minimální počet hlasů */))
            {
                Console.WriteLine($"{candidate.Name}, {candidate.Age}: {candidate.VoteCount} | {candidate.Party}");
            }

            collection.Save("data.json");
        }
    }
}