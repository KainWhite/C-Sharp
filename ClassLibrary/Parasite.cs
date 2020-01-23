using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary
{
    [DataContract]
    public class Parasite: Person
    {
        [DataMember]
        protected bool drunk = true;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Parasite(string _name = "Kek", int _age = 40, bool _drunk = true) : base(_name, _age)
        {
            drunk = _drunk;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Parasite()
        {
            unusedNames.Add(nameNumber);
        }
        public void Sober()
        {
            drunk = false;
        }
        public void Drink()
        {
            drunk = true;
        }
        public override string GetName()
        {
            return this.GetType().Name + nameNumber.ToString();
        }
    }
}
