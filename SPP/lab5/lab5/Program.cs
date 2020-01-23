using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace lab5
{
    public delegate void Task();
    public class Parallel
    {
        public static void WaitAll(List<Task> tasks)
        {
            using (var countdownEvent = new CountdownEvent(tasks.Count))
            {
                foreach(Task t in tasks)
                {
                    ThreadPool.QueueUserWorkItem((x) =>
                    {
                        t();
                        countdownEvent.Signal();
                    });
                }
                countdownEvent.Wait();
            }
            Console.WriteLine("Threads done");
        }
    }
    class Program
    {
        static void SomeTask()
        {
            var number = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine($"Thread {number} starts");
            for (int i = 0; i < 1000000000; i++) ;
            Console.WriteLine($"Thread {number} finishes");
        }
        static void Main(string[] args)
        {
            List<Task> tasks = new List<Task>();
            for(int i = 0; i < 10; i++)
            {
                tasks.Add(SomeTask);
            }
            Console.WriteLine("Starting threads");
            Parallel.WaitAll(tasks);
            Console.ReadLine();
        }
    }
}
