namespace LibraryInterfaces
{
    public interface ICrypter
    {
        byte[] Encrypt(byte[] path);
        byte[] Decrypt(byte[] path);
    }
}
