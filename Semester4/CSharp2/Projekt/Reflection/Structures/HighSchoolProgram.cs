using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataLayer.Structures
{
	public class HighSchoolProgram
	{
		[PrimaryKey(false)]
		public string Identifier { get; set; }
		public int HighSchool { get; set; }
		public string Name { get; set; }
		public string Description { get; set; }
		public int OpenedPlaces { get; set; }
	}
}
