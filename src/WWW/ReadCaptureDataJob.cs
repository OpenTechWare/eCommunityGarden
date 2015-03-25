using System;
using Quartz;
using Quartz.Impl;
using GardenManager.Core;

namespace GardenManager
{
	public class ReadCaptureDataJob : IJob
	{
		public void Execute(IJobExecutionContext context)
		{
			var store = new DataStore();

			var conversion = new DataConversion(store);
			conversion.ConvertFileToData();

		}
	}
}

