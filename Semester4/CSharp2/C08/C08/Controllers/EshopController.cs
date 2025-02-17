using C08.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;

namespace C08.Controllers
{
	public class EshopController : Controller
	{
		private readonly ProductsService productsService;
		private readonly CartService cartService;

		public EshopController(ProductsService productsService, CartService cartService)
		{
			this.productsService = productsService;
			this.cartService = cartService;
		}
		public IActionResult Index()
		{
			ViewBag.Products = productsService.List();
			return View();
		}

		public IActionResult Detail(int id)
		{
			Product p = productsService.Find(id);
			if (p == null)
			{
				return NotFound();
			}

			ViewBag.Product = p;
			return View();
		}

		public IActionResult AddToCart(int productId/*, [FromServices] CartService cartService*/)
		{
			cartService.Add(productsService.Find(productId));
			return RedirectToAction("Detail", new { id = productId });
		}

		public override void OnActionExecuted(ActionExecutedContext context)
		{
			ViewBag.ProductCount = cartService.Count;
			base.OnActionExecuted(context);
		}

		public IActionResult Form()
		{
			ViewBag.Products = cartService.GetProducts();
			return View();
		}

		[HttpPost]
		public IActionResult Form(CartForm form)
		{
			ViewBag.Products = cartService.GetProducts();
			if (ModelState.IsValid)
			{
				//formulář je validní

			}
			return View();
		}

		public IActionResult Json(int limit)
		{
			return new JsonResult(productsService.List().Take(limit));
		}
	}
}
