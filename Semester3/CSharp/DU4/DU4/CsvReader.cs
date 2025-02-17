using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DU4
{
    internal class CsvReader
    {
        private string Path;

        public delegate void OnDataCallback(string[] columns, int index);
        private OnDataCallback Callback;

        public delegate void OnInvalidDataHandler(string line, int index);
        public event OnInvalidDataHandler OnInvalidData;

        public CsvReader(string path)
        {
            Path = path;
        }

        public void SetDataCallback(OnDataCallback callback)
        {
            this.Callback = callback;
        }

        public void Read()
        {
            using StreamReader sr = new(this.Path);

            string? data = sr.ReadLine();//skip first line

            if (data == null)
            {
                this.OnInvalidData?.Invoke("", 0);
                return;
            }

            int columns = data.Split(";").Length;
            int i = 0;
            while ((data = sr.ReadLine()) != null)
            {
                string[] rows = data.Split(";");

                if (rows.Length != columns)
                {
                    this.OnInvalidData?.Invoke(data, i + 1);
                    continue;
                }

                this.Callback(rows, rows.Length);
                ++i;
            }
        }
    }
}
