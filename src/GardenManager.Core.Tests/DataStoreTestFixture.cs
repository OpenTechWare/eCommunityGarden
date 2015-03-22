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
			var data = new Dictionary<string, double> ();

			var dateTime = DateTime.Now;

			for (int i = 0; i < 10; i++) {
				data.Add (dateTime.AddMinutes (i).ToString(), Convert.ToDouble(i + "." + i));
			}

			var store = new DataStore ();
			store.Add ("TestData", data);

			var foundData = store.GetValues ("TestData");

			foreach (var key in foundData.Keys) {
				Console.Write (key);
				Console.Write (",");
				Console.Write (foundData [key]);
				Console.WriteLine (";");

			}
		}
	}
}

