namespace LibraryInterfaces
{
    public interface IBuilder
    {
        object Build(int cost, string adress, int area, int floorCount);
    }
}
