#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <math.h>

//Esse é o add do BSTR
#include <comdef.h>

using namespace std;

extern "C" __declspec(dllexport) BSTR GetData(const char* a, const char* b);
extern "C" __declspec(dllexport) BSTR ResgateBruto(const char* principal, const char* taxaAABruta, const char* vencDias, const char* base);
extern "C" __declspec(dllexport) BSTR ResgateEstimado(const char* resgateBruto, const char* principal, const char* aliquota);

//Pode ser usado tanto para os juros corridos quanto os juros uteis é só trocar a base e o vencDias (para dias corridos ou úteis)
extern "C" __declspec(dllexport) BSTR FatorJuros(const char* taxaAABruta, const char* vencDias, const char* base);

//Pode ser os juros corridos ou os úteis
extern "C" __declspec(dllexport) BSTR PUpreFix(const char* principal, const char* fatorJuros);

//É a mesma coisa que o fatorJuros, mas por questão de nomenclatura criarei esta também
extern "C" __declspec(dllexport) BSTR TDIk(const char* taxaAABruta, const char* taxaDI, const char* vencDias, const char* base);
extern "C" __declspec(dllexport) BSTR FatorDI(const char* fatorAnt, const char* tDIk);

extern "C" __declspec(dllexport) BSTR TaxaSpread(const char* taxaAABruta);

// É a mesma coisa que o fatorJuros, mas por questão de nomenclatura criarei esta também. Pode ser usado para fator spread 
//e o fator spread dos dias úteis
extern "C" __declspec(dllexport) BSTR FatorSpread(const char* taxaSpread, const char* vencDias, const char* base);
extern "C" __declspec(dllexport) BSTR UpdateFatorSpread(const char* ultimoFator, const char* primeiroFator);

//Cálculo do fator de juros do pos fixado CDI+
extern "C" __declspec(dllexport) BSTR FatorJurosCDI(const char* fatorDI, const char* fatorSpread, const char* taxaSpread, const char* tDIk);
extern "C" __declspec(dllexport) BSTR UpdateFatorJurosCDI(const char* fatorJurosAnt, const char* TDIk);

extern "C" __declspec(dllexport) BSTR PuOperacao(const char* fatorJuros, const char* principal, const char* taxaAABruta, const char* vencDias, const char* base);
extern "C" __declspec(dllexport) BSTR CDImaisPU(const char* fatorJuros, const char* principal, const char* puOperacao);

extern "C" __declspec(dllexport) BSTR PorcentCDIPU(const char* principal, const char* puOperacao);
