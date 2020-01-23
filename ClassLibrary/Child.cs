using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary
{
    [DataContract]
    public class Child : Person
    {
        [DataMember]
        protected string favouriteToy;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Child(string _name = "Tom", int _age = 8, string _favouriteToy = "bear") : base(_name, _age)
        {
            favouriteToy = _favouriteToy;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Child()
        {
            unusedNames.Add(nameNumber);
        }
        public void ChangeFavouriteToy(string newFavouriteToy)
        {
            favouriteToy = newFavouriteToy;
        }
        public override string GetName()
        {
            return this.GetType().Name + nameNumber.ToString();
        }
    }
}
