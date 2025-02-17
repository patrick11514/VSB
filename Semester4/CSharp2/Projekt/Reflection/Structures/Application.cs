using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataLayer.Structures
{
    public class Application
    {
        [PrimaryKey]
        public int Id { get; set; }
        public int Student { get; set; }
        public DateTime Date { get; set; }
        public string StudyProgram1 { get; set; }
        [Nullable]
        public string? StudyProgram2 { get; set; }
        [Nullable]
        public string? StudyProgram3 { get; set; }
    }
}
