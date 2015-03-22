using System;
using NUnit.Framework;

namespace GardenManager.Core.Tests
{
	public class BaseTestFixture
	{
		public BaseTestFixture ()
		{
		}

		[SetUp]
		public void SetUp()
		{
			Console.WriteLine ("Setting up test: " + this.GetType ().Name);

			new TestFixtureRelocator ().Relocate ();
		}
	}
}

