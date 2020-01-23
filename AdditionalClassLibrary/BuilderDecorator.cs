using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace AdditionalClassLibrary
{
    public abstract class BuilderDecorator: Builder, IDecorator
    {
        protected Builder builder;
        public BuilderDecorator(string _name, Builder _builder) : base(_name)
        {
            builder = _builder;
        }
        public static Type[] GetDecoratedTypes()
        {
            Assembly curAsm = Assembly.GetExecutingAssembly();
            Type[] allTypes = curAsm.GetTypes();
            List<Type> resTypes = new List<Type>();
            for(int i = 0; i < allTypes.Length; i++)
            {
                if(typeof(IInvisible).IsAssignableFrom(allTypes[i]))
                {
                    resTypes.Add(allTypes[i]);
                }
            }
            return resTypes.ToArray();
        }
        public bool IsDecorator()
        {
            return true;
        }
    }
}
