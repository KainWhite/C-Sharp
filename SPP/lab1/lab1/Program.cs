using System;
using System.Collections.Generic;
using System.Threading;

namespace lab1
{
    class TaskQueue : IDisposable
    {
        public delegate void Task();
        object locker = new object();
        List<Thread> threadList;
        Queue<Task> taskQueue;
        public TaskQueue(int threadNumber)
        {
            threadList = new List<Thread>();
            taskQueue = new Queue<Task>();
            for (int i = 0; i < threadNumber; i++)
            {
                Thread t = new Thread(() => Consume(i));
                threadList.Add(t);
                t.Start();
            }
        }
        private void Consume(int threadNumber)
        {
            while (true)
            {
                Task someTask;
                lock (locker)
                {
                    while (taskQueue.Count == 0)
                    {
                        Monitor.Wait(locker);
                    }
                    someTask = taskQueue.Dequeue();
                }
                if (someTask == null)
                {
                    return;
                }
                Console.Write($"Thread {threadNumber} starts: ");
                someTask();
                Console.WriteLine($", thread {threadNumber} finishes.");
            }

        }
        public void EnqueueTask(Task task)
        {
            lock (locker)
            {
                taskQueue.Enqueue(task);
                Monitor.PulseAll(locker);
            }
        }
        public void Dispose()
        {
            threadList.ForEach((thread) => { EnqueueTask(null); });
            threadList.ForEach((thread) => { thread.Join(); });
        }
        ~TaskQueue()
        {
            Dispose();
        }
    };
    class Program
    {
        static void count1bTicks()
        {
            Console.WriteLine("counting 1/10 billion ticks...");
            for (long i = 0; i < 100000000; i++) ;
            Console.Write("1 billion ticks counted");
        }
        static void count2bTicks()
        {
            Console.WriteLine("counting 2 billion ticks...");
            for (long i = 0; i < 2000000000; i++) ;
            Console.Write("2 billion ticks counted");
        }
        static void count3bTicks()
        {
            Console.WriteLine("counting 3 billion ticks...");
            for (long i = 0; i < 3000000000; i++) ;
            Console.Write("3 billion ticks counted");
        }
        static void Main(string[] args)
        {
            var watch = System.Diagnostics.Stopwatch.StartNew();
            using (TaskQueue tq = new TaskQueue(10))
            {
                for (int i = 0; i < 100; i++)
                {
                    //Console.WriteLine(i + " !!!!!!!!!!!!!!!!!");
                    /*var rand = new Random();
                    int q = rand.Next(3);
                    switch (q)
                    {
                        case 0:
                            tq.EnqueueTask(count1bTicks);
                            break;
                        case 1:
                            tq.EnqueueTask(count2bTicks);
                            break;
                        case 2:
                            tq.EnqueueTask(count3bTicks);
                            break;
                    }*/
                    tq.EnqueueTask(count1bTicks);
                }
            }
            watch.Stop();
            Console.WriteLine(watch.ElapsedMilliseconds);
        }
    }
}
