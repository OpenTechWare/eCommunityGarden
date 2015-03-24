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

			var data1 = dataStore.GetValues (DeviceId.Parse("1.1.1"), "Mst");

			Assert.AreEqual (2, data1.Count, "Incorrect number of values.");

			var data2 = dataStore.GetValues (DeviceId.Parse("1.1.2"), "Mst");

			Assert.AreEqual (2, data2.Count, "Incorrect number of values.");
		}

		void CreateSerialLogFile()
		{
			var content = @"D;T:2015-02-09 11:36:59; ID:1.1.1; LtOut:54; Mst:5; Lt:63; Hm:57.0; Tmp:22.0; Fl:0.0000; 
D;T:2015-02-09 11:37:01; ID:1.1.2; LtOut:54; Mst:20; Lt:60; Hm:58.0; Tmp:23.0; Fl:0.0000; 
D;T:2015-02-09 11:37:03; ID:1.1.1; LtOut:54; Mst:19; Lt:61; Hm:59.0; Tmp:24.0; Fl:0.0000; 
D;T:2015-02-09 11:37:05; ID:1.1.2; LtOut:54; Mst:18; Lt:62; Hm:57.0; Tmp:25.0; Fl:0.0000; ";

			var fileName = Path.GetFullPath ("serialLog.txt");

			File.WriteAllText (fileName, content);
		}
	}
}

