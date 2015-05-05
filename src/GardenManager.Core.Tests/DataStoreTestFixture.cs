using System;
using NUnit.Framework;
using System.Collections.Generic;

namespace GardenManager.Core.Tests
{
	[TestFixture]
	public class DataStoreTestFixture : BaseTestFixture
	{
		[Test]
		public void Test_Add()
		{
			var dateTime = DateTime.Now;

			var id = DeviceId.Parse ("1.1.1");

			var store = new DataStore ();

			for (int i = 0; i < 10; i++) {
				store.AddData (id, 1, 1, dateTime.AddMinutes (i), i);
			}

			var foundData = store.GetValues (id, 1);

			foreach (var key in foundData.Keys) {
				Console.Write (key);
				Console.Write (",");
				Console.Write (foundData [key]);
				Console.WriteLine (";");

			}
		}
	}
}

