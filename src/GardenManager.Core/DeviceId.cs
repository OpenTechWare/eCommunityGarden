using System;

namespace GardenManager.Core
{
	public struct DeviceId
	{
		public int Id1;
		public int Id2;
		public int Id3;

		public DeviceId (int id1, int id2, int id3)
		{
			this.Id1 = id1;
			this.Id2 = id2;
			this.Id3 = id3;
		}

		public static DeviceId Parse(string idString)
		{
			var parts = idString.Split ('.');
			if (parts.Length != 3)
				throw new ArgumentException ("Invalid id string: " + idString);

			return new DeviceId (
				Convert.ToInt32(parts [0]),
				Convert.ToInt32(parts [1]),
				Convert.ToInt32(parts [2])
			);
		}

		public string ToString()
		{
			return Id1 + "." + Id2 + "." + Id3;
		}
	}
}

