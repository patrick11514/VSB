using System.Text;

namespace Database
{
    public class Person
    {
        public string Name { get; set; }
        public GenderEnum Gender { get; set; }
        private int? _Age { get; set; }
        public int? Age { get {
                return _Age;
            }
            set {
                if (value >= 0 && value <= 150) {
                    _Age = value;

                }
            }
        } 

        public bool IsAdult { get
            {
                return Age >= 18;
            } 
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(Name);
            sb.Append(" (");
            sb.Append(Gender.Equals(GenderEnum.MALE) ? "Male" : "Female");
            sb.Append("): ");
            sb.Append(_Age);

            return sb.ToString();

        }
    } 
}