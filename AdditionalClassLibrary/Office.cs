using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdditionalClassLibrary
{
    [DataContract]
    public class Office: Building
    {
        [DataMember]
        protected int accessibility;
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public Office(int _cost = -1, string _adress = "Kek st.", int _area = 30, int _floorCount = 1000, int _accessibility = 80): 
            base(_cost, _adress, _area, _floorCount)
        {
            accessibility = _accessibility;
            nameNumber = unusedNames.First();
            unusedNames.Remove(nameNumber);
        }
        ~Office()
        {
            unusedNames.Add(nameNumber);
        }
        public override string GetName()
        {
            return GetType().Name + nameNumber.ToString();
        }
    }
}
