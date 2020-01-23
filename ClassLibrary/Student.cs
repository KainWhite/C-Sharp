using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary
{
    [DataContract]
    public class Student: Person
    {
        [DataMember]
        protected string university;
        [DataMember]
        protected string favouriteSubject;
        [DataMember]
        protected int subjectUnderstanding;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Student(string _name = "Xiaoice", int _age = 20, string _university = "MIT", string _favouriteSubject = "math", int _subjectUnderstanding = 95) : base(_name, _age)
        {
            university = _university;
            favouriteSubject = _favouriteSubject;
            subjectUnderstanding = _subjectUnderstanding;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Student()
        {
            unusedNames.Add(nameNumber);
        }
        public void ChangeSubjectUnderstanding(int newSubjectUnderstanding)
        {
            subjectUnderstanding = newSubjectUnderstanding;
        }
        public void ChangeUniversity(string new_university)
        {
            university = new_university;
        }
        public override string GetName()
        {
            return this.GetType().Name + nameNumber.ToString();
        }
    }
}
