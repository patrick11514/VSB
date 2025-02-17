using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    public class Customer : INotifyPropertyChanged
    {
        public int Id { get; set; }
        private string name;
        public string Name
        {
            get { return name; }
            set
            {
                SetValue(ref name, value);
            }
        }
        public string surname;
        public string Surname
        {
            get { return surname; }
            set
            {
                SetValue(ref surname, value);
            }
        }
        public int Age { get; set; }

        public event PropertyChangedEventHandler? PropertyChanged;

        private void SetValue<T>(ref T variable, T value, [CallerMemberName] string propertyName = null)
        {
            variable = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
