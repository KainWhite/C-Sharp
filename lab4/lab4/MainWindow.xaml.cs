using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using Microsoft.Win32;
using LibraryInterfaces;
using System.Security.Cryptography;
using System.Text;
using System.Linq;

namespace lab4
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private List<object> createdObjects;
        private List<Border> createdLabels;
        private HashSet<object> selectedObjects;
        private List<Assembly> loadedAsms;
        private List<Type> loadedTypes;
        private List<Type> serializableTypes;
        private bool CrypterEnabled;

        public MainWindow()
        {
            InitializeComponent();
            createdObjects = new List<object>();
            createdLabels = new List<Border>();
            selectedObjects = new HashSet<object>();
            loadedAsms = new List<Assembly>();
            loadedTypes = new List<Type>();
            serializableTypes = new List<Type>();
            CrypterEnabled = false;

            AddAssembly(@"..\..\..\..\ClassLibrary\bin\Release\ClassLibrary.dll", false);
            cbClassNames.SelectedItem = cbClassNames.Items[0];
        }

        private int AddAssembly(string path, bool checkNeeded)
        {
            if (checkNeeded && !CheckFile(path))
                return 0;
            Assembly asm = Assembly.LoadFrom(path);
            if (loadedAsms.Contains(asm))
                return 0;
            Type[] classLibTypes = asm.GetTypes();
            int unitClassesAdded = 0;
            for (int i = 0; i < classLibTypes.Length; i++)
            {
                if (!classLibTypes[i].IsAbstract)
                {
                    if (typeof(IUnit).IsAssignableFrom(classLibTypes[i]) && !typeof(IInvisible).IsAssignableFrom(classLibTypes[i]))
                    {
                        if(typeof(IDecorator).IsAssignableFrom(classLibTypes[i]))
                        {
                            Type[] decoratedTypes = (Type[])classLibTypes[i].GetMethod(
                                "GetDecoratedTypes", 
                                BindingFlags.Public |
                                BindingFlags.Static |
                                BindingFlags.Instance |
                                BindingFlags.FlattenHierarchy
                            ).Invoke(null, null);
                            for(int j = 0; j < decoratedTypes.Length; j++)
                            {
                                cbClassNames.Items.Add($"{decoratedTypes[j].Assembly.FullName} {classLibTypes[i].Assembly.FullName}");
                                unitClassesAdded++;
                            }
                        }
                        else
                        {
                            cbClassNames.Items.Add(classLibTypes[i]);
                            unitClassesAdded++;
                        }
                    }
                    if (typeof(ICrypter).IsAssignableFrom(classLibTypes[i]) && !CrypterEnabled)
                    {
                        StackPanel spCrypt = new StackPanel();

                        CheckBox cbEncryptionEnabled = new CheckBox();
                        cbEncryptionEnabled.IsChecked = true;
                        cbEncryptionEnabled.Content = "Enable encryption";

                        spCrypt.Children.Add(cbEncryptionEnabled);
                        spCrypt.Margin = new Thickness(30, 0, 0, 0);

                        dpControls.Children.Insert(1, spCrypt);

                        CrypterEnabled = true;
                    }
                }
                if (typeof(IUnit).IsAssignableFrom(classLibTypes[i]))
                    serializableTypes.Add(classLibTypes[i]);
                if (typeof(IUnit).IsAssignableFrom(classLibTypes[i]) || typeof(ICrypter).IsAssignableFrom(classLibTypes[i]))
                    loadedTypes.Add(classLibTypes[i]);
            }
            loadedAsms.Add(asm);
            return unitClassesAdded;
        }

        private string SubscribeFile(string path)
        {
            byte[] hash;
            using (FileStream fs = File.OpenRead(path))
            {
                MD5 md5 = new MD5CryptoServiceProvider();
                byte[] fileData = new byte[fs.Length];
                fs.Read(fileData, 0, (int)fs.Length);
                hash = md5.ComputeHash(fileData);
            }
            DateTime dt = DateTime.Now;
            byte[] dtByte = BitConverter.GetBytes(dt.Ticks);
            using (FileStream fs = File.Open(path + ".sign", FileMode.Create))
            {
                fs.Write(hash, 0, hash.Length);
                fs.Write(dtByte, 0, dtByte.Length);
            }
            return path + ".sign";
        }

        private bool CheckFile(string path)
        {
            string subscriptPath = path + ".sign";
            if(File.Exists(path + ".sign"))
            {
                using (FileStream fsSign = File.Open(path + ".sign", FileMode.Open))
                {
                    byte[] hashSign = new byte[16];
                    fsSign.Read(hashSign, 0, 16);
                    byte[] dtByte = new byte[fsSign.Length - fsSign.Position + 1];
                    fsSign.Read(dtByte, 0, (int)(fsSign.Length - fsSign.Position + 1));
                    DateTime dtSign = DateTime.FromBinary(BitConverter.ToInt64(dtByte, 0));
                    byte[] hash;
                    using (FileStream fs = File.OpenRead(path))
                    {
                        MD5 md5 = new MD5CryptoServiceProvider();
                        byte[] fileData = new byte[fs.Length];
                        fs.Read(fileData, 0, (int)fs.Length);
                        hash = md5.ComputeHash(fileData);
                    }
                    if (hashSign.SequenceEqual(hash))
                    {
                        MessageBox.Show($"Subscribe time: {dtSign}",
                                        "File sign",
                                        MessageBoxButton.OK,
                                        MessageBoxImage.Information);
                        return true;
                    }
                    else
                    {
                        for (int i = 0; i < Math.Max(hash.Length, hashSign.Length); i++)
                        {
                            if (i >= hash.Length)
                            {
                                Console.Write("\t-\t");
                            }
                            else
                            {
                                Console.Write($"{hash[i]}\t-\t");
                            }
                            if (i < hashSign.Length)
                            {
                                Console.Write($"{hashSign[i]}");
                            }
                            Console.Write("\n");
                        }
                        MessageBox.Show($"Signature: {Encoding.UTF8.GetString(hashSign)}\n" +
                                        $"Real hash: {Encoding.UTF8.GetString(hash)}\n",
                                        "File sign",
                                        MessageBoxButton.OK,
                                        MessageBoxImage.Error);
                        return false;
                    }
                }
                
            }
            else
            {
                var msgResult = MessageBox.Show("File was not signed.\nSubscribe it now?", 
                                                "File not signed", 
                                                 MessageBoxButton.YesNo, 
                                                 MessageBoxImage.Warning);
                if(msgResult == MessageBoxResult.Yes)
                {
                    SubscribeFile(path);
                    return true;
                }
                else
                    return false;
            }
        }

        private void objectLabelClick(object sender, RoutedEventArgs e)
        {
            object curSelectedObject = createdObjects[createdLabels.IndexOf(sender as Border)];
            if (selectedObjects.Contains(curSelectedObject))
            {
                selectedObjects.Remove(curSelectedObject);
                (sender as Border).BorderBrush = new SolidColorBrush(Color.FromRgb(200, 200, 200));
            }
            else
            {
                selectedObjects.Add(curSelectedObject);
                (sender as Border).BorderBrush = new SolidColorBrush(Color.FromRgb(0, 100, 255));
            }
        }

        private void btnCreateObjectClick(object sender, RoutedEventArgs e)
        {
            Type objType = null;
            Type decoratorType = null;
            objType = (Type)cbClassNames.SelectedItem;
            //if(objType == null)
            //{
            //    string[] types = cbClassNames.SelectedItem.ToString().Split(' ');
            //    //objType = Type.GetType("AdditionalClassLibrary.OfficeBuilder");
            //    objType = Type.GetType(types[0]);
            //    Console.WriteLine(types[0]);
            //    Console.WriteLine(objType);
            //    decoratorType = Type.GetType(types[1]);
            //}

            ConstructorInfo[] ctors = objType.GetConstructors();
            ParameterInfo[] ctorParams = new ParameterInfo[0];
            for (int i = 0; i < ctors.Length; i++)
            {
                ParameterInfo[] curCtorParams = ctors[i].GetParameters();
                if (ctorParams.Length < curCtorParams.Length)
                    ctorParams = curCtorParams;
            }
            if (ctorParams.Length == 0)
                return;

            AddObject wndAddObject = new AddObject(ctorParams);
            wndAddObject.Owner = this;
            if(sender == btnCreateObject)
                if (wndAddObject.ShowDialog() == false)
                    return;
            if (sender == btnCreateDefaultObject)
                wndAddObject.btnApplyClick(sender, e);

            object curObj = Activator.CreateInstance(objType, wndAddObject.ObjParams);
            //if(decoratorType != null)
            //{
            //    curObj = Activator.CreateInstance(decoratorType, new object[] { curObj });
            //}
            createdObjects.Add(curObj);
            Border wrap = new Border();
            wrap.BorderBrush = new SolidColorBrush(Color.FromRgb(200, 200, 200));
            wrap.BorderThickness = new Thickness(3);
            wrap.Margin = new Thickness(5);
            ToolTip tt = new ToolTip();
            TextBlock text = new TextBlock();
            tt.Content = "";
            FieldInfo[] curObjFields = objType.GetFields(BindingFlags.Instance | BindingFlags.NonPublic);
            for (int i = 0; i < curObjFields.Length; i++)
            {
                tt.Content += curObjFields[i].Name + ": " + curObjFields[i].GetValue(curObj) + "\n";
            }
            text.Inlines.Add(new Bold(new Run( (curObj as IUnit).GetName() )));
            text.ToolTip = tt;
            text.Padding = new Thickness(20);
            wrap.Child = text;
            wrap.PreviewMouseDown += objectLabelClick;
            wpCreatedObjects.Children.Add(wrap);
            createdLabels.Add(wrap);
        }

        private void btnRemoveObjectClick(object sender, RoutedEventArgs e)
        {
            foreach (object curObj in selectedObjects)
            {
                wpCreatedObjects.Children.Remove(createdLabels[createdObjects.IndexOf(curObj)]);
                createdLabels.Remove(createdLabels[createdObjects.IndexOf(curObj)]);
                createdObjects.Remove(curObj);
            }
            selectedObjects.Clear();
        }
        
        private void btnSerializeClick(object sender, RoutedEventArgs e)
        {
            DataContractJsonSerializer jsonFormatter = new DataContractJsonSerializer(typeof(HashSet<object>));
            var checkBox = VisualTreeHelper.GetChild(VisualTreeHelper.GetChild(dpControls, 1), 0);
            if (!(checkBox is CheckBox) || (checkBox as CheckBox).IsChecked == false)
            {
                using (FileStream fs = new FileStream("people.json", FileMode.Create))
                {
                    jsonFormatter.WriteObject(fs, selectedObjects);
                }
            }
            else
            {
                Type Crypter = loadedTypes.Where((type) => type.Name == "Crypter").FirstOrDefault();
                Type CrypterProxy = loadedTypes.Where((type) => type.Name == "CrypterProxy").FirstOrDefault();
                object crypter = Activator.CreateInstance(Crypter);
                object crypterProxy = Activator.CreateInstance(CrypterProxy, new object[] { crypter, 10 });
                //object crypter = Crypter.GetInstance();
                byte[] data;
                using (MemoryStream ms = new MemoryStream())
                {
                    jsonFormatter.WriteObject(ms, selectedObjects);
                    data = ms.ToArray();
                }
                data = (crypterProxy as ICrypter).Encrypt(data);
                using (FileStream fs = new FileStream("people.encrypt", FileMode.Create))
                {
                    fs.Write(data, 0, data.Length);
                }
            }
            
        }

        private void btnDeserializeClick(object sender, RoutedEventArgs e)
        {
            DataContractJsonSerializer jsonFormatter = new DataContractJsonSerializer(typeof(HashSet<object>), serializableTypes.ToArray());
            HashSet<object> deserializedObjects;
            var checkBox = VisualTreeHelper.GetChild(VisualTreeHelper.GetChild(dpControls, 1), 0);
            if (!(checkBox is CheckBox) || (checkBox as CheckBox).IsChecked == false)
            {
                using (FileStream fs = new FileStream("people.json", FileMode.Open))
                {
                    deserializedObjects = (HashSet<object>)jsonFormatter.ReadObject(fs);
                }
            }
            else
            {
                Type Crypter = loadedTypes.Where((type) => type.Name == "Crypter").FirstOrDefault();
                Type CrypterProxy = loadedTypes.Where((type) => type.Name == "CrypterProxy").FirstOrDefault();
                object crypter = Activator.CreateInstance(Crypter);
                object crypterProxy = Activator.CreateInstance(CrypterProxy, new object[] { crypter, 10 });
                //object crypter = (Crypter as ICrypter).GetInstance();
                byte[] data;
                using (FileStream fs = new FileStream("people.encrypt", FileMode.Open))
                {
                    data = new byte[fs.Length];
                    fs.Read(data, 0, data.Length);
                }
                data = (crypterProxy as ICrypter).Decrypt(data);
                //Console.WriteLine(Encoding.UTF8.GetString(data));
                using (MemoryStream ms = new MemoryStream(data))
                {
                    deserializedObjects = (HashSet<object>)jsonFormatter.ReadObject(ms);
                }                
            }
            foreach (object curObj in deserializedObjects)
            {
                Type curType = curObj.GetType();
                FieldInfo[] curObjFields = curType.GetFields(BindingFlags.Instance | BindingFlags.NonPublic);
                string curObjContent = curType.ToString() + ":\n";
                for (int i = 0; i < curObjFields.Length; i++)
                {
                    curObjContent += "    " + curObjFields[i].Name + ": " + curObjFields[i].GetValue(curObj) + "\n";
                }
                MessageBox.Show(curObjContent, "Deserialize", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void btnAddAssemblyClick(object sender, RoutedEventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            if (fileDialog.ShowDialog() == false)
                return;
            
            MessageBox.Show($"{AddAssembly(fileDialog.FileName, true)} class(es) added", 
                            "Add assembly", 
                            MessageBoxButton.OK, 
                            MessageBoxImage.Information);
        }

        private void btnSubscribeClick(object sender, RoutedEventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            if (fileDialog.ShowDialog() == false)
                return;

            MessageBox.Show($"File was subscribed.\nSign file: {SubscribeFile(fileDialog.FileName)}",
                           "Subscribe",
                           MessageBoxButton.OK,
                           MessageBoxImage.Information);
        }
    }
}
