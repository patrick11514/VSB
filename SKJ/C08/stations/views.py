from django.http import HttpResponse
from django.shortcuts import get_object_or_404, redirect, render

from .forms import CompanyForm
from .models import Company, Station

# Create your views here.


def index(request):
    companies = Company.objects.all()
    company_form = CompanyForm()

    return render(
        request,
        "stations/index.html",
        {"companies": companies, "company_form": company_form},
    )


def company(request, company_id: int):
    company = get_object_or_404(Company, pk=company_id)
    stations = Station.objects.filter(company=company)

    return render(
        request, "stations/company.html", {"company": company, "stations": stations}
    )


def add_company(request):
    if request.method == "POST":
        company_form = CompanyForm(request.POST)
        if company_form.is_valid():
            name = company_form.cleaned_data["name"]
            company = Company(name=name)
            company.save()

            return redirect("index")
