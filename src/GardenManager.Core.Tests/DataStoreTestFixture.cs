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
			var data = new Dictionary<string, string> ();

			var dateTime = DateTime.Now;

			var id = DeviceId.Parse ("1.1.1");

			for (int i = 0; i < 10; i++) {
				data.Add (dateTime.AddMinutes (i).ToString(), i + "." + i);
			}

			var store = new DataStore ();
			store.AddData (id, "TestData", data);

			var foundData = store.GetValues (id, "TestData");

			foreach (var key in foundData.Keys) {
				Console.Write (key);
				Console.Write (",");
				Console.Write (foundData [key]);
				Console.WriteLine (";");

			}
		}
	}
}

