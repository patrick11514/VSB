using System.Globalization;
using System.Text;
using System.Xml.Linq;

namespace DU2
{
    internal enum EventType
    {
        None = 0,
        Lecture,
        Tutorial,
        Consultation
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.GetCultureInfo("cs_CZ");

            Schedule schedule = ParseInput(args[0]);

            List<Event> todayEvents = schedule[2023, 10, 11];

            //print data
            Console.WriteLine("DNEŠNÍ UDÁLOSTI:");
            Console.WriteLine("----------");
            foreach (Event e in todayEvents)
            {
                Console.WriteLine(e);
            }

            Console.WriteLine("");

            Console.WriteLine("STUDENTI, KTERÉ DNES UVIDÍM:");
            Console.WriteLine("----------");
            foreach (Event e in todayEvents)
            {
                if (e is Tutorial tutorial)
                {
                    Student[] students = tutorial.Students;

                    foreach (Student student in students)
                    {
                        StringBuilder sb = new();
                        sb.Append(student);
                        sb.Append(" - ");
                        sb.Append(tutorial.Start);
                        Console.WriteLine(sb.ToString());
                    }
                } else if (e is Consultation consultation)
                {
                    Student[] students = consultation.Students;

                    foreach (Student student in students)
                    {
                        StringBuilder sb = new();
                        sb.Append(student);
                        sb.Append(" - ");
                        sb.Append(consultation.Start);
                        Console.WriteLine(sb.ToString());
                    }
                }
            }

            Console.WriteLine("");

            Console.WriteLine("ČASY PŘIPOMENUTÍ DNEŠNÍCH UDÁLOSTÍ:");
            Console.WriteLine("----------");
            foreach (Event e in todayEvents)
            {
                StringBuilder sb = new();
                sb.Append(e);
                sb.Append(" => ");
                sb.Append(e.GetReminderTime());
                Console.WriteLine(sb.ToString());
            }

            Console.WriteLine("");

            Console.WriteLine("BUDOUCÍ UDÁLOSTI:");
            Console.WriteLine("----------");
            List<Event> upcomingEvents = schedule.GetAllUpcoming();

            foreach (Event e in upcomingEvents)
            {
                Console.WriteLine(e);
            }
        }

        static Schedule ParseInput(string FileName)
        {
            string data = File.ReadAllText(FileName);
            string[] lines = data.Split("\n");

            EventType eventType = EventType.None;
            Schedule schedule = new();

            for (int i = 0; i < lines.Length; ++i)
            {
                string line = lines[i];

                //set event
                if (line.StartsWith("LECTURE"))
                {
                    eventType = EventType.Lecture;
                } else if (line.StartsWith("TUTORIAL"))
                {
                    eventType = EventType.Tutorial;
                } else if (line.StartsWith("CONSULTATION"))
                {
                    eventType = EventType.Consultation;
                }

                //if no event, skip
                if (eventType == EventType.None)
                {
                    continue;
                }

                //shared data
                string name = lines[i + 1];
                string[] dates = lines[i + 2].Split(";");
                DateTime start = DateTime.Parse(dates[0]);
                DateTime end = DateTime.Parse(dates[1]);

                //get data
                switch (eventType)
                {
                    case EventType.Lecture:
                        //create lecture
                        Lecture lecture = new(name, start, end);
                        schedule.Add(lecture);

                        //skip to next event
                        i += 2;
                        break;
                    case EventType.Tutorial:
                        {
                            //get additional data
                            string namesLine = lines[i + 3];
                            string[] names = lines[i + 3].Split(";");
                            if (namesLine.Length == 0)
                            {
                                names = new string[0];
                            }

                            Student[] students = new Student[names.Length];
                            int l = 0;
                            foreach (string student in names)
                            { 
                                students[l] = new Student(student);
                                ++l;
                            }

                            //create tutorial
                            Tutorial tutorial = new(name, start, end, students);
                            schedule.Add(tutorial);

                            //skip to next event
                            i += 3;
                        }
                        break;
                    case EventType.Consultation:
                        {
                            //get additonal data
                            string? location = null;
                            string locationLine = lines[i + 3];
                            if (locationLine.Length > 0)
                            {
                                location = locationLine;
                            }

                            string namesLine = lines[i + 4];
                            string[] names = lines[i + 4].Split(";");
                            if (namesLine.Length == 0)
                            {
                                names = new string[0];
                            }
                            Student[] students = new Student[names.Length];
                            int j = 0;
                            foreach (string student in names)
                            {
                                students[j] = new Student(student);
                                ++j;
                            }

                            //Create consultation
                            Consultation consultation = new(name, start, end)
                            {
                                Location = location,
                                Students = students
                            };
                            schedule.Add(consultation);

                            //skip to next event
                            i += 4;
                        }
                        break;
                }
                    
                //Reset for next event
                eventType = EventType.None;
            }

            return schedule;
        }
    }
}