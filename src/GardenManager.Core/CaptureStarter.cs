using System;
using System.Diagnostics;
using System.IO;

namespace GardenManager.Core
{
	public class CaptureStarter
	{
		public CaptureStarter ()
		{
		}

		public void Start()
		{
			var scriptFile = Path.GetFullPath ("captureSerial.sh");
			
			ProcessStartInfo psi = new ProcessStartInfo();
			psi.FileName = "sh";
			psi.Arguments = scriptFile;
			//psi.UseShellExecute = false;
			//psi.RedirectStandardOutput = true;
			psi.CreateNoWindow = true;
			psi.WindowStyle = ProcessWindowStyle.Hidden;

			Process.Start(psi);
			//string strOutput = p.StandardOutput.ReadToEnd();
			//p.WaitForExit();
			//Console.WriteLine(strOutput);
		}
	}
}

