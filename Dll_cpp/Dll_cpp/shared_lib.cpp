#include "pch.h"
#include "shared_lib.h"

using namespace std;
//using boost::lexical_cast;
//using boost::bad_lexical_cast;
using namespace boost::multiprecision;


BSTR GetData(const char* a, const char* b)
{
    /*const char* aStr((const char*)a);
    const char* bStr((const char*)b);*/

    /*_bstr_t aShort(a);
    const char* aStr = aShort;

    _bstr_t bShort(a);
    const char* bStr = aShort;*/

    cpp_bin_float_50 A = cpp_bin_float_50(a);
    cpp_bin_float_50 B = cpp_bin_float_50(b);

    cpp_bin_float_50 res = A * B;
    /*boost::multiprecision::cpp_dec_float_50 d("1212121212.121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212");
cout << d.convert_to<string>() << '\n';*/
    return _bstr_t(res.str(18).c_str()).Detach();
}


BSTR ResgateBruto(const char* principal, const char* taxaAABruta, const char* vencDias, const char* base)
{
    cpp_bin_float_50 resgateBruto = cpp_bin_float_50(principal) * pow((1.0 + cpp_bin_float_50(taxaAABruta)), (cpp_bin_float_50(vencDias) / cpp_bin_float_50(base)));
    return _bstr_t(resgateBruto.str(18).c_str()).Detach();
}

BSTR ResgateEstimado(const char* resgateBruto, const char* principal, const char* aliquota)
{
    cpp_bin_float_50 resBrut = cpp_bin_float_50(resgateBruto);
    cpp_bin_float_50 resgateEstimado = resBrut - ((resBrut - cpp_bin_float_50(principal)) * cpp_bin_float_50(aliquota));

    return _bstr_t(resgateEstimado.str().c_str()).Detach();
}

//Pode ser usado tanto para os juros corridos quanto os juros uteis é só trocar a base e o vencDias (para dias corridos ou úteis)
BSTR FatorJuros(const char* taxaAABruta, const char* vencDias, const char* base)
{
    cpp_bin_float_50 fatorJuros = pow((1.0 + cpp_bin_float_50(taxaAABruta)), (cpp_bin_float_50(vencDias) / cpp_bin_float_50(base)));

    int casas = log10((double)fatorJuros) + 1;

    return _bstr_t(fatorJuros.str(casas + 13).c_str()).Detach();
}

//Pode ser os juros corridos ou os úteis
BSTR PUpreFix(const char* principal, const char* fatorJuros)
{
    cpp_bin_float_50 PU = cpp_bin_float_50(principal) * cpp_bin_float_50(fatorJuros);
    return _bstr_t(PU.str(8).c_str()).Detach();
}



//É a mesma coisa que o fatorJuros, mas por questão de nomenclatura criarei esta também
BSTR TDIk(const char* taxaAABruta, const char* taxaDI, const char* vencDias, const char* base)
{
    cpp_bin_float_50 tDIk = 0.0;
    cpp_bin_float_50 taxaBruta = cpp_bin_float_50(taxaAABruta);

    tDIk = pow((1.0 + cpp_bin_float_50(taxaDI)), (cpp_bin_float_50(vencDias) / cpp_bin_float_50(base))) - 1.0;
    
    int casas = log10((double)tDIk) + 1;

    //truncamento de 8 casas decimas. Tenho que buscar ARREDONDAMENTO
    return _bstr_t(tDIk.str(casas+8).c_str()).Detach();
}

BSTR FatorDI(const char* fatorAnt, const char* tDIk)
{
    cpp_bin_float_50 fatorDI_float = cpp_bin_float_50(fatorAnt) * cpp_bin_float_50(tDIk);


    //int casas = log10((double)fatorDI_float) + 1;

    /*string saida;
    std::stringstream buffer;  
    buffer << std::fixed;
    buffer << fatorDI_float.str(40);

    saida = buffer.str();*/

    ostringstream oss;
    oss.precision(16);
    oss << std::fixed << fatorDI_float;
    string str = oss.str();

    // Remove padding
    // This must be done in two steps because of numbers like 700.00
    std::size_t pos1 = str.find_last_not_of("0");
    if (pos1 != std::string::npos)
        str.erase(pos1 + 1);

    std::size_t pos2 = str.find_last_not_of(".");
    if (pos2 != std::string::npos)
        str.erase(pos2 + 1);


    return _bstr_t(str.c_str()).Detach();
}

