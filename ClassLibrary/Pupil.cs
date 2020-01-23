using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary
{
    [DataContract]
    public class Pupil : Child
    {
        [DataMember]
        protected int schoolNumber;
        [DataMember]
        protected string favouriteSubject;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Pupil(string _name = "Alex", int _age = 10, string _favouriteToy = "bear", int _schoolNumber = 42, string _favouriteSubject = "math") : base(_name, _age, _favouriteToy)
        {
            schoolNumber = _schoolNumber;
            favouriteSubject = _favouriteSubject;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Pupil()
        {
            unusedNames.Add(nameNumber);
        }
        public void ChangeSchool(int newSchoolNumber)
        {
            schoolNumber = newSchoolNumber;
        }
        public override string GetName()
        {
            return this.GetType().Name + nameNumber.ToString();
        }
    }
}
