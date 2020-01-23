using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Threading;
using System.Collections;

namespace lab4
{
    public class Comparer : IComparer
    {
        public int Compare(object x, object y)
        {
            Type a = x as Type;
            Type b = y as Type;
            int assCmp = string.Compare(a.Namespace, b.Namespace);
            //Console.WriteLine($"{a.Namespace} {b.Namespace} {assCmp}");
            if (assCmp != 0)
            {
                return assCmp;
            }
            else
            {
                return string.Compare(a.Name, b.Name);
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("Not enough arguments");
                Console.ReadLine();
                return;
            }
            Assembly asm = Assembly.LoadFrom(args[0]);
            Type[] types = asm.GetTypes();
            IComparer cmp = new Comparer();
            Array.Sort(types, cmp);
            foreach (Type type in types)
            {
                Console.WriteLine(type.FullName);
            }
            Console.ReadLine();
        }
    }
}
