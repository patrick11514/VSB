using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C11
{
    internal class Translator
    {
        private Dictionary<string, string> dict = new(StringComparer.OrdinalIgnoreCase);

        public void Add(string source, string target)
        {
            this.dict[source] = target;
        }

        public string Translate(string txt)
        {
            List<string> translated = new();

            foreach (string word in txt.Split(" "))
            {
               translated.Add(dict.ContainsKey(word) ? this.dict[word] : "???"); 
            }

            return string.Join(" ", translated);
        }
    }
}
