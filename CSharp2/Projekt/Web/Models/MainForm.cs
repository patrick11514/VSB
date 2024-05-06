using Microsoft.AspNetCore.Mvc;

namespace Web.Models
{
    public class MainForm
    {
        //basic data
        public string name { get; set; }
        public string surname { get; set; }
        public string state { get; set; }
        public string region { get; set; }
        public string town { get; set; }
        public string street { get; set; }
        public int housenumber { get; set; }
        public int postalcode { get; set; }
        public string phone { get; set; }
        public string email { get; set; }
        //programs
        public int hs1 { get; set; }
        public string program1 { get; set; }

        public int? hs2 { get; set; }
        public string? program2 { get; set; }

        public int? hs3 { get; set; }
        public string? program3 { get; set; }

    }
}
