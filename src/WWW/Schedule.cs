using System;
using Quartz;
using Quartz.Impl;

namespace GardenManager
{
	public class Schedule
	{
		public static void Start()
		{
			IScheduler scheduler = StdSchedulerFactory.GetDefaultScheduler();
			scheduler.Start();

			IJobDetail job = JobBuilder.Create<ReadCaptureDataJob>().Build();

			ITrigger trigger = TriggerBuilder.Create()
				.StartNow()
				.WithSimpleSchedule(x => x
					.WithIntervalInSeconds(5)
					.RepeatForever())
				.Build();

			scheduler.ScheduleJob(job, trigger);
		}
	}
}

