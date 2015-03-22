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

			var conversion = new DataConversion ();

			conversion.ConvertFileToData ();

			var dataStore = new DataStore ();

			var data = dataStore.GetValues ("Mst");

			Assert.AreEqual (10, data.Count, "Incorrect number of values.");
		}

		void CreateSerialLogFile()
		{
			var content = @"D;T:2015-02-09 11:36:59; LtOut:54; Mst:5; Lt:63; Hm:57.0; Tmp:22.0; Fl:0.0000; 
D;T:2015-02-09 11:37:01; LtOut:54; Mst:20; Lt:60; Hm:58.0; Tmp:23.0; Fl:0.0000; 
D;T:2015-02-09 11:37:03; LtOut:54; Mst:19; Lt:61; Hm:59.0; Tmp:24.0; Fl:0.0000; 
D;T:2015-02-09 11:37:05; LtOut:54; Mst:18; Lt:62; Hm:57.0; Tmp:25.0; Fl:0.0000; 
D;T:2015-02-09 11:37:08; LtOut:54; Mst:17; Lt:63; Hm:58.0; Tmp:26.0; Fl:0.0000; 
D;T:2015-02-09 11:37:10; LtOut:54; Mst:16; Lt:64; Hm:58.0; Tmp:27.0; Fl:0.0000; 
D;T:2015-02-09 11:37:12; LtOut:54; Mst:15; Lt:65; Hm:55.0; Tmp:28.0; Fl:0.0000; 
D;T:2015-02-09 11:37:14; LtOut:54; Mst:14; Lt:66; Hm:56.0; Tmp:27.0; Fl:0.0000; 
D;T:2015-02-09 11:37:16; LtOut:54; Mst:13; Lt:67; Hm:58.0; Tmp:26.0; Fl:0.0000; 
D;T:2015-02-09 11:37:18; LtOut:54; Mst:12; Lt:68; Hm:58.0; Tmp:25.0; Fl:0.0000;";

			var fileName = Path.GetFullPath ("serialLog.txt");

			File.WriteAllText (fileName, content);
		}
	}
}

