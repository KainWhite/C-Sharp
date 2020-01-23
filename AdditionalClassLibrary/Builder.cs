using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace AdditionalClassLibrary
{
    public abstract class Builder: IBuilder
    {
        public string name;
        public Builder(string _name)
        {
            name = _name;
        }
        public abstract object Build(int cost, string adress, int area, int floorCount);
        public abstract int GetCost(int area, int floorCount);
        public abstract string GetName();
    }
}
