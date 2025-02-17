using DataLayer;
using DataLayer.Structures;
using Microsoft.AspNetCore.Mvc;
using Web.Models;

namespace Web.Controllers
{
    public class Api : Controller
    {
        private MyDapper dapper;
        public Api(DataService dataService)
        {
            this.dapper = dataService.GetDapper();
        }
        public IActionResult Index()
        {
            return new JsonResult(new { status = true, message = "Hello! :)" });
        }

        [HttpPost]
        public async Task<IActionResult> Form(MainForm form)
        {
            if (!ModelState.IsValid)
            {
                return new JsonResult(new { status = false, message = "Neplatný formulář" });
            }

            //check data
            if (form.program1 == "")
            {
                return new JsonResult(new { status = false, message = "Nevybral jsi 1. program" });
            }

            if (form.hs1 == 0)
            {
                return new JsonResult(new { status = false, message = "Nevybral jsi žádnou školu" });
            }

            if (form.hs2 > 0 && form.program2 == "")
            {
                return new JsonResult(new { status = false, message = "Vybral jsi školu 2, ale nevybral jsi program" });
            }

            if (form.hs3 > 0 && form.program3 == "")
            {
                return new JsonResult(new { status = false, message = "Vybral jsi školu 3, ale nevybral jsi program" });
            }

            if (form.housenumber == 0)
            {
                return new JsonResult(new { status = false, message = "Zadal jsi neplatné číslo domu" });
            }

            if (form.postalcode < 10000 || form.postalcode > 99999)
            {
                return new JsonResult(new { status = false, message = "Zadal jsi neplatné PSČ" });
            }

            //check same programs
            if (
                (form.hs1 == form.hs2 && form.program1 == form.program2 && form.program2 != null) ||
                (form.hs2 == form.hs3 && form.program2 == form.program3 && (form.program2 != null || form.program3 != null)) ||
                (form.hs1 == form.hs3 && form.program1 == form.program3 && form.program3 != null)
            )
            {
                return new JsonResult(new { status = false, message = "Nemůžeš vybrat dva stejné programy na stejné škole" });
            }

            //save to db



            Student student = new()
            {
                Name = form.name,
                Surname = form.surname,
                State = form.state,
                City = form.town,
                Street = form.street,
                Region = form.region,
                HouseNumber = form.housenumber,
                PostalCode = form.postalcode,
                Phone = form.phone,
                Email = form.email
            };

            int studentId = await dapper.Insert(student);

            Application application = new()
            {
                Student = studentId,
                Date = DateTime.Now,
                StudyProgram1 = form.program1,
                StudyProgram2 = form.program2,
                StudyProgram3 = form.program3,
            };

            await dapper.Insert(application);

            return new JsonResult(new { status = true });
        }

        public async Task<IActionResult> CheckStudent(UserCheck user)
        {
            if (!ModelState.IsValid)
            {
                return new JsonResult(new { status = false, message = "Neplatná data" });
            }

            var result = await dapper.Select<Student>($"Phone = \"{user.Phone}\" OR Email = \"{user.Email}\"");
            var count = result.Count();

            return new JsonResult(new { status = true, count });
        }

        public async Task<IActionResult> GetSchools(SchoolSearch search)
        {
            if (!ModelState.IsValid)
            {
                return new JsonResult(new { status = false, message = "Neplatná data" });
            }

            var result = await dapper.Select<HighSchool>($"Name LIKE \"%{search.Text}%\" LIMIT 5");
            var items = result.Select((school) => new { id = school.Id, name = school.Name }).ToList();

            return new JsonResult(new { status = true, items });
        }

        public async Task<IActionResult> GetPrograms(SchoolId schoolId)
        {
            if (!ModelState.IsValid)
            {
                return new JsonResult(new { status = false, message = "Neplatná data" });
            }

            var result = await dapper.Select<HighSchoolProgram, int>(new Dictionary<string, object>() { { "HighSchool", schoolId.Id } });
            var items = result.Select((Program) => new { id = Program.Identifier, name = Program.Name }).ToList();

            return new JsonResult(new { status = true, items });
        }
    }
}
