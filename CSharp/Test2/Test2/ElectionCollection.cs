using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Test2
{
    internal class ElectionCollection
    {
        private List<Candidate> Candidates = new();
        private List<String> Won = new();

        public void Load(string path)
        {
            using FileStream fs = new(path, FileMode.Open);
            using BinaryReader br = new(fs);

            int Candidates = br.ReadInt32();
            Console.WriteLine(Candidates);
            for (int i = 0; i < Candidates; i++)
            {
                string party = br.ReadString();
                byte number = br.ReadByte();
                string name = br.ReadString();
                byte age = br.ReadByte();
                string prislusnost = br.ReadString();
                int votes = br.ReadInt32();

                this.Candidates.Add(new Candidate()
                {
                    Number = number,
                    Name = name,
                    Age = age,
                    Party = party,
                    VoteCount = votes,
                    Prislusnost = prislusnost
                });
            }

            int WonCount = br.ReadInt32();
            for (int i = 0; i < WonCount; i++)
            {
                this.Won.Add(br.ReadString());
            }
        }

        public void PrintPartyVotes()
        {
            Dictionary<string, int> dict = new();

            foreach (Candidate candidate in this.Candidates)
            {
                if (!dict.ContainsKey(candidate.Party))
                {
                    dict[candidate.Party] = 0;
                }

                dict[candidate.Party] += candidate.VoteCount;
            }

            var sorted = dict.ToList();
            sorted.Sort((a, b) =>
            {
                return a.Key.CompareTo(b.Key);
            });

            foreach (KeyValuePair<string, int> data in sorted)
            {
                Console.WriteLine($"{data.Key}: {data.Value}");
            }
        }

        public void Sort()
        {
            this.Candidates.Sort(new CandidateComparer());
        }

        public IEnumerable<Candidate> Filter(bool won, int minAge, int minVoteCount)
        {
            foreach (Candidate candidate in this.Candidates)
            {
                string? party = this.Won.Find((party) => party == candidate.Party);
                if (won && party == null) continue;
                if (candidate.Age < minAge) continue;
                if (candidate.VoteCount < minVoteCount) continue;

                yield return candidate;
            }
        }
        public void Save(string fileName)
        {
            using FileStream fs = new(fileName, FileMode.Create);
            JsonSerializer.Serialize(fs, this.Candidates, typeof(List<Candidate>), new JsonSerializerOptions()
            {
                WriteIndented = true,
                Encoder = System.Text.Encodings.Web.JavaScriptEncoder.UnsafeRelaxedJsonEscaping
            });
        }
    }
}
