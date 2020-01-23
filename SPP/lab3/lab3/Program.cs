using System;
using System.Threading;

namespace lab3
{
    public class Mutex
    {
        private int locker;
        private int number;
        public int Number
        {
            get
            {
                if(locker == Thread.CurrentThread.ManagedThreadId)
                {
                    return number;
                }
                Console.WriteLine("Blocked getting");
                return -1;
            }
            set
            {
                if (locker == Thread.CurrentThread.ManagedThreadId)
                {
                    number = value;
                }
                else
                {
                    Console.WriteLine("Blocked setting");
                }
            }
        }
        public Mutex(int _number)
        {
            locker = 0;
            number = _number;
        }
        public void Lock()
        {
            Interlocked.CompareExchange(ref locker, Thread.CurrentThread.ManagedThreadId, 0);
        }
        public void Unlock()
        {
            Interlocked.CompareExchange(ref locker, 0, Thread.CurrentThread.ManagedThreadId);
        }
    }
    class Program
    {
        static void task(Mutex mutex)
        {
            mutex.Lock();
            mutex.Number = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine(mutex.Number);
            mutex.Unlock();
        }
        static void Main(string[] args)
        {
            Mutex mutex = new Mutex(0);
            Thread t1 = new Thread(() => task(mutex));
            Thread t2 = new Thread(() => task(mutex));
            Console.WriteLine($"t1 id - {t1.ManagedThreadId}");
            Console.WriteLine($"t2 id - {t2.ManagedThreadId}");
            t1.Start();
            t2.Start();
            Thread.Sleep(2000);
        }
    }
}
