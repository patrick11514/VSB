using System.Net.NetworkInformation;
using System.Text;

namespace C09
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<Contact> contacts = new();
            contacts.Add(new Contact()
            {
                Name = "Pepa Jánský",
                Age = 20,
                Email = "pepa.jansky@janskeho.cz",
                IsAlive = true,
                Weight = 42
            });

            contacts.Add(new Contact()
            {
                Name = "Babi Lehova",
                Age = 120,
                Email = "leha23@centrum.cz",
                IsAlive = false,
                Weight = 55
            });

            SaveToCSV(contacts);
            ReadFromCSV();
            WriteToBin(contacts);
            ReadFromBin();

            using MemoryStream ms = new();
            WriteToMemory(contacts, ms);
            ms.Seek(0, SeekOrigin.Begin);
            ReadFromMemory(ms);
        }

        private static void SaveToCSV(List<Contact> contacts)
        {
            /*
            using FileStream fs = new("data.csv", FileMode.OpenOrCreate);
            
            fs.Write(new byte[] { 60, 65, 70 });
             */
            using (FileStream fs = new("data.csv", FileMode.OpenOrCreate)) {
                /*string text = "Pokus";
                byte[] bytes = Encoding.UTF8.GetBytes(text);
                fs.Write(bytes);*/

                using StreamWriter sw = new(fs);
                foreach (Contact contact in contacts)
                {
                    sw.Write(contact.Name);
                    sw.Write(";");
                    sw.Write(contact.Age);
                    sw.Write(";");
                    sw.Write(contact.Email);
                    sw.Write(";");
                    sw.Write(contact.Weight);
                    sw.Write(";");
                    sw.Write(contact.IsAlive);

                    sw.WriteLine();
                }
            }    
        }

        private static void ReadFromCSV()
        {
            using StreamReader sr = new("data.csv");
            while (true)
            {
                string line = sr.ReadLine();
                if (line == null) break;
                try
                {
                    Contact c = ReadLine(line);
                    Console.WriteLine($"{c.Name}: {c.Age}");
                } catch (InvalidAgeException e)
                {
                    Console.WriteLine($"Chybný věk {e.Contact.Age}.");
                }

            }
        }

        private static Contact ReadLine(string line)
        {
            string[] parts = line.Split(';');
            Contact c = new Contact()
            {
                Name = parts[0],
                Age = string.IsNullOrEmpty(parts[1]) ? null : int.Parse(parts[1]),
                Email = parts[2],
                Weight = double.Parse(parts[3]),
                IsAlive = bool.Parse(parts[4])
            };
                
            if (c.Age < 0)
            {
                throw new InvalidAgeException(c);
            }

            return c;
        }

        private static void WriteToBin(List<Contact> contacts)
        {
            using FileStream fs = new("data.bin", FileMode.OpenOrCreate);
            using BinaryWriter bw = new(fs);

            bw.Write(contacts.Count);
            foreach (Contact contact in contacts)
            {
                bw.Write(contact.Name);
                bw.Write(contact.Age.HasValue);
                if (contact.Age.HasValue)
                {
                    bw.Write(contact.Age.Value);
                }
                bw.Write(contact.Email);
                bw.Write(contact.Weight);
                bw.Write(contact.IsAlive);
            }
        }

        private static void ReadFromBin()
        {
            using FileStream fs = new("data.bin", FileMode.OpenOrCreate);
            using BinaryReader br = new(fs);
            int count = br.ReadInt32();

            for (int i = 0; i < count; ++i)
            {
                Contact c = new()
                {
                    Name = br.ReadString(),
                    Age = br.ReadBoolean() ? br.ReadInt32() : null,
                    Email = br.ReadString(),
                    Weight = br.ReadDouble(),
                    IsAlive = br.ReadBoolean()
                };

                Console.WriteLine($"{c.Name}: {c.Age}");
            }
        }

        private static void WriteToMemory(List<Contact> contacts, MemoryStream ms)
        {
            using BinaryWriter bw = new(ms, Encoding.UTF8, true);

            bw.Write(contacts.Count);
            foreach (Contact contact in contacts)
            {
                bw.Write(contact.Name);
                bw.Write(contact.Age.HasValue);
                if (contact.Age.HasValue)
                {
                    bw.Write(contact.Age.Value);
                }
                bw.Write(contact.Email);
                bw.Write(contact.Weight);
                bw.Write(contact.IsAlive);
            }
        }

        private static void ReadFromMemory(MemoryStream ms)
        {
            using BinaryReader br = new(ms);
            int count = br.ReadInt32();

            for (int i = 0; i < count; ++i)
            {
                Contact c = new()
                {
                    Name = br.ReadString(),
                    Age = br.ReadBoolean() ? br.ReadInt32() : null,
                    Email = br.ReadString(),
                    Weight = br.ReadDouble(),
                    IsAlive = br.ReadBoolean()
                };

                Console.WriteLine($"{c.Name}: {c.Age}");
            }
        }
    }
}