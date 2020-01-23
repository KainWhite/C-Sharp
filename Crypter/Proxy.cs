using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibraryInterfaces;

namespace CryptClasses
{
    public class CrypterProxy: ICrypter
    {
        private Crypter crypter;
        private int permission;
        public CrypterProxy(Crypter _crypter, int _permission)
        {
            crypter = _crypter;
            permission = _permission;
            Console.WriteLine(crypter);
        }
        public byte[] Decrypt(byte[] data)
        {
            if (permission > 5)
                return crypter.Decrypt(data);
            else
                return null;
        }
        public byte[] Encrypt(byte[] data)
        {
            if (permission > 5)
                return crypter.Encrypt(data);
            else
                return null;
        }
    }
}
