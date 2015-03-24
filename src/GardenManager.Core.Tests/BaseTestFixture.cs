using System;
using NUnit.Framework;
using GardenManager.Core;

namespace GardenManager.Core.Tests
{
	public class BaseTestFixture
	{
		public DataStore Store { get;set; }

		public BaseTestFixture ()
		{
		}

		[SetUp]
		public void SetUp()
		{
			Console.WriteLine ("Setting up test: " + this.GetType ().Name);

			new TestFixtureRelocator ().Relocate ();
		}

		[TearDown]
		public void TearDown()
		{
			if (Store != null)
				Store.DeleteAll ();
		}

		public DataStore GetDataStore()
		{
			if (Store == null)
				Store = new DataStore ("eCG-Tests");
			return Store;
		}
	}
}

