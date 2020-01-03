using System.Diagnostics;
using DogSpeak.Models;
using DogSpeak.Services;
using Microsoft.AspNetCore.Mvc;

namespace DogSpeak.Controllers
{
	public class HomeController : Controller
	{
		private SoundPlayer soundPlayer = new SoundPlayer();

		public IActionResult Index()
		{
			return View();
		}

		public IActionResult LetsGoForAWalk()
		{
			return PlaySound("LetsGoForAWalk.mp3");
		}

		public IActionResult ImHungry()
		{
			return PlaySound("ImHungry.mp3");
		}

		public IActionResult ImThirsty()
		{
			return PlaySound("ImThirsty.mp3");
		}

		public IActionResult IWantToPlay()
		{
			return PlaySound("IWantToPlay.mp3");
		}

		public IActionResult INeedToPee()
		{
			return PlaySound("INeedToPee.mp3");
		}

		public IActionResult INeedToPoop()
		{
			return PlaySound("INeedToPoop.mp3");
		}

		public IActionResult INeedCare()
		{
			return PlaySound("INeedCare.mp3");
		}

		public IActionResult SleepyDog()
		{
			return PlaySound("SleepyDog.mp3");
		}

		[ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
		public IActionResult Error()
		{
			return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
		}

		private IActionResult PlaySound(string path)
		{
			soundPlayer.Play(path);
			return View();
			return new EmptyResult();
		}
	}
}
