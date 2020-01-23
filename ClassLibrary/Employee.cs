using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary
{
    [DataContract]
    public class Employee: Person
    {
        [DataMember]
        protected string graduation;
        [DataMember]
        protected string company;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Employee(string _name = "Bob", int _age = 30, string _graduation = "MIT", string _company = "Google") : base(_name, _age)
        {
            graduation = _graduation;
            company = _company;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Employee()
        {
            unusedNames.Add(nameNumber);
        }
        public void ChangeCompany(string new_company)
        {
            company = new_company;
        }
        public override string GetName()
        {
            return this.GetType().Name + nameNumber.ToString();
        }
    }
}
