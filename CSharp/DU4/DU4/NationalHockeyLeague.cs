using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace DU4
{
    internal class NationalHockeyLeague
    {
        public List<Match> Matches { get; set; }
        public List<Team> Teams { get; set; }

        public NationalHockeyLeague()
        {
            Matches = new();
            Teams = new();
        }

        public void LoadData(string teamsCsvPath, string matchCsvPath)
        {
            //teams
            CsvReader reader = new(teamsCsvPath);
            reader.SetDataCallback((cols, _) =>
            {
                this.Teams.Add(new Team()
                {
                    Id = int.Parse(cols[0]),
                    ShortName = cols[1],
                    TeamName = cols[2],
                });
            });
            reader.Read();

            //matches
            reader = new (matchCsvPath);
            reader.SetDataCallback((cols, _) =>
            {
                this.Matches.Add(new Match()
                {
                    Season = cols[0],
                    Date = DateTime.Parse(cols[1]),
                    AwayTeam = this.Teams.Find((team) => team.Id == int.Parse(cols[2])),
                    HomeTeam = this.Teams.Find((team) => team.Id == int.Parse(cols[3])),
                    AwayGoals = int.Parse(cols[4]),
                    HomeGoals = int.Parse(cols[5]),
                 });
            });
            reader.Read();
        }

        public void ComputeStatistics()
        {
            this.Teams.ForEach((team) =>
            {
                //musim modifikovat mimo
                foreach (Match match in this.Matches)
                { 

                    int? diff = null;
                    if (match.HomeTeam != null && match.HomeTeam.Id == team.Id)
                    {
                        diff = match.HomeGoals - match.AwayGoals;
                    } else if (match.AwayTeam != null && match.AwayTeam.Id == team.Id)
                    {
                        diff = match.AwayGoals - match.HomeGoals;

                    }

                    if (diff.HasValue)
                    {
                        if (diff > 0) team.WinCount++;
                        else if (diff == 0) team.TieCount++;
                        else team.LossCount++;
                    }
                }
            });
        }

        public void SortTeams()
        {
            StatsComparer sc = new();
            this.Teams.Sort(sc.Compare);
        }

        public IEnumerable<Match> GetMatchesByTeam(Team team)
        {
            foreach (Match match in this.Matches)
            {
                if (match.HomeTeam != null && match.HomeTeam.Id == team.Id)
                {
                    yield return match;
                } else if( match.HomeTeam != null && match.AwayTeam.Id == team.Id)
                {
                    yield return match;
                }
            }
        }

       public void SaveTeamsToJson(string path)
        {
            using FileStream fs = new(path, FileMode.Create);
            JsonSerializer.Serialize(fs, this.Teams, new JsonSerializerOptions()
            {
                WriteIndented = true
            }) ;
        }

        public List<Team> LoadTeamsFromJson(string path)
        {
            using FileStream fs = new(path, FileMode.Open);
            return JsonSerializer.Deserialize(fs, typeof(List<Team>), new JsonSerializerOptions()) as List<Team>;
        }

        public void SaveTeamsToXml(string path)
        {
            using FileStream fs = new(path, FileMode.Create);
            XmlSerializer serializer = new(typeof(List<Team>));
            serializer.Serialize(fs, this.Teams);
        }

        public List<Team> LoadTeamsFromXml(string path)
        {
            using FileStream fs = new(path, FileMode.Open);
            XmlSerializer serializer = new XmlSerializer(typeof(List<Team>));
            return serializer.Deserialize(fs) as List<Team>;
        }
    }
}
