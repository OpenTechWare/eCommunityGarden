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

			var code1 = dataStore.GetSensorCode (DeviceId.Parse ("1.1.1"), 1);

			Assert.AreEqual (1, code1, "Incorrect sensor code");

			var code12 = dataStore.GetSensorCode (DeviceId.Parse ("1.1.1"), 2);

			Assert.AreEqual (1, code12, "Incorrect sensor code");

			var code2 = dataStore.GetSensorCode (DeviceId.Parse ("1.1.1"), 1);

			Assert.AreEqual (1, code2, "Incorrect sensor code");

			var code22 = dataStore.GetSensorCode (DeviceId.Parse ("1.1.1"), 2);

			Assert.AreEqual (1, code22, "Incorrect sensor code");
		}

		void CreateSerialLogFile()
		{
			var content = @"D;T:2015-02-09 11:36:59; Id:1.1.1; N:1; S:1; V:5;
D;T:2015-02-09 11:36:59; Id:1.1.1; N:1; S:1; V:6;

D;T:2015-02-09 11:36:59; Id:1.1.1; N:2; S:1; V:3;
D;T:2015-02-09 11:36:59; Id:1.1.1; N:2; S:1; V:8;

D;T:2015-02-09 11:36:59; Id:1.1.2; N:1; S:1; V:6;
D;T:2015-02-09 11:36:59; Id:1.1.2; N:1; S:1; V:7;

D;T:2015-02-09 11:36:59; Id:1.1.2; N:2; S:1; V:4;
D;T:2015-02-09 11:36:59; Id:1.1.2; N:2; S:1; V:3;";

			var fileName = Path.GetFullPath ("serialLog.txt");

			File.WriteAllText (fileName, content);
		}
	}
}

