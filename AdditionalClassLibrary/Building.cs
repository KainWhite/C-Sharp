using System;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace AdditionalClassLibrary
{
    [DataContract, KnownType(typeof(Office))]
    public abstract class Building: IUnit
    {
        [DataMember]
        protected string adress;
        [DataMember]
        protected int area;
        [DataMember]
        protected int floorCount;
        [DataMember]
        protected int cost;
        public Building(int _cost = -1, string _adress = "Kek st.", int _area = 30, int _floorCount = 1000)
        {
            adress = _adress;
            area = _area;
            floorCount = _floorCount;
            cost = _cost;
        }
        public void ChangeArea(int newArea)
        {
            area = newArea;
        }
        public abstract string GetName();
    }
}
