using System;
using NUnit.Framework;
using System.IO;

namespace GardenManager.Core.Tests
{
	[TestFixture]
	public class DataConversionTestFixture : BaseTestFixture
	{
		[Test]
		public void Test_ConvertFileToData()
		{
			CreateSerialLogFile ();

			var dataStore = GetDataStore ();

			var conversion = new DataConversion (dataStore);

			conversion.ConvertFileToData ();

			var deviceIds = dataStore.GetDeviceIds ();

			Assert.AreEqual (2, deviceIds.Length, "Incorrect number of devices.");

			var data1 = dataStore.GetValues (DeviceId.Parse("1.1.1"), 1);

			Assert.AreEqual (2, data1.Count, "Incorrect number of values.");

			var data2 = dataStore.GetValues (DeviceId.Parse("1.1.2"), 1);

			Assert.AreEqual (2, data2.Count, "Incorrect number of values.");
		}

		void CreateSerialLogFile()
		{
			var content = @"D;T:2015-02-09 11:36:59; Id:1.1.1; S:1; V:5;
D;T:2015-02-09 11:36:59; Id:1.1.2; S:1; V:6;
D;T:2015-02-09 11:36:59; Id:1.1.1; S:1; V:3;
D;T:2015-02-09 11:36:59; Id:1.1.2; S:1; V:4;";

			var fileName = Path.GetFullPath ("serialLog.txt");

			File.WriteAllText (fileName, content);
		}
	}
}

