using Database;

namespace C04
{
    internal class Program
    {
        static void Main(string[] args)
        {
            PopulationDatabase database = new();

            while(true)
            {
                Console.WriteLine("Add - pro přidání osoby");
                Console.WriteLine("Print - pro zobrazení databáze");
                Console.WriteLine("Exit - pro ukončení");
                string input = Console.ReadLine();
                if (input.ToLower().Equals("exit")) break;
                if (input.ToLower().Equals("add"))
                {
                    Console.WriteLine("Zadej jméno: ");
                    string? name = Console.ReadLine();
                    if (name == null)
                    {
                        Console.WriteLine("Chyba!");
                        continue;
                    }
                    Console.WriteLine("Zadej věk v rozsahu 0-150");
                    string? ageStr = Console.ReadLine();

                    int.TryParse(ageStr, out int age);

                    Console.WriteLine("Zadej pohlaví: Male, nebo Female");
                    string genderStr = Console.ReadLine();
                    GenderEnum gender;
                    if (genderStr.ToLower().Equals("male"))
                    {
                        gender = GenderEnum.MALE;
                    } else if (genderStr.ToLower().Equals("female"))
                    {
                        gender = GenderEnum.FEMALE;
                    } else
                    {
                        Console.WriteLine("Chyba!");
                        continue;
                    }

                    database.Add(new Person() { Name = name, Age = age, Gender = gender });
                } else if (input.ToLower().Equals("print"))
                {
                    Console.WriteLine(database.ToString());
                } else
                {
                    Console.WriteLine("Neplatná hodnota");
                }

            }
        }
    }
}