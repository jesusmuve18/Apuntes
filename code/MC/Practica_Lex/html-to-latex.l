%option noyywrap
/*----- Sección de Declaraciones ----------------*/
%{
    #include <iostream>
    #include <fstream>
    #include <string>

    using namespace std;

    ifstream fichero;
    string resultado;
%}

/*----- Sección de Definiciones ----------------*/
BOLD    \<strong\>[^\<]*\<\/strong\>

/*----- Sección de Reglas ----------------*/
%%
{BOLD}  {
    string tmp = yytext;
    /* Procesa el contenido entre <strong> y </strong> */
    resultado += "\\textbf{" + tmp.substr(8, tmp.length() - 17) + "}";
}

.|\n    {
    string tmp = yytext;
    resultado += tmp;}
%%

/*----- Sección de Procedimientos --------*/
int main(int argc, char *argv[]) {
    if (argc == 2) {
        fichero.open(argv[1]);
        if (!fichero.is_open()) {
            cout << "Error de lectura" << endl;
            return 1;
        }
    } else {
        cout << "Uso: " << argv[0] << " <archivo>" << endl;
        return 1;
    }

    // Configura Flex para leer desde el archivo
    yyFlexLexer flujo (&fichero,0);
    flujo.yylex();

    cout << "El resultado es: \n" << resultado << endl;

    return 0;
}
