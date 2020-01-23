using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdditionalClassLibrary
{
    public class BuilderGold: BuilderDecorator
    {
        public BuilderGold(Builder _builder) : base(_builder.name + "(Gold)", _builder) { }
        public override object Build(int cost = -1, string adress = "Kek st.", int area = 30, int floorCount = 1000)
        {
            cost = GetCost(area, floorCount);
            return builder.Build(cost, adress, area, floorCount);
        }
        public override int GetCost(int area, int floorCount)
        {
            return builder.GetCost(area, floorCount) * 10;
        }
        public override string GetName()
        {
            return builder.GetName();
        }
    }
}
