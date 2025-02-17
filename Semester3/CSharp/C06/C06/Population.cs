using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C06
{
    internal class Population
    {
        public int PersonCount { get; set; }
        List<Person> persons = new();

        public Population()
        {
        }

        public void Add(Person person)
        {
            PersonCount++;
            persons.Add(person);
        }

        public void PrintUnemployed()
        {
            foreach(Person person in persons)
            {
                if (person is Unemployed unemployed)
                {
                    unemployed.PrintInfo();
                }
            }
        }

        public void PrintEmployeesWithSalary()
        {
            foreach(Person person in persons) { 
                if (person is Employee employee)
                {
                    employee.PrintInfo();
                    Console.WriteLine(employee.GetSalary());
                }
            }
        }

        public Employee? GetPersonWithHighestSalary()
        {
            Employee? e = null;

            foreach (Person person in persons) { 
                if (person is Employee employee)
                {
                    if (e is null)
                    {
                        e = employee;
                    } else if (employee > e)
                    {
                        e = employee;
                    }
                }
            }

            return e;
        }

        public List<HourlySalaryEmployee> GetHourlySalaryEmployees()
        {
            List<HourlySalaryEmployee> employees = new();
           
            foreach(Person employee in persons)
            {
                if (employee is HourlySalaryEmployee hourlyEmployee) {
                    employees.Add(hourlyEmployee);
                }
            }

            return employees;
        }

        public void IncreaseHorlySalary(List<HourlySalaryEmployee> employees, double increase) 
        {
            foreach(HourlySalaryEmployee e in employees)
            {
                e.HourlySalary = e.HourlySalary + increase;
            }
        }
    }
}
