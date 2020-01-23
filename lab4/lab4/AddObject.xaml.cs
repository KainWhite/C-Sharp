using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace lab4
{
    /// <summary>
    /// Interaction logic for AddObject.xaml
    /// </summary>
    public partial class AddObject : Window
    {
        private ParameterInfo[] ctorParams;
        private List<object> objParams;
        public object[] ObjParams
        {
            get
            {
                return objParams.ToArray();
            }
            private set { }
        }
        public AddObject(ParameterInfo[] _ctorParams)
        {
            InitializeComponent();
            ctorParams = _ctorParams;
            objParams = new List<object>();
            for (int i = 0; i < ctorParams.Length; i++)
            {
                StackPanel paramDef = new StackPanel();
                paramDef.Orientation = Orientation.Horizontal;
                paramDef.Margin = new Thickness(20, 10, 20, 10);

                Label paramName = new Label();
                paramName.Content = ctorParams[i].Name + ":";
                TextBox paramValue = new TextBox();
                paramValue.TextWrapping = TextWrapping.NoWrap;
                paramValue.Margin = new Thickness(20, 0, 0, 0);
                paramValue.MinWidth = 50;

                paramDef.Children.Add(paramName);
                paramDef.Children.Add(paramValue);
                stackObjDefinition.Children.Add(paramDef);
            }
        }
        public void btnApplyClick(object sender, RoutedEventArgs e)
        {
            StackPanel[] paramDefs = stackObjDefinition.Children.OfType<StackPanel>().ToArray();
            TextBox[] paramValues = new TextBox[paramDefs.Length];
            for (int i = 0; i < paramDefs.Length; i++)
            {
                paramValues[i] = paramDefs[i].Children.OfType<TextBox>().ToArray()[0];
            }
            for (int i = 0; i < ctorParams.Length; i++)
            {
                try
                {
                    objParams.Add(TypeDescriptor.GetConverter(ctorParams[i].ParameterType).ConvertFromString(paramValues[i].Text));
                }
                catch
                {
                    objParams.Add(null);
                }
            }
            if(sender == btnApply)
                this.DialogResult = true;
        }
    }
}
