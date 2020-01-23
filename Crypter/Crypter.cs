using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace CryptClasses
{
    public class Crypter : ICrypter
    {
        //private static Crypter instance;
        //public static Crypter GetInstance()
        //{
        //    if (instance == null)
        //    {
        //        instance = new Crypter();
        //    }
        //    return instance;
        //}
        public byte[] Encrypt(byte[] data)
        {
            return Encoding.UTF8.GetBytes(Convert.ToBase64String(data));
        }
        public byte[] Decrypt(byte[] data)
        {
            return Convert.FromBase64String(Encoding.UTF8.GetString(data));
        }
    }
}
