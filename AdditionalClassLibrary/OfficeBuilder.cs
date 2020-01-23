using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace AdditionalClassLibrary
{
    [DataContract]
    public class OfficeBuilder : Builder, IInvisible
    {
        private static HashSet<int> unusedNames = new HashSet<int>(Enumerable.Range(1, 100));
        [DataMember]
        private int nameNumber;
        public OfficeBuilder(string _name) : base(_name) { }
        public override object Build(int cost = -1, string adress = "Kek st.", int area = 30, int floorCount = 1000)
        {
            if (cost == -1)
                return new Office(GetCost(area, floorCount), adress, area, floorCount);
            else
                return new Office(cost, adress, area, floorCount);
        }
        public override int GetCost(int area, int floorCount)
        {
            return area * floorCount;
        }
        public override string GetName()
        {
            return GetType().Name + nameNumber.ToString();
        }
        public void MakeInvisible() { }
    }
}
