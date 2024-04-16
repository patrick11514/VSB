using System.ComponentModel.DataAnnotations;

namespace C08.Models
{
	public class CartForm
	{
		[Display(Name = "Jméno")]
		public string Name { get; set; }
		[Required]
		[EmailAddress]
		[Display(Name = "Email")]
		public string Email { get; set; }
		[Required]
		[Range(10, 100)]
		[Display(Name = "Věk")]
		public int? Age { get; set; }
	}
}
