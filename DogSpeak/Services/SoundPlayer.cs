using System;
using System.IO;
using NetCoreAudio;

namespace DogSpeak.Services
{
	public class SoundPlayer
	{
		public void Play(string filename)
		{
			var fullPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Sounds", filename);
			var player = new Player();
			player.Play(fullPath);
		}
	}
}
