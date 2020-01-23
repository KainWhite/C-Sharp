using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace ClassLibrary
{
    [DataContract, KnownType(typeof(Child)), KnownType(typeof(Employee)), KnownType(typeof(Parasite)), KnownType(typeof(Pupil)), KnownType(typeof(Student))]
    public abstract class Person: IUnit
    {
        [DataMember]
        protected string name;
        [DataMember]
        protected int age;
        public Person(string _name = "Mark", int _age = 20)
        {
            name = _name;
            age = _age;
        }
        public void Birthday()
        {
            age++;
        }
        public void ChangeName(string newName)
        {
            name = newName;
        }
        public abstract string GetName();
    }
}
