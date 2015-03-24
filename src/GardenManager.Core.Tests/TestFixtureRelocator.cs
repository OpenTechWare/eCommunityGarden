using System;
using System.IO;

namespace GardenManager.Core.Tests
{
	public class TestFixtureRelocator
	{
		public TestFixtureRelocator ()
		{
		}

		public void Relocate()
		{
			var tmpDir = Path.GetFullPath ("_tmp");

			if (!Directory.Exists (tmpDir))
				Directory.CreateDirectory (tmpDir);

			var dateDir = Path.Combine (tmpDir, DateTime.Now.ToString ());

			if (!Directory.Exists (dateDir))
				Directory.CreateDirectory (dateDir);

			Environment.CurrentDirectory = dateDir;

			Console.WriteLine ("Relocating...");
			Console.WriteLine (Environment.CurrentDirectory);
		}
	}
}

