using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab6
{
    public class DynamicList<T> : IEnumerable<T>
    {
        private T[] elements;
        public int Count { get; private set; } = 0;
        public DynamicList()
        {
            elements = new T[10];
        }
        public T this[int index]
        {
            get
            {
                if(index >= Count)
                {
                    throw new IndexOutOfRangeException();
                }
                return elements[index];
            }
            set
            {
                if (index >= Count)
                {
                    throw new IndexOutOfRangeException();
                }
                elements[index] = value;
            }
        }
        public void Add(T x)
        {
            if(elements.Length <= Count)
            {
                T[] newElements = new T[elements.Length * 2];
                for(int i = 0; i < elements.Length; i++)
                {
                    newElements[i] = elements[i];
                }
                elements = newElements;
            }
            elements[Count] = x;
            Count++;
        }
        public void Remove(int start, int count)
        {
            count = Math.Max(count, 0);
            for (int i = start; i < Count - count; i++)
            {
                if (elements.Length > i + count)
                {
                    elements[i] = elements[i + count];
                }
            }
            Count -= Math.Min(Math.Max(Count - start, 0), count);
            //Console.WriteLine(Count);
        }
        public void RemoveAt(int index)
        {
            Remove(index, 1);
        }
        public void Clear()
        {
            Count = 0;
        }

        public IEnumerator<T> GetEnumerator()
        {
            for(int i = 0; i < Count; i++)
            {
                yield return elements[i];
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            DynamicList<int> dl = new DynamicList<int>();
            for(int i = 0; i < 25; i++)
            {
                dl.Add(i);
            }
            for (int i = 0; i < dl.Count; i++)
            {
                Console.WriteLine(dl[i]);
            }
            Console.WriteLine("////////////////////////////");
            dl.Remove(10, 10);
            for (int i = 0; i < dl.Count; i++)
            {
                Console.WriteLine(dl[i]);
            }
            Console.WriteLine("////////////////////////////");
            dl.RemoveAt(10);
            for (int i = 0; i < dl.Count; i++)
            {
                Console.WriteLine(dl[i]);
            }
            Console.WriteLine("////////////////////////////");
            dl.RemoveAt(20);
            for (int i = 0; i < dl.Count; i++)
            {
                Console.WriteLine(dl[i]);
            }
            Console.WriteLine("////////////////////////////");
            dl.Clear();
            for(int i = 0; i < dl.Count; i++)
            {
                Console.WriteLine(dl[i]);
            }
            Console.WriteLine("////////////////////////////");
            //Console.WriteLine(dl.Count);
            Console.ReadLine();
        }
    }
}