BSTR TaxaSpread(const char* taxaAABruta)
{
    cpp_bin_float_50 taxaBruta = cpp_bin_float_50(taxaAABruta);
    cpp_bin_float_50 taxaSpread = 0.0;

    if (taxaBruta > 1.0)
    {
        taxaSpread = taxaBruta - 1.0;
    }
    else
    {
        //Se a taxa spread for 0 então a gente sabe que a porcentagem do CDI é menor que 100 por cento
        taxaSpread = 0.0;
    }

    return _bstr_t(taxaSpread.str().c_str()).Detach();
}


// É a mesma coisa que o fatorJuros, mas por questão de nomenclatura criarei esta também. Pode ser usado para fator spread 
//e o fator spread dos dias úteis
BSTR FatorSpread(const char* taxaSpread, const char* vencDias, const char* base)
{
    cpp_bin_float_50 fatorSpread = pow((1.0 + cpp_bin_float_50(taxaSpread)), (cpp_bin_float_50(vencDias) / cpp_bin_float_50(base)));

    int casas = log10((double)fatorSpread) + 1;

    return _bstr_t(fatorSpread.str(casas+11).c_str()).Detach();
}

BSTR UpdateFatorSpread(const char* ultimoFator, const char* primeiroFator)
{
    cpp_bin_float_50 fatorSpread = cpp_bin_float_50(ultimoFator) * cpp_bin_float_50(primeiroFator);

    //int casas = log10((double)fatorSpread) + 1;
    ostringstream oss;
    oss.precision(8);
    oss << std::fixed << fatorSpread;
    string str = oss.str();

    // Remove padding
    // This must be done in two steps because of numbers like 700.00
    std::size_t pos1 = str.find_last_not_of("0");
    if (pos1 != std::string::npos)
        str.erase(pos1 + 1);

    std::size_t pos2 = str.find_last_not_of(".");
    if (pos2 != std::string::npos)
        str.erase(pos2 + 1);


    return _bstr_t(str.c_str()).Detach();
}


//Cálculo do fator de juros do pos fixado CDI+
BSTR FatorJurosCDI(const char* fatorDI, const char* fatorSpread, const char* taxaSpread, const char* tDIk)
{
    cpp_bin_float_50 taxaSpread_float = cpp_bin_float_50(taxaSpread);
    cpp_bin_float_50 fatorDI_float = cpp_bin_float_50(fatorDI);
    cpp_bin_float_50 fatorSpread_float = cpp_bin_float_50(fatorSpread);

    cpp_bin_float_50 fatorJuros = 0.0;

    if (taxaSpread_float == 0.0)
    {
        fatorJuros = cpp_bin_float_50(tDIk);
    }
    else
    {
        fatorJuros = fatorDI_float * fatorSpread_float;
    }

    return _bstr_t(fatorJuros.str(11).c_str()).Detach();
}

BSTR UpdateFatorJurosCDI(const char* fatorJurosAnt, const char* tDIk) 
{
    cpp_bin_float_50 fatorJuros = cpp_bin_float_50(fatorJurosAnt) * cpp_bin_float_50(tDIk);

    //int casas = log10((double)fatorJuros) + 1;

    ostringstream oss;
    oss.precision(8);
    oss << std::fixed << fatorJuros;
    string str = oss.str();

    // Remove padding
    // This must be done in two steps because of numbers like 700.00
    std::size_t pos1 = str.find_last_not_of("0");
    if (pos1 != std::string::npos)
        str.erase(pos1 + 1);

    std::size_t pos2 = str.find_last_not_of(".");
    if (pos2 != std::string::npos)
        str.erase(pos2 + 1);


    return _bstr_t(str.c_str()).Detach();
}


BSTR PuOperacao(const char* fatorJuros, const char* principal, const char* taxaAABruta, const char* vencDias, const char* base)
{
    cpp_bin_float_50 puOperacao = cpp_bin_float_50(fatorJuros) * cpp_bin_float_50(principal) * (pow((1.0 + cpp_bin_float_50(taxaAABruta)), (cpp_bin_float_50(vencDias) / cpp_bin_float_50(base))));
    return _bstr_t(puOperacao.str().c_str()).Detach();
}

BSTR CDImaisPU(const char* fatorJuros, const char* principal, const char* puOperacao)
{
    cpp_bin_float_50 PU = (cpp_bin_float_50(fatorJuros) * cpp_bin_float_50(principal)) + cpp_bin_float_50(puOperacao);
    return _bstr_t(PU.str(8).c_str()).Detach();
}


BSTR PorcentCDIPU(const char* principal, const char* puOperacao)
{
    cpp_bin_float_50 PU = cpp_bin_float_50(principal) + cpp_bin_float_50(puOperacao);
    return _bstr_t(PU.str(8).c_str()).Detach();
}