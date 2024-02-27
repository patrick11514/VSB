

using MyLib;
using MyLib.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;




namespace ReflectionApp
{
    public class Program
    {
        public static void Main(string[] args)
        {
            string path = Path.GetFullPath("../../../../MyLib/bin/Debug/net6.0/MyLib.dll");

            Assembly assembly = Assembly.LoadFile(path);

            foreach (Type type in assembly.GetTypes())
            {
                Console.WriteLine(type.FullName);
            }

            //object customerController = assembly.CreateInstance("MyLib.Controllers.CustomerController");

            Type controllerType = assembly.GetType("MyLib.Controllers.CustomerController");

            object customerController = Activator.CreateInstance(controllerType);

            MethodInfo listMethod = controllerType.GetMethod("List");

            foreach (ParameterInfo par in listMethod.GetParameters())
            {
                Console.WriteLine(par.Name + " | " + par.ParameterType);
            }

            string result = (string) listMethod.Invoke(customerController, new object[] { 2 });

            Console.WriteLine(result);

            string url = "/Customer/Add?Name=Pepa&Age=30&IsActive=true";

            string[] parts = url.Split("?");
            string[] left = parts[0].Split("/");
            string[] right = parts[1].Split("&");

            string controllerName = left[1];
            string methodName = left[2];

            Dictionary<string, string> arguments = right.Select((pair) => pair.Split("=")).ToDictionary(x => x[0], x => x[1], StringComparer.OrdinalIgnoreCase);

            string classFullName = "MyLib.Controllers." + controllerName + "Controller";
            Type classType = assembly.GetType(classFullName);

            if (classType == null)
            {
                Console.WriteLine("404");
                return;
            }

            MethodInfo classMethod = classType.GetMethod(methodName);

            if (methodName == null)
            {
                Console.WriteLine("404");
                return;
            }

            object controllerObj = Activator.CreateInstance(classType);

            List<object> methodArgs = new();

            foreach (var mPar in classMethod.GetParameters())
            {
                if (!arguments.TryGetValue(mPar.Name, out string val))
                {
                    Console.WriteLine("404");
                    return;
                }

                if (mPar.ParameterType == typeof(int))
                {
                    methodArgs.Add(int.Parse(val));
                } 
                else if (mPar.ParameterType == typeof(bool)) {
                    methodArgs.Add(bool.Parse(val));
                }
                else if (mPar.ParameterType == typeof(string))
                {
                    methodArgs.Add(val);
                } 
                else
                {
                    throw new NotImplementedException();
                } 
            }

            object methodResult = classMethod.Invoke(controllerObj, methodArgs.ToArray());

            Console.WriteLine(methodResult);

            //--

            Type customerType = typeof(Customer);//assembly.GetType("MyLib.Models.Customer");
            Customer customerObj = (Customer)Activator.CreateInstance(customerType);


            PropertyInfo nameProperty = customerType.GetProperty("Name");
            nameProperty.SetValue(customerObj, "Pepa");

            var attrs = customerType.GetCustomAttributes().ToArray();

            foreach (var attr in attrs)
            {
                if (attr is TestAttribute t) {
                    Console.WriteLine("Moje hodnota: " + t.Name);
                }
            }
        }

    }
}